#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <QList>
#include <QPair>

class Highscore
{
public:
    Highscore();
    QList<QPair<QString, int>> getHighscore();
    void addHighscore(QString name, int score);
    void saveHighscore();
    void sortHighscore();
    QList<QPair<QString, int>> topFive();
private:
    QList<QPair<QString, int>> highscoreList;
};

#endif // HIGHSCORE_H
