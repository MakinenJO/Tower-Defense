#ifndef MAP_H
#define MAP_H

#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QFileInfo>
#include "maptypes.h"

class Map: public QGraphicsPixmapItem{
public:

    Map(MAPTYPES::TYPE t, QGraphicsItem * parent=0);
    ~Map();
    // reads two numbers per line and combines them to list of coordinates.
    //returns the list.
    static QList<QPointF> loadRoad(int filenumber);
    void draw();
    void drawRoad();
    QList<QPointF> getRoadList();

private:
    MAPTYPES::TYPE type;
    QGraphicsPixmapItem *tile;
    QList<QPointF> road;
};

#endif // MAP_H
