#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QTime>
#include <QGraphicsEllipseItem>

#include "tower.h"
#include "highscore.h"
#include "enemy.h"
#include "map.h"
#include "mapeditor.h"
#include "button.h"
#include "buildtowericon.h"

class Game: public QGraphicsView
{
    Q_OBJECT
public:
    //member functions
    Game();
    ~Game();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void closeEvent(QCloseEvent *event);
    void setCursor(QString filename, QPointF position);
    void clearCursor();
    void setCursorArea(double diameter, QPointF position);
    void buildTower(QPointF pos);
    void drawTexts();

    void resetStats();
    void initGame(MAPTYPES::TYPE t);
    void raycast();
    void updateTexts();
    bool canBuild(QPointF pos);
    int getMoney();
    void addMoney(int m);
    int getScore();
    void addScore(int s);
    int getWave();
    void setWave(int w);
    void setLives(int l);
    float getMultiplier(CONSTANTS::TYPE,CONSTANTS::TYPE);
    void advanceSpawn();
    void getNextWave();

    //member attributes
    QGraphicsScene *scene;
    QGraphicsPixmapItem *cursor;
    QGraphicsEllipseItem *cursorArea;
    Highscore *highscore;
    Tower *build;
    QVector<Enemy *> enemies;
    QVector<Tower *> towers;
    //QVector<QGraphicsTextItem *> texts;
    QGraphicsTextItem *moneyText;
    QGraphicsTextItem *waveText;
    QGraphicsTextItem *livesText;
    QGraphicsTextItem *instructionalText;
    QGraphicsTextItem *waveWarningText;
    QGraphicsTextItem *loseText;
    QGraphicsTextItem *scoreText;
    QGraphicsTextItem *topFiveText;
    QGraphicsTextItem *finalScoreText;
    Map *map;
    Mapeditor *mapeditor;
    QFont font_large;
    QFont font_small;
    QTimer *updateTimer;
    QGraphicsPixmapItem *background;

    QFont getFontSmall() const;
    QFont getFontLarge() const;

public slots:
    void start();
    void update();
    void spawnEnemy();
    void spawnWave();
    void editor();
    void normal();
    void hard();
    void displayMenu();
    void displayBuildMenu();
    void pauseGame();
    void fastForward();
    void quitGame(bool died = false);
    void addOneToSpawn();
    void playHit();
    void mapeditorPlay();
    void displayLoseMenu();

private:
    int lives;
    int money;
    int score;
    int enemiesAlive;
    int enemiesToSpawn;
    int spawnCounter;
    int wave;
    bool onEditor;
    bool paused;
    bool ffOn;
    QString playerName;
    Button *pauseButton;
    Button *quitButton;
    Button *ffButton;
    Button *waveButton;
    Button *spawnButton;
    QMediaPlaylist *playlist;
    QMediaPlayer *music;
    QMediaPlayer *hit;
    QMediaPlayer *deny_sound;
    BuildTowerIcon *fire_icon;
    BuildTowerIcon *water_icon;
    BuildTowerIcon *earth_icon;
    BuildTowerIcon *wind_icon;
    Button *normalButton;
};

#endif // GAME_H
