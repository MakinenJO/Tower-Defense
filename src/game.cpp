#include "game.h"
#include "tower.h"
#include "bullet.h"
#include "enemy.h"
#include "buildtowericon.h"
#include "highscore.h"
#include "button.h"
#include "map.h"

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QDebug>
#include <QCloseEvent>
#include <QTimer>
#include <QHoverEvent>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QLineEdit>
#include <QInputDialog>

Game::Game()
{
    //set qwidget size and scroll bars
    setFixedSize(1280, 720);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //create a scene
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0,0,1280,720);

    //set the scene
    setScene(scene);

    //set attributes
    cursor = nullptr;
    build = nullptr;
    setMouseTracking(true);
    playerName = "Player1";

    //make a font for the game to use
    font_large = QFont("Lucida Console", 22, QFont::Bold);
    font_large.setStyleHint(QFont::Monospace);

    font_small = QFont("Lucida Console", 14, QFont::Bold);
    font_small.setStyleHint(QFont::Monospace);

    //add a highscore
    highscore = new Highscore();

    //add a playlist to loop music from
    playlist = new QMediaPlaylist();
    playlist->addMedia(QUrl("qrc:/sounds/bg_music.mp3"));
    playlist->setPlaybackMode(QMediaPlaylist::Loop);

    //set sound for taking damage
    hit = new QMediaPlayer();
    hit->setMedia(QUrl("qrc:/sounds/enemy_take_damage.mp3"));
    hit->setVolume(10);

    //set sound to deny build
    deny_sound = new QMediaPlayer();
    deny_sound->setMedia(QUrl("qrc:/sounds/button_deny.mp3"));
    deny_sound->setVolume(40);

    //play menu music
    music = new QMediaPlayer();
    music->setMedia(playlist);
    music->setVolume(20);
    music->play();
}

Game::~Game(){
    scene->clear();
    delete scene;
}

void Game::mousePressEvent(QMouseEvent *event)
{
    if(onEditor){
        if(event->button() == Qt::LeftButton){
            mapeditor->makeRoad(event->pos());
        }
        else if(event->button() == Qt::RightButton){
            mapeditor->deleteRoad(event->pos());
        }
            QGraphicsView::mousePressEvent(event);
    }
    else if (build){
        //leftclick builds, rightclick cancels
        if(event->button() == Qt::LeftButton){
        qDebug() << "Building tower at: " << event->pos();
        buildTower(event->pos());
        }

        if(event->button() == Qt::RightButton) {
            //qDebug() << "Building canceled";
            clearCursor();
        }
    }

    else {
        //if not in build mode, pass the event forward
        QGraphicsView::mousePressEvent(event);
    }
}

void Game::mouseMoveEvent(QMouseEvent *event)
{
    if (cursor){
        cursor->setPos(event->pos());
        cursor->setOffset(-cursor->pixmap().width()/2, -cursor->pixmap().height()/2);
        QPointF cursorAreaOffset(cursorArea->boundingRect().width()/2,cursorArea->boundingRect().height()/2);
        cursorArea->setPos(event->pos() - cursorAreaOffset);
    } else {
        QGraphicsView::mouseMoveEvent(event);
    }
}

void Game::closeEvent(QCloseEvent *event)
{
    highscore->saveHighscore();
    event->accept();
}

void Game::clearCursor(){
    scene->removeItem(cursor);
    delete cursor;
    scene->removeItem(cursorArea);
    delete cursorArea;

    //reset cursor and build mode
    cursor = nullptr;
    build = nullptr;
}

void Game::setCursor(QString filename, QPointF position)
{
    if (cursor){
        clearCursor();
    }
    cursor = new QGraphicsPixmapItem();
    cursor->setPixmap(QPixmap(filename));
    cursor->setPos(position);
    cursor->setScale(0.5);
    cursor->setOffset(-cursor->pixmap().width()/2, -cursor->pixmap().height()/2);
    scene->addItem(cursor);

    double range = build->getRange();
    setCursorArea(range, position);
}

