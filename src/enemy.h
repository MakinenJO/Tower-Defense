#ifndef ENEMY_H
#define ENEMY_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QList>
#include <QPointF>
#include <QMediaPlayer>

#include "constants.h"

class Enemy: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Enemy(CONSTANTS::TYPE t, QGraphicsPixmapItem *parent=0);
    ~Enemy();
    void rotateToPoint(QPointF p);
    void takeDamage(double d, double t);
    QPointF getCenter();
    double getHealth();
    int getValue();
    void setPath(QList<QPointF> list);
    bool isDead();
    bool isSlow();
    void clear();
    void slowSpeed();
    double getDamageToTake() const;
    void drawHealthBar();
    void updateHealthBar();
    void setToBoss();
    QList<QPointF> nearbyPoints();
    CONSTANTS::TYPE getType();

public slots:
    void move_forward();
    void inflictDamage();
    void normalSpeed();
private:
    CONSTANTS::TYPE type;
    QList<QPointF> points;
    QPointF dest;
    int point_index;
    double speed;
    double health;
    double maxHealth;
    int value;
    bool alive;
    bool slow;
    double damageToTake;
    QList<QPointF> travelled;
    QGraphicsRectItem *red;
    QGraphicsRectItem *green;
};

#endif // ENEMY_H
