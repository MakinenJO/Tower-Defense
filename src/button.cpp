#include "button.h"
#include "game.h"

#include <QBrush>

extern Game *game;

Button::Button(QString name, int w, int h)
{
    //draw the button to screen
    setRect(0,0,w,h);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::black);
    setBrush(brush);

    //set text

    text = new QGraphicsTextItem(name,this);
    text->setFont(game->getFontSmall());
    int xPos = rect().width()/2 - text->boundingRect().width()/2;
    int yPos = rect().height()/2 - text->boundingRect().height()/2;
    text->setPos(xPos,yPos);
    text->setDefaultTextColor(Qt::white);

    setAcceptHoverEvents(true);

    //set sound
    hover = new QMediaPlayer();
    hover->setMedia(QUrl("qrc:/sounds/button_hover.mp3"));
    hover->setVolume(8);

    click = new QMediaPlayer();
    click->setMedia(QUrl("qrc:/sounds/button_click.mp3"));
    click->setVolume(45);
}

Button::~Button(){
    delete text;
    delete hover;
    delete click;
}

void Button::setText(QString t){
    text->setPlainText(t);
}

void Button::setColor(QColor c) {
    text->setDefaultTextColor(c);
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    click->play();
    emit clicked();

    event->accept();
}

void Button::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::darkGray);
    setBrush(brush);

    if (hover->state() == QMediaPlayer::PlayingState){
        hover->setPosition(0);
    } else {
        hover->play();
    }

    event->accept();
}

void Button::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::black);
    setBrush(brush);

    event->accept();
}