void Game::setCursorArea(double diameter, QPointF position)
{
    cursorArea = new QGraphicsEllipseItem();
    cursorArea->setRect(0,0,diameter*2,diameter*2);
    cursorArea->setBrush(QBrush(Qt::red));
    cursorArea->setOpacity(0.3);

    //set positionoffset
    position -= QPointF(cursorArea->boundingRect().width()/2,cursorArea->boundingRect().height()/2);
    cursorArea->setPos(position);

    scene->addItem(cursorArea);
}

void Game::buildTower(QPointF pos)
{

    if(canBuild(pos) == false){
        return;
    }

    if (money < build->getCost()){
        qDebug() << "Not enough money";
        if (deny_sound->state() == QMediaPlayer::PlayingState){
            deny_sound->setPosition(0);
        } else {
            deny_sound->play();
        }
        return;
    }

    //add build item to scene on keypress, offset to cursor position
    scene->addItem(build);
    build->setPos(pos);

    //tilesystem for tower creation
    build->setX(floor(build->x() / 64) * 64 + 39);
    build->setY(floor(build->y() / 64) * 64 + 39);

    //add built tower to towers list
    towers << build;

    qDebug() << "Towercount: " << towers.size();

    //take correct amount of money
    money -= build->getCost();
    build = new Tower(build->getType());
}

void Game::drawTexts()
{
    qDebug() << "Drawing texts to screen";

    //draw texts to screen
    moneyText = new QGraphicsTextItem();
    moneyText->setPos(64 * 15 + 7, 155);
    moneyText->setFont(font_small);
    moneyText->setDefaultTextColor(Qt::white);
    //texts << moneyText;
    scene->addItem(moneyText);

    livesText = new QGraphicsTextItem();
    livesText->setPos(64 * 15 + 7, 180);
    livesText->setFont(font_small);
    livesText->setDefaultTextColor(Qt::white);
    //texts << livesText;
    scene->addItem(livesText);

    waveWarningText = new QGraphicsTextItem();
    waveWarningText->setPos(64 * 15 + 7, 230);
    waveWarningText->setFont(font_small);
    waveWarningText->setDefaultTextColor(Qt::white);
    //texts << waveWarningText;
    scene->addItem(waveWarningText);

    waveText = new QGraphicsTextItem();
    waveText->setPos(64 * 15 + 7, 205);
    waveText->setFont(font_small);
    waveText->setDefaultTextColor(Qt::white);
    //texts << waveText;
    scene->addItem(waveText);

    scoreText = new QGraphicsTextItem();
    scoreText->setPos(64 * 15 + 70, 520);
    scoreText->setFont(font_small);
    scoreText->setDefaultTextColor(Qt::white);
    //texts << scoreText;
    scene->addItem(scoreText);
}

void Game::displayMenu()
{
    /*if (highscore) {
        highscore->saveHighscore();
    }*/
    onEditor = false;
    scene->clear();

    resetStats();

    //set background image to menu
    background = new QGraphicsPixmapItem();
    background->setPixmap(QPixmap(":/images/bg_menu.png"));
    background->setZValue(-1);
    scene->addItem(background);

    //create a start button
    Button *startButton = new Button("START", 200, 50);
    startButton->setPos(this->width()/2 - startButton->boundingRect().width()/2, 420);
    connect(startButton, SIGNAL(clicked()),this,SLOT(start()));
    scene->addItem(startButton);

    //create a quit button
    Button *exitButton = new Button("EXIT", 200, 50);
    exitButton->setPos(this->width()/2 - exitButton->boundingRect().width()/2, 480);
    connect(exitButton, SIGNAL(clicked()),this,SLOT(close()));
    scene->addItem(exitButton);

}

