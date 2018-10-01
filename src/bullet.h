#ifndef BULLET_H
#define BULLET_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsItem>

#include "constants.h"

class Bullet: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Bullet(CONSTANTS::TYPE t, QGraphicsItem *parent=0);
    ~Bullet();
    CONSTANTS::TYPE getType();
    void setRange(double newRange);
    void addDistanceTravelled(double dist);
public slots:
    void move();
private:
    double speed;
    double range;
    double distanceTravelled;
    CONSTANTS::TYPE type;
};

#endif // BULLET_H
