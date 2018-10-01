#include "tower.h"
#include "bullet.h"
#include "game.h"
#include "enemy.h"
#include "constants.h"

#include <QPixmap>
#include <QVector>
#include <QPointF>
#include <QPolygonF>
#include <QTimer>
#include <QDebug>
#include <qmath.h>
#include <QPainter>
#include <QWidget>
#include <QMediaPlayer>

extern Game *game;

Tower::Tower(CONSTANTS::TYPE t, QGraphicsItem *parent): QObject(), QGraphicsPixmapItem(parent)
{
    //init sounds
    sound = new QMediaPlayer();

    type = t;
    switch(type) {
        case(CONSTANTS::TYPE::FIRE):
            //set graphics
            setPixmap(QPixmap(":/images/tower_fire.png"));

            //set attributes
            cost = CONSTANTS::FIRECOST;
            range = 120;
            firerate = 2;
            damage = 70.0; //32
            break;
        case(CONSTANTS::TYPE::WATER):
            //set graphics
            setPixmap(QPixmap(":/images/tower_water.png"));

            //set attributes
            cost = CONSTANTS::WATERCOST;
            range = 200;
            firerate = 1.5;
            damage = 100.0; //43
            break;
        case(CONSTANTS::TYPE::EARTH):
            //set graphics
            setPixmap(QPixmap(":/images/tower_earth.png"));

            //set attributes
            cost = CONSTANTS::EARTHCOST;
            range = 180;
            firerate = 1;
            damage = 150.0; //64
            break;
        case(CONSTANTS::TYPE::WIND):
            //set graphics
            setPixmap(QPixmap(":/images/tower_wind.png"));

            //set attributes
            cost = CONSTANTS::WINDCOST;
            range = 140;
            firerate = 4;
            damage = 50.0; //16
        break;
    }

    upgrade_cost = cost/2;
    setScale(0.5);
    setOffset(-pixmap().width()/2,-pixmap().height()/2);

    setAcceptHoverEvents(true);

    //initialise range polygon
    initRange();
    initInfo();



    //disable target
    has_target = false;

    focused = false;
    onCooldown = false;
    cooldown = 1000/firerate;
}

Tower::~Tower() {
    game->scene->removeItem(this);
    game->towers.removeOne(this);
    delete sound;
}

void Tower::clear() {
    game->addMoney(cost/2);
    delete this;
}

void Tower::setRange(int newRange)
{
    range = newRange;

    //also update attack_area
    attack_area->setRect(0,0,range*4,range*4);
    attack_area->setPos(-range*2,-range*2);
}

double Tower::distanceTo(QGraphicsItem *item)
{
    QLineF ln(pos(), item->pos());
    return ln.length();
}

void Tower::fire(QPointF dest)
{
    //create and aim a bullet
    Bullet *bullet = new Bullet(type);
    bullet->setPos(getCenter());

    QLineF ln(QPointF(getCenter()), dest);
    int angle = -1 * ln.angle();

    bullet->setRotation(angle);
    double dist = QLineF(getCenter(), dest).length();
    bullet->setRange(dist);
    game->scene->addItem(bullet);

    /*if (sound->state() == QMediaPlayer::PlayingState){
        sound->setPosition(0);
    } else {
        sound->play();
    }*/
}

int Tower::getCost() const
{
    return cost;
}

void Tower::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if (!focused){
        this->setZValue(1); //so info doesn't get hidden under other objects
        attack_area->show();
        info_text->show();
        //qDebug() << x() << ", " << y();

    }
    event->accept();
}

void Tower::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if (!focused){
        attack_area->hide();
        info_text->hide();
        this->setZValue(0);
    }
    event->accept();
}

void Tower::mousePressEvent(QGraphicsSceneMouseEvent *event){
    qDebug() << "Tower clicked!";

    if(event->button() == Qt::LeftButton && game->getMoney() >= upgrade_cost){
        upgrade();
    }
    if(event->button() == Qt::RightButton){
        clear();
    }
}

void Tower::upgrade(){
    game->addMoney(-upgrade_cost);
    cost += upgrade_cost;
    level++;
    upgrade_cost *= 1.2;
    setRange(range+5);
    damage *= 1.3;
    firerate += 0.1;
    cooldown = 1000 / firerate;
    setInfoString();
    info_text->setPlainText(info_string);

}

double Tower::getRange()
{
    return range;
}

QPointF Tower::getCenter()
{
    return pos();
}

double Tower::getDamage()
{
    return damage;
}

bool Tower::isOnCooldown()
{
    return onCooldown;
}

void Tower::setCooldown(bool b)
{
    onCooldown = b;
}

double Tower::getCooldown()
{
    return cooldown;
}

CONSTANTS::TYPE Tower::getType()
{
    return type;
}

void Tower::toggleCoolDown()
{
    onCooldown = false;
}

//Creating tower's attack area
void Tower::initRange()
{
    //draw circle with range
    attack_area = new QGraphicsEllipseItem(this);
    attack_area->setRect(0,0,range*4,range*4);
    attack_area->setBrush(QBrush(Qt::red));
    attack_area->setOpacity(0.3);

    //set circle to middle of the tower and hide it
    QPointF ellipse_center(range*2, range*2);
    QLineF ln(ellipse_center, pos());
    attack_area->setPos(x()+ln.dx(), y()+ln.dy());
    attack_area->setFlag(QGraphicsItem::ItemStacksBehindParent);
    attack_area->hide();
}

void Tower::setInfoString(){
    info_string = QString("\tLv: " + QString::number(level) + "\n\n"
                          "Range: " + QString::number(range) + "\t          +5\n" +
                          "Speed: " + QString::number(firerate) + "\t          +0.1\n" +
                          "Dmg: " + QString::number(damage) + "\t          +30%\n\n"
                          + "            (L) LvUP: -$" + QString::number(upgrade_cost) + "\n"
                          + "            (R) SELL: +$" + QString::number(cost/2));
}

void Tower::initInfo(){
    setInfoString();
    //info_bg = new QGraphicsRectItem(this);
    info_text = new QGraphicsTextItem(this);
    info_text->setPos(x()-250, y()-120);
    info_text->setDefaultTextColor(Qt::yellow);
    info_text->setPlainText(info_string);
    info_text->setScale(2.5);
    info_text->setAcceptHoverEvents(false);
    QFont font(info_text->font());
    font.setWeight(QFont::Bold);
    info_text->setFont(font);
    info_text->hide();
}