void Game::displayBuildMenu()
{
    waveButton = new Button("NEXT WAVE", 180, 40);
    waveButton->setPos(scene->width() - 185 - waveButton->boundingRect().width()/2, 550);
    connect(waveButton, SIGNAL(clicked()),this,SLOT(spawnWave()));
    scene->addItem(waveButton);

    //add pause and quit buttons
    pauseButton = new Button("PAUSE", 130, 40);
    pauseButton->setPos(scene->width() - 160 - pauseButton->boundingRect().width()/2, 600);
    connect(pauseButton, SIGNAL(clicked()),this,SLOT(pauseGame()));
    scene->addItem(pauseButton);

    quitButton = new Button("QUIT TO MENU", 180, 40);
    quitButton->setPos(scene->width() - 185 - quitButton->boundingRect().width()/2, 650);
    connect(quitButton, SIGNAL(clicked()),this,SLOT(quitGame()));
    scene->addItem(quitButton);

    ffButton = new Button("1X", 40, 40);
    ffButton->setPos(scene->width() - 255 - ffButton->boundingRect().width()/2, 600);
    connect(ffButton, SIGNAL(clicked()),this,SLOT(fastForward()));
    scene->addItem(ffButton);

    //add tower build icon1
    fire_icon = new BuildTowerIcon(CONSTANTS::TYPE::FIRE);
    fire_icon->setPos(64 * 16 + 7 - 32, 64 * 4 + 7);
    fire_icon->setCostTextPosition();
    scene->addItem(fire_icon);

    //add tower build icon2
    water_icon = new BuildTowerIcon(CONSTANTS::TYPE::WATER);
    water_icon->setPos(64 * 16 + 7 - 32, 64 * 5 + 7);
    water_icon->setCostTextPosition();
    scene->addItem(water_icon);

    //add tower build icon4
    earth_icon = new BuildTowerIcon(CONSTANTS::TYPE::EARTH);
    earth_icon->setPos(64 * 16 + 7 - 32, 64 * 6 + 7);
    earth_icon->setCostTextPosition();
    scene->addItem(earth_icon);

    //add tower build icon4
    wind_icon = new BuildTowerIcon(CONSTANTS::TYPE::WIND);
    wind_icon->setPos(64 * 16 + 7 - 32, 64 * 7 + 7);
    wind_icon->setCostTextPosition();
    scene->addItem(wind_icon);
}

void Game::displayLoseMenu(){

    scene->clear();
    background = new QGraphicsPixmapItem();
    background->setPixmap(QPixmap(":/images/lose_background.png"));
    background->setZValue(-1);
    scene->addItem(background);

    loseText = new QGraphicsTextItem();
    loseText->setPos(this->width() / 2 - 150, 380);
    loseText->setFont(QFont("Lucida Console", 45, QFont::Bold));
    loseText->setDefaultTextColor(Qt::white);
    loseText->setPlainText(QString("YOU LOSE"));
    //texts << loseText;
    scene->addItem(loseText);

    Button *playAgain = new Button("Play Again", 200,50);
    playAgain->setPos(this->width() / 2 - playAgain->boundingRect().width()/2,470);
    connect(playAgain,SIGNAL(clicked()),this,SLOT(start()));
    scene->addItem(playAgain);

    Button *mainMenu = new Button("Main Menu", 200,50);
    mainMenu->setPos(this->width() / 2 - playAgain->boundingRect().width()/2, 530);
    connect(mainMenu,SIGNAL(clicked()),this,SLOT(displayMenu()));
    scene->addItem(mainMenu);

    finalScoreText = new QGraphicsTextItem();
    finalScoreText->setPos(this->width() / 2 - 100, 580);
    finalScoreText->setFont(QFont("Lucida Console", 30, QFont::Bold));
    finalScoreText->setDefaultTextColor(Qt::white);
    finalScoreText->setPlainText(QString("Score: " + QString::number(score)));
    //texts << finalScoreText;
    scene->addItem(finalScoreText);

    playerName = QInputDialog::getText(  this,  "Enter name",
              "Enter your name here",
               QLineEdit::Normal, playerName);

    highscore->addHighscore(playerName, score);

    topFiveText = new QGraphicsTextItem();
    topFiveText->setPos(80, 380);
    topFiveText->setFont(QFont("Lucida Console", 22, QFont::Bold));
    topFiveText->setDefaultTextColor(Qt::white);
    QString temp = "TOP FIVE";
    for(auto i : highscore->topFive()){
        temp += "\n" + i.first + ": " + QString::number(i.second);
    }

    //topFiveText->setPlainText(QString("TOP FIVE\n" + highscore->topFive()[0].first + " " + QString::number(highscore->topFive()[0].second)));
    topFiveText->setPlainText(temp);
    //texts <<topFiveText;
    scene->addItem(topFiveText);

    resetStats();
}

