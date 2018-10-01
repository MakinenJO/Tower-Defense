#include "enemy.h"
#include "bullet.h"
#include "game.h"
#include "tower.h"

#include <QPixmap>
#include <QTimer>
#include <qmath.h>
#include <QDebug>
#include <QGraphicsRectItem>

extern Game *game;

Enemy::Enemy(CONSTANTS::TYPE t, QGraphicsPixmapItem *parent): QObject(), QGraphicsPixmapItem(parent)
{
    //set graphics



    //draw healthbar
    drawHealthBar();

    //set attributes
    type = t;
    switch(type){
        case(CONSTANTS::TYPE::FIRE):
            setPixmap(QPixmap(":/images/enemy_fire.png"));
            speed = 0.3;
            health = 200 + 100*game->getWave();
            break;
        case(CONSTANTS::TYPE::WATER):
            setPixmap(QPixmap(":/images/enemy_water.png"));
            speed = 0.2;
            health = 300 + 125*game->getWave();
            break;
        case(CONSTANTS::TYPE::EARTH):
            setPixmap(QPixmap(":/images/enemy_earth.png"));
            speed = 0.15;
            health = 400 + 175*game->getWave();
            break;
        case(CONSTANTS::TYPE::WIND):
            setPixmap(QPixmap(":/images/enemy_wind.png"));
            speed = 0.35;
            health = 100 + 75*game->getWave();
            break;

    }

    health *= 1 + 0.01 * qPow(game->getWave(), 2);

    setOffset(-64,-64);
    setTransformOriginPoint(0,0);
    setScale(0.4);

    maxHealth = health;
    value = 20 + 2 * game->getWave();
    alive = true;
    slow = false;
    damageToTake = 0;

    /*/connect timer to move_forward
    QTimer *timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(move_forward()));
    timer->start(5);
    */
}

Enemy::~Enemy(){
    game->scene->removeItem(this);
    game->enemies.removeOne(this);
}

void Enemy::clear()
{
    game->scene->removeItem(red);
    game->scene->removeItem(green);
    delete this;
}

void Enemy::rotateToPoint(QPointF p)
{
    QLineF ln(pos(),p);
    setRotation(-1 * ln.angle());
}

void Enemy::takeDamage(double d, double t)
{
    if (alive) {
        damageToTake += d;
    }

    QTimer::singleShot(t, this, SLOT(inflictDamage()));
}

CONSTANTS::TYPE Enemy::getType()
{
    return type;
}

QPointF Enemy::getCenter()
{
    return QPointF(x(), y());
}

double Enemy::getHealth()
{
    return health;
}

int Enemy::getValue()
{
    return value;
}

void Enemy::setPath(QList<QPointF> list)
{
    points = list;
    point_index = 0;
    dest = points[0];
    travelled << dest;
    rotateToPoint(dest);
}

bool Enemy::isDead()
{
    return alive;
}

bool Enemy::isSlow(){
    return slow;
}

void Enemy::slowSpeed()
{
    slow = true;
    speed -= 0.05;

}

void Enemy::normalSpeed(){
    slow = false;
    speed += 0.05;
}

void Enemy::move_forward()
{

    if (health <= 0){
        alive = false;
    }

    if (!alive) {
        qDebug() << "This enemy died";
        game->addMoney(value);
        game->addScore(value);
        clear();
        return;
    }

    //if close to dest, rotate to next dest
    QLineF ln(pos(), dest);
    if (ln.length() < 1){
            if(x() >= game->scene->width() - 432) {
                game->setLives(-1);
                clear();
                return;
            }
            else if(y() <= 40 && floor(x() / 64) != floor(points[0].x() / 64)){
                game->setLives(-1);
                clear();
                return;
            }
            else if(x() <= 40 && floor(y() / 64) != floor(points[0].y() / 64)){
                game->setLives(-1);
                clear();
                return;
            }
            else if(y() >= game->scene->height() - 42 || y() <= 0){
                game->setLives(-1);
                clear();
                return;
            }
        QList<QPointF> nearby = nearbyPoints();
        int a = 0;
        if(nearby.size() == 2){
            a = rand()% 2;
        }
        dest = nearby[a];
        travelled << dest;
        rotateToPoint(dest);
    }


    //define enemy's movement "vector"
    double theta = qDegreesToRadians(rotation()); //degrees converted for qSin and qCos functions
    double dx = speed * qCos(theta);
    double dy = speed * qSin(theta);

    //move enemy
    setPos(x()+dx, y()+dy);

    updateHealthBar();
}

QList<QPointF> Enemy::nearbyPoints()
{

    QList<QPointF> list;
    for(int i = 0; i < points.size(); i++){
        if(floor(pos().x() / 64) * 64 + 7== points[i].x() - 32){
            if(floor(pos().y() / 64) * 64 + 7== points[i].y() - 96 || floor(pos().y() / 64) * 64 + 7== points[i].y() + 32){
                    list << points[i];
            }
        }
        if(floor(pos().y() / 64) * 64 + 7== points[i].y() - 32){
            if(floor(pos().x() / 64) * 64 + 7== points[i].x() - 96 || floor(pos().x() / 64) * 64 + 7== points[i].x() + 32){
                    list << points[i];
            }
        }
    }
    if(travelled.size() > 0){
        int b = 0;
        for(int a = 0; a < list.size(); a++){
            for(; b < travelled.size(); b++){
                if(list[a] == travelled[b]){
                    list.removeAt(a);
                }
            }
        }
    }
    if(list.size() > 2){
        qDebug() << "list" << list;
    }

    return list;
}

void Enemy::inflictDamage()
{
    health -= damageToTake;

    //qDebug() << "Enemy took " << damageToTake << " damage, remaining health " <<  health;
    damageToTake = 0;
}

double Enemy::getDamageToTake() const
{
    return damageToTake;
}

void Enemy::drawHealthBar()
{
    red = new QGraphicsRectItem();
    red->setBrush(QBrush(Qt::red));
    game->scene->addItem(red);

    green = new QGraphicsRectItem();
    green->setBrush(QBrush(Qt::green));
    game->scene->addItem(green);
}

void Enemy::updateHealthBar()
{
    red->setRect(this->x() - 16, this->y() - 24, 32, 5);
    green->setRect(this->x() - 16, this->y() - 24, 32 * health/maxHealth, 5);
}

void Enemy::setToBoss(){
    value *= 10;
    health *= 14;
    maxHealth = health;
    setScale(1);
    speed = 0.08;
}
