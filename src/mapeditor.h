#ifndef MAPEDITOR_H
#define MAPEDITOR_H

#include <QDebug>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>

class Mapeditor: public QGraphicsScene
{
    Q_OBJECT
public:

    Mapeditor();
    ~Mapeditor();
    void draw();
    void makeRoad(QPointF pos);
    bool canBuild(QPointF pos);
    void deleteRoad(QPointF pos);
    bool newStart(QPointF pos);
    void roadEnd(QPointF pos);
    bool nearbyPoints(QPointF pos);
    QList<QPointF> customRoad();
    void play();

    QGraphicsTextItem *text;
    QFont font;

private:
    QGraphicsPixmapItem *tile;
    QList<QPointF> road;
    bool roadEnded;
    QList<QPointF> crossroads;
    bool onCrosroad;
};

#endif // MAPEDITOR_H
