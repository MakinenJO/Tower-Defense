#ifndef BUTTON_H
#define BUTTON_H

#include <QGraphicsTextItem>
#include <QGraphicsSceneMouseEvent>
#include <QMediaPlayer>

class Button: public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    Button(QString name, int w, int h);
    ~Button();
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void setText(QString t);
    void setColor(QColor c);
signals:
    void clicked();
private:
    QGraphicsTextItem *text;
    QMediaPlayer *hover;
    QMediaPlayer *click;
};

#endif // BUTTON_H
