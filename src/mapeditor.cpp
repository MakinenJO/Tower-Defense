#include "mapeditor.h"
#include "game.h"
#include "button.h"

#include <QDebug>

extern Game * game;

Mapeditor::Mapeditor()
{
    font = QFont("Lucida Console", 12, QFont::Bold);
    font.setStyleHint(QFont::Monospace);

    draw();
    qDebug() << "Mapeditor";
    roadEnded = false;
    text = new QGraphicsTextItem();
    text->setPlainText(QString("Draw single road before\nadding branches.\nYou can press PLAY when all\nbranches are finished."));
    text->setPos(64 * 15, 200);
    text -> setFont(font);
    game->scene->addItem(text);
}

Mapeditor::~Mapeditor()
{
    delete tile;
    delete text;
}

void Mapeditor::draw()
{
    game->scene->setBackgroundBrush(Qt::gray);
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

void Mapeditor::makeRoad(QPointF pos)
{
    if(roadEnded){
        if(!newStart(pos)){
            return;
        }
    }
    else if(canBuild(pos)){
        tile = new QGraphicsPixmapItem();
        tile->setPixmap(QPixmap(":/images/bg_road.png"));
        tile->setPos(pos);

        tile->setX(floor(tile->x() / 64) * 64 + 7);
        tile->setY(floor(tile->y() / 64) * 64 + 7);
        game->scene->addItem(tile);

        pos.setX(floor(pos.x() / 64));
        pos.setY(floor(pos.y() / 64));
        road << pos;
    }
    else{
        return;
    }
}

bool Mapeditor::canBuild(QPointF pos)
{
    if(floor(pos.x() / 64) >= 14 || floor(pos.y() / 64) >= 11){
        return false;
    }
    else if(!nearbyPoints(pos)){
        return false;
    }
    else if(road.size() == 0){
        if(floor(pos.x() / 64) == 0 || floor(pos.x() / 64) == 13){
            if(floor(pos.y() / 64) != 0 && floor(pos.y() / 64) != 10){
                roadEnd(pos);
                return true;
            }
        }
        else if(floor(pos.y() / 64) == 0 || floor(pos.y() / 64) == 10){
            if(floor(pos.x() / 64) != 0 && floor(pos.x() / 64) != 13){
                roadEnd(pos);
                return true;
            }
        }
        else{
            return false;
        }
    }
    else if(road.size() > 0){
        if(road.size() == 1){
            if(floor(pos.y() / 64) == 0 || floor(pos.y() / 64) == 10){
                return false;
            }
            else if(floor(pos.x() / 64) == 0 || floor(pos.x() / 64) == 13){
                return false;
            }
        }
        if(floor(pos.x() / 64) == road.last().x()){
            if(floor(pos.y() / 64) == road.last().y() - 1 || floor(pos.y() / 64) == road.last().y() + 1){
                roadEnd(pos);
                return true;
            }
        }
        else if(floor(pos.y() / 64) == road.last().y()){
            if(floor(pos.x() / 64) == road.last().x() - 1 || floor(pos.x() / 64) == road.last().x() + 1){
                roadEnd(pos);
                return true;
            }
        }
        else{
            qDebug() << "road.size() > 0 false" << roadEnded;
            return false;
        }

    }
    return false;
}



void Mapeditor::roadEnd(QPointF pos)
{
    if(road.size() > 0){
        if(floor(pos.x() / 64) == 0 || floor(pos.x() / 64) == 13){
            roadEnded = true;
        }
        else if(floor(pos.y() / 64) == 0 || floor(pos.y() / 64) == 10){
            roadEnded = true;
        }
    }
}

bool Mapeditor::nearbyPoints(QPointF pos)
{
    QList<QPointF> list;
    for(int i = 0; i < road.size(); i++){
        if(floor(pos.x() / 64) == road[i].x()){
            if(floor(pos.y() / 64) == road[i].y() - 1 || floor(pos.y() / 64) == road[i].y() + 1){
                list << road[i];
            }
        }
        else if(floor(pos.y() / 64) == road[i].y()){
            if(floor(pos.x() / 64) == road[i].x() - 1 || floor(pos.x() / 64) == road[i].x() + 1){
                list << road[i];
            }
        }
    }

    if (list.size() > 1){
        if(list[0] == list[1]){
            list.pop_back();
        }
    }
    if(list.size() <= 1){
        return true;
    }
    qDebug() << "nearby == false" << list;
    return false;
}

void Mapeditor::deleteRoad(QPointF pos)
{
    if(road.size() == 0){
        return;
    }
    if(roadEnded){
        roadEnded = false;
    }
    qDebug() << "deleteRoad called";
    QPointF position;
    position.setX(floor(pos.x() / 64));
    position.setY(floor(pos.y() / 64));
    if(position == road.last()){
        road.pop_back();
        position.setX(position.x() * 64 + 7);
        position.setY(position.y() * 64 + 7);
        tile = new QGraphicsPixmapItem();
        tile->setPixmap(QPixmap(":/images/bg_grass.png"));
        tile->setPos(position);
        game->scene->addItem(tile);
        qDebug() << "road" << road;
        if(road.size() == 0){
            roadEnded = false;
        }
    }
}

bool Mapeditor::newStart(QPointF pos)
{
    onCrosroad = false;
    if(floor(pos.x() / 64) == 0 || floor(pos.x() / 64) >= 13){
        qDebug() << "newStart == false";
        return false;
    }
    else if(floor(pos.y() / 64) == 0 || floor(pos.y() / 64) >= 10){
        qDebug() << "newStart == false";
        return false;
    }
    for(int i = 0; i < road.size(); i++){
        for(int a = 0; a < crossroads.size(); a++){
            if(road[i] == crossroads[a]){
                qDebug() << "crossroads" << crossroads;
                onCrosroad = true;
            }
        }
        if(floor(pos.x() / 64) == road[i].x() && onCrosroad == false){
            if(floor(pos.y() / 64) == road[i].y() - 1 || floor(pos.y() / 64) == road[i].y() + 1){
                roadEnded = false;
                crossroads << road[i];
                tile = new QGraphicsPixmapItem();
                tile->setPixmap(QPixmap(":/images/bg_road.png"));
                tile->setPos(pos);

                tile->setX(floor(tile->x() / 64) * 64 + 7);
                tile->setY(floor(tile->y() / 64) * 64 + 7);
                game->scene->addItem(tile);

                pos.setX(floor(pos.x() / 64));
                pos.setY(floor(pos.y() / 64));
                qDebug() << "pos" << pos;
                road << pos;
                return true;
            }
        }
        else if(floor(pos.y() / 64) == road[i].y() && onCrosroad == false){
            if(floor(pos.x() / 64) == road[i].x() - 1 || floor(pos.x() / 64) == road[i].x() + 1){
                roadEnded = false;
                crossroads << road[i];
                tile = new QGraphicsPixmapItem();
                tile->setPixmap(QPixmap(":/images/bg_road.png"));
                tile->setPos(pos);

                tile->setX(floor(tile->x() / 64) * 64 + 7);
                tile->setY(floor(tile->y() / 64) * 64 + 7);
                game->scene->addItem(tile);

                pos.setX(floor(pos.x() / 64));
                pos.setY(floor(pos.y() / 64));
                road << pos;
                return true;
            }
        }
        onCrosroad = false;
    }
    qDebug() << "newStart == false";
    return false;
}
QList<QPointF> Mapeditor::customRoad()
{
    for(int i = 0; i < road.size(); i++){
        road[i].setX(road[i].x() * 64 + 7);
        road[i].setY(road[i].y() * 64 + 7);
    }
    qDebug() << road;
    return road;
}

void Mapeditor::play()
{
    if(!roadEnded){
        qDebug() << "roadEnded  = false";
        return;
    }
    else{
        game->scene->clear();
        game->initGame(MAPTYPES::TYPE::CUSTOM);
    }
}

