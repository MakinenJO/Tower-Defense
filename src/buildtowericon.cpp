#include "buildtowericon.h"
#include "game.h"
#include "tower.h"
#include "constants.h"

#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QDebug>

extern Game * game;

BuildTowerIcon::BuildTowerIcon(CONSTANTS::TYPE t, QGraphicsItem *parent): QGraphicsPixmapItem(parent){
    costText = new QGraphicsTextItem();
    costText->setFont(game->getFontLarge());
    costText->setDefaultTextColor(Qt::white);
    game->scene->addItem(costText);

    typeText = new QGraphicsTextItem();
    typeText->setFont(game->getFontLarge());
    typeText->setDefaultTextColor(Qt::white);
    game->scene->addItem(typeText);

    counterText = new QGraphicsTextItem();
    counterText->setFont(game->getFontSmall());
    counterText->setDefaultTextColor(Qt::white);
    game->scene->addItem(counterText);

    //init click sound and deny build sound
    click_sound = new QMediaPlayer();
    click_sound->setMedia(QUrl("qrc:/sounds/button_click.mp3"));
    click_sound->setVolume(45);


    type = t;
    switch(type){
        case(CONSTANTS::TYPE::FIRE):
            setPixmap(QPixmap(":/images/tower_fire.png"));
            costText->setPlainText("$" + QString::number(CONSTANTS::FIRECOST));
            typeText->setPlainText("FIRE");
            counterText->setPlainText("counters WIND");
            break;
        case(CONSTANTS::TYPE::WATER):
            setPixmap(QPixmap(":/images/tower_water.png"));
            costText->setPlainText("$" + QString::number(CONSTANTS::WATERCOST));
            typeText->setPlainText("WATER");
            counterText->setPlainText("counters FIRE");
            break;
        case(CONSTANTS::TYPE::EARTH):
            setPixmap(QPixmap(":/images/tower_earth.png"));
            costText->setPlainText("$" + QString::number(CONSTANTS::EARTHCOST));
            typeText->setPlainText("EARTH");
            counterText->setPlainText("counters WATER");
            break;
        case(CONSTANTS::TYPE::WIND):
            setPixmap(QPixmap(":/images/tower_wind.png"));
            costText->setPlainText("$" + QString::number(CONSTANTS::WINDCOST));
            typeText->setPlainText("WIND");
            counterText->setPlainText("counters EARTH");
            break;
    }
    setScale(0.5);
}

BuildTowerIcon::~BuildTowerIcon(){
    game->scene->removeItem(this);
    delete click_sound;
}

void BuildTowerIcon::mousePressEvent(QGraphicsSceneMouseEvent *event){
    if (!game->build){

        game->build = new Tower(type);

        QString pixmap_string;

        switch(type){
            case(CONSTANTS::TYPE::FIRE):
                pixmap_string = QString(":/images/tower_fire.png");
                break;
            case(CONSTANTS::TYPE::WATER):
                pixmap_string = QString(":/images/tower_water.png");
                break;
            case(CONSTANTS::TYPE::EARTH):
                pixmap_string = QString(":/images/tower_earth.png");
                break;
            case(CONSTANTS::TYPE::WIND):
                pixmap_string = QString(":/images/tower_wind.png");
                break;

        }
        game->setCursor(pixmap_string, event->scenePos());

        click_sound->play();
    }
}

void BuildTowerIcon::setCostTextPosition()
{
    costText->setPos(this->x() - 76, this->y() + 16);
    typeText->setPos(this->x() + 64, this->y() + 6);
    counterText->setPos(this->x() + 64, this->y() + 34);
}
