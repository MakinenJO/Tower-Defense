#ifndef TOWER_H
#define TOWER_H

#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QGraphicsPolygonItem>
#include <QPointF>
#include <QObject>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QMediaPlayer>

#include "constants.h"



class Tower: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Tower(CONSTANTS::TYPE t, QGraphicsItem *parent=0);
    ~Tower();
    void clear();
    void setRange(int newRange);
    double distanceTo(QGraphicsItem *item);
    void fire(QPointF dest);
    int getCost() const;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void upgrade();
    double getRange();
    QPointF getCenter();
    double getDamage();
    bool isOnCooldown();
    void setCooldown(bool b);
    void setInfoString();
    double getCooldown();
    CONSTANTS::TYPE getType();
public slots:
    void toggleCoolDown();
private:
    CONSTANTS::TYPE type;
    double range;
    double firerate;
    double cooldown;
    double damage;
    bool focused;
    bool onCooldown;
    int cost;
    int upgrade_cost;
    int level = 1;
    QGraphicsEllipseItem *attack_area;
    QGraphicsTextItem *info_text;
    QString info_string;
    QPointF attack_dest;
    bool has_target;
    void initRange();
    void initInfo();
    QMediaPlayer *sound;

};

#endif // TOWER_H