void Game::start()
{
    scene->clear();

    //set background image to menu
    background = new QGraphicsPixmapItem();
    background->setPixmap(QPixmap(":/images/bg_menu.png"));
    background->setZValue(-1);
    scene->addItem(background);

    //create a normal button
    Button *normalButton = new Button("NORMAL", 200, 50);
    normalButton->setPos(this->width()/2 - normalButton->boundingRect().width()/2, 420);
    connect(normalButton, SIGNAL(clicked()),this,SLOT(normal()));
    scene->addItem(normalButton);

    //create a hard button
    Button *hardButton = new Button("HARD", 200, 50);
    hardButton->setPos(this->width()/2 - hardButton->boundingRect().width()/2, 480);
    connect(hardButton, SIGNAL(clicked()),this,SLOT(hard()));
    scene->addItem(hardButton);

    //create a custom button
    Button *customButton = new Button("CUSTOM", 200, 50);
    customButton->setPos(this->width()/2 - customButton->boundingRect().width()/2, 540);
    connect(customButton, SIGNAL(clicked()),this,SLOT(editor()));
    scene->addItem(customButton);

    //create a back button
    Button *backButton = new Button("BACK", 200, 50);
    backButton->setPos(this->width()/2 - backButton->boundingRect().width()/2, 600);
    connect(backButton, SIGNAL(clicked()),this,SLOT(displayMenu()));
    scene->addItem(backButton);
}

void Game::pauseGame() {
    if(paused) {
        updateTimer->start(ffOn ? 5 : 10);
        music->play();
    }
    else {
        updateTimer->stop();
        music->pause();
    }
    pauseButton->setText(paused ? "PAUSE" : "RESUME" );
    paused =! paused;
}

void Game::fastForward(){
    if(ffOn) updateTimer->setInterval(10);
    else updateTimer->setInterval(5);
    ffButton->setText(ffOn ? "1X" : "2X");
    ffButton->setColor(ffOn ? Qt::white : Qt::green);
    music->setPlaybackRate(ffOn ? 1 : 1.2);
    ffOn = !ffOn;
}

void Game::quitGame(bool died) {
    music->setPlaybackRate(1);
    delete updateTimer;
    delete map;
    died ? displayLoseMenu() : displayMenu();
}

void Game::normal()
{
    scene->clear();
    initGame(MAPTYPES::TYPE::NORMAL);
}

void Game::hard()
{
    scene->clear();
    initGame(MAPTYPES::TYPE::HARD);
}

void Game::editor()
{
    onEditor = true;
    scene->clear();
    Button *playButton = new Button("PLAY", 200, 50);
    playButton->setPos(scene->width() - 200 - playButton->boundingRect().width()/2, 540);
    connect(playButton, SIGNAL(clicked()),this,SLOT(mapeditorPlay()));
    scene->addItem(playButton);
    mapeditor = new Mapeditor();
}

void Game::mapeditorPlay()
{
    mapeditor->play();
}

void Game::update()
{
    raycast();
    updateTexts();
    advanceSpawn();
    for (auto& e : enemies){
        e->move_forward();
    }
    if(lives <= 0){
        if(build){
            clearCursor();
        }
        quitGame(true);
    }
}

void Game::advanceSpawn(){
    if(spawnCounter == 100){
        if(enemiesToSpawn > 0) spawnEnemy();
        spawnCounter = 0;
    }
    spawnCounter++;
}

