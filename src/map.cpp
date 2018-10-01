#include "map.h"
#include "game.h"

#include <QDebug>

extern Game * game;

Map::Map(MAPTYPES::TYPE t, QGraphicsItem *parent): QGraphicsPixmapItem(parent)
{
    int filenumber;
    type = t;
    switch (type) {
    case(MAPTYPES::TYPE::NORMAL):
        filenumber = 0;
        road = loadRoad(filenumber);
        break;
    case(MAPTYPES::TYPE::HARD):
        filenumber = 1;
        road = loadRoad(filenumber);
        break;
    case(MAPTYPES::TYPE::CUSTOM):
        road = game->mapeditor->customRoad();
        break;
    default:
        break;
    }
    draw();
    drawRoad();
}

Map::~Map()
{
    delete tile;
}

QList<QPointF> Map::loadRoad(int filenumber)
{
    QList<QPointF> list;
    if(filenumber == 1){
        QFile file(":/files/hard.txt");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return list;

        while (!file.atEnd()) {
            QString line = file.readLine();
            QStringList text = line.split(',');
            int a = text[0].toInt();
            int b = text[1].toInt();
            list << QPointF(a * 64 + 7,b * 64 + 7);
        }
    }
    else{
        QFile file(":/files/normal.txt");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return list;

        while (!file.atEnd()) {
            QString line = file.readLine();
            QStringList text = line.split(',');
            int a = text[0].toInt();
            int b = text[1].toInt();
            list << QPointF(a * 64 + 7,b * 64 + 7);
        }
    }

    return list;
}

void Map::draw()
{
    int counter = 0;
    int b = 7;
    for(int a = 7; a < game->scene->width() - 400; a = a + 64){
        tile = new QGraphicsPixmapItem();
        tile->setPixmap(QPixmap(":/images/bg_grass.png"));
        tile->setPos(a,b);
        game->scene->addItem(tile);
        for(; b < game->scene->height() - 32; b = b + 64){
            tile = new QGraphicsPixmapItem();
            tile->setPixmap(QPixmap(":/images/bg_grass.png"));
            tile->setPos(a,b);
            game->scene->addItem(tile);
            counter++;
        }
        b = 7;
        counter++;
    }
    qDebug() << "Created " << counter << " tiles";
}

void Map::drawRoad()
{
    for(int i = 0; i < road.size(); i++){
        tile = new QGraphicsPixmapItem();
        tile->setPixmap(QPixmap(":/images/bg_road.png"));
        tile->setPos(road[i]);
        game->scene->addItem(tile);
    }

}

QList<QPointF> Map::getRoadList()
{
    return road;
}
