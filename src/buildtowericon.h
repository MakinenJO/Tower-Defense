#ifndef BUILDTOWERICON_H
#define BUILDTOWERICON_H

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>
#include <QMediaPlayer>

#include "constants.h"

class BuildTowerIcon: public QGraphicsPixmapItem{
public:
    BuildTowerIcon(CONSTANTS::TYPE t, QGraphicsItem * parent=0);
    ~BuildTowerIcon();
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void setCostTextPosition();
private:
    CONSTANTS::TYPE type;
    QGraphicsTextItem *costText;
    QGraphicsTextItem *typeText;
    QGraphicsTextItem *counterText;
    QMediaPlayer *click_sound;
};

#endif // BUILDTOWERICON_H