void Game::spawnEnemy()
{
    if(paused) return;
    enemiesToSpawn--;
    //create an enemy
    CONSTANTS::TYPE t;
    if(wave % 4 == 0){
        t = CONSTANTS::TYPE::FIRE;
    }
    else if(wave % 4 == 1){
        t = CONSTANTS::TYPE::WATER;
    }
    else if(wave % 4 == 2){
        t = CONSTANTS::TYPE::EARTH;
    }
    else{
        t = CONSTANTS::TYPE::WIND;
    }
    Enemy *enemy = new Enemy(t);
    if(wave % 5 == 0){
        enemiesToSpawn = 0;
        enemy->setToBoss();
    }
    QList<QPointF> enemy_path = map->getRoadList();
    for (size_t i = 0, n = enemy_path.size(); i < n; i++){
        enemy_path[i].setX(enemy_path[i].x() + 32);
        enemy_path[i].setY(enemy_path[i].y() + 32);
    }
    enemy->setPath(enemy_path);
    enemy->setPos(enemy_path[0]);
    enemies << enemy;
    enemiesAlive++;
    scene->addItem(enemy);
    qDebug() << "Spawned an enemy, enemies now: " << enemies.size();
}

void Game::addOneToSpawn(){
    enemiesToSpawn++;
}

//spawns 10 enemies that are delay apart from each other
void Game::spawnWave()
{
    if(paused) return;

    //int delay = 0;
    //int amount = 10;
    /*for(int i = 0;i<10;i++){
        QTimer::singleShot(delay,this,SLOT(spawnEnemy()));
        delay += 1000;
    }*/
    if(enemiesToSpawn == 0){
        enemiesToSpawn += 10;
        wave++;
    }

}

void Game::initGame(MAPTYPES::TYPE t)
{

    //set background image to menu
    background = new QGraphicsPixmapItem();
    background->setPixmap(QPixmap(":/images/bg_game.png"));
    background->setZValue(-1);
    scene->addItem(background);

    onEditor = false;
    //build background and road
    map = new Map(t);

    updateTimer = new QTimer();
    connect(updateTimer,SIGNAL(timeout()),this,SLOT(update()));
    updateTimer->start(10);

    /*instructionalText = new QGraphicsTextItem();
    instructionalText->setPlainText(QString("FIRE  counters WIND\n\nWATER counters FIRE\n\nEARTH counters WATER\n\nWIND  counters EARTH"));
    instructionalText->setPos(64 * 16 + 7, 64 * 4 + 7);
    instructionalText -> setFont(font_small);
    texts << instructionalText;
    scene->addItem(instructionalText);*/

    //draw texts
    drawTexts();

    //display build menu at start
    displayBuildMenu();
}

void Game::resetStats(){
    lives = 10;
    money = 500;
    score = 0;
    enemiesAlive = 0;
    enemiesToSpawn = 0;
    spawnCounter = 0;
    wave = 0;
    onEditor = false;
    paused = false;
    ffOn = false;
}

void Game::raycast()
{
    //checks a line between every tower and enemy, tower fires if enemy is in attack range and has health
    for(auto &t : towers){
        if (!t){
            qDebug() << "No towers";
            return;
        }
        for(auto &e : enemies){
            if (enemies.isEmpty()) {
                qDebug() << "No enemies in list";
                return;
            }
            int distance = QLineF(t->getCenter(), e->getCenter()).length();
            if (distance <= t->getRange() && !t->isOnCooldown() && e->getHealth() > e->getDamageToTake()) {
                t->setCooldown(true);
                if(ffOn == true){
                    QTimer::singleShot(t->getCooldown()/2,t,SLOT(toggleCoolDown()));
                }
                else{
                    QTimer::singleShot(t->getCooldown(),t,SLOT(toggleCoolDown()));
                }
                t->fire(e->getCenter());
                float x = getMultiplier(t->getType(),e->getType());
                e->takeDamage(t->getDamage() * x, distance);
                //qDebug() << "Without multiplier: " << t->getDamage() << "With Multiplier: " << t->getDamage() * x;
                if (t->getType() == CONSTANTS::TYPE::WATER && !e->isSlow()){
                    e->slowSpeed();
                    QTimer::singleShot(ffOn ? 500 : 1000, e, SLOT(normalSpeed()));
                }
                QTimer::singleShot(distance, this, SLOT(playHit()));
                break;
            }
        }
    }
}

