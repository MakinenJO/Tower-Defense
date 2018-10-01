#include "bullet.h"
#include "game.h"

#include <QPixmap>
#include <QTimer>
#include <qmath.h>
#include <QDebug>

extern Game *game;

Bullet::Bullet(CONSTANTS::TYPE t, QGraphicsItem *parent): QGraphicsPixmapItem(parent)
{
    //set bullet type
    type = t;
    //set graphic offset
    switch(type){
        case(CONSTANTS::TYPE::FIRE):
            setPixmap(QPixmap(":/images/bullet_fire.png"));
            break;
        case(CONSTANTS::TYPE::WATER):
            setPixmap(QPixmap(":/images/bullet_water.png"));
            break;
        case(CONSTANTS::TYPE::EARTH):
            setPixmap(QPixmap(":/images/bullet_earth.png"));
            break;
        case(CONSTANTS::TYPE::WIND):
            setPixmap(QPixmap(":/images/bullet_wind.png"));
            break;
    }
    setOffset(-32,-32);

    //set attributes
    speed = 10;
    distanceTravelled = 0;

    //connect timer to move()
    connect(game->updateTimer,SIGNAL(timeout()),this,SLOT(move()));
}

Bullet::~Bullet() {}

CONSTANTS::TYPE Bullet::getType()
{
    return type;
}

void Bullet::setRange(double newRange)
{
    range = newRange;
}

void Bullet::addDistanceTravelled(double dist)
{
    distanceTravelled += dist;
}

void Bullet::move()
{
    //define bullets movement
    double theta = qDegreesToRadians(rotation()); //degrees converted for qSin and qCos functions
    double dx = speed * qCos(theta);
    double dy = speed * qSin(theta);

    //move bullet
    setPos(x()+dx, y()+dy);

    //add to distance travelled
    double dist = qSqrt(qPow(dx,2)+qPow(dy,2));
    addDistanceTravelled(dist);

    //delete if traveled more than max range
    if (distanceTravelled >= range) {
        game->scene->removeItem(this);
        delete this;
        return;
    }

    //delete if out of bounds
    if (x() + this->pixmap().width() < 0 || x() > game->scene->width()
            || y() > game->scene->height() || y() + this->pixmap().height() < 0) {
        game->scene->removeItem(this);
        delete this;
        return;
    }

}