void Game::updateTexts()
{
    moneyText->setPlainText(QString("MONEY: $" + QString::number(money)));
    waveText->setPlainText(QString("WAVE: " + QString::number(wave)));
    livesText->setPlainText(QString("LIVES: " + QString::number(lives)));
    if(lives <= 3) livesText->setDefaultTextColor(Qt::red);
    scoreText->setPlainText(QString("SCORE: " + QString::number(score)));
    getNextWave();
}

bool Game::canBuild(QPointF pos)
{
    //check if on road
    QList<QPointF> road = map->getRoadList();
    for(int i = 0; i < road.size(); ++i){
        if(floor(pos.x() / 64) * 64 + 7== road[i].x()){
            if(floor(pos.y() / 64) * 64 + 7== road[i].y()){
                qDebug() << "Can't build there";
                return false;
            }
        }
    }
    //check if on other tower
    for(int i = 0; i < towers.size(); ++i){
        if(floor(pos.y() / 64) * 64 + 39 == towers[i]->y()){
            if(floor(pos.x() / 64) * 64 + 39 == towers[i]->x()){
                return false;
            }
        }
    }
    //check if too far right
    if (pos.x() >= scene->width()-400){
        return false;
    }
    return true;
}

int Game::getMoney()
{
    return money;
}

void Game::addMoney(int m)
{
    money += m;
}

int Game::getScore()
{
    return score;
}

void Game::addScore(int s)
{
    score += s;
}

int Game::getWave()
{
    return wave;
}

void Game::setWave(int w)
{
    wave = w;
}

void Game::setLives(int l)
{
    lives += l;
}

QFont Game::getFontLarge() const
{
    return font_large;
}

QFont Game::getFontSmall() const
{
    return font_small;
}


void Game::playHit(){
    if (hit->state() == QMediaPlayer::PlayingState){
        hit->setPosition(10);
    } else {
        hit->play();
    }
}

void Game::getNextWave(){
    if((wave + 1) % 4 == 0){
        waveWarningText->setPlainText(QString("NEXT WAVE: FIRE"));
    }
    else if((wave + 1)% 4 == 1){
        waveWarningText->setPlainText(QString("NEXT WAVE: WATER"));
    }
    else if((wave + 1)% 4 == 2){
        waveWarningText->setPlainText(QString("NEXT WAVE: EARTH"));
    }
    else{
        waveWarningText->setPlainText(QString("NEXT WAVE: WIND"));
    }
}

float Game::getMultiplier(CONSTANTS::TYPE t, CONSTANTS::TYPE e){
    switch(t){
        case(CONSTANTS::TYPE::FIRE):
            switch(e){
                case(CONSTANTS::TYPE::FIRE):
                    return 1;
                    break;
                case(CONSTANTS::TYPE::WATER):
                    return 0.5;
                case(CONSTANTS::TYPE::EARTH):
                    return 1;
                case(CONSTANTS::TYPE::WIND):
                    return 2;
            break;
        }
        case(CONSTANTS::TYPE::WATER):
            switch(e){
                case(CONSTANTS::TYPE::FIRE):
                    return 2;
                case(CONSTANTS::TYPE::WATER):
                    return 1;
                case(CONSTANTS::TYPE::EARTH):
                    return 0.5;
                case(CONSTANTS::TYPE::WIND):
                    return 1;
            break;
        }
        case(CONSTANTS::TYPE::EARTH):
            switch(e){
                case(CONSTANTS::TYPE::FIRE):
                    return 1;
                case(CONSTANTS::TYPE::WATER):
                    return 2;
                case(CONSTANTS::TYPE::EARTH):
                    return 1;
                case(CONSTANTS::TYPE::WIND):
                    return 0.5;
            break;
        }
        case(CONSTANTS::TYPE::WIND):
            switch(e){
                case(CONSTANTS::TYPE::FIRE):
                    return 0.5;
                case(CONSTANTS::TYPE::WATER):
                    return 1;
                case(CONSTANTS::TYPE::EARTH):
                    return 2;
                case(CONSTANTS::TYPE::WIND):
                    return 1;
            break;
        }
    }
    return 1;
}
