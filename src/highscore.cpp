#include "highscore.h"
#include <QFile>
#include <QDebug>
#include <qmath.h>
#include <QCoreApplication>
#include <QString>
#include <QDir>
#include <bits/stdc++.h>

Highscore::Highscore()
{
    QString filename = QCoreApplication::applicationDirPath() + "/savefile.txt";
    QFile file(filename);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "No file open, returning.";
        return;
    }

    QString string;
    QStringList stringList;
    QPair<QString, int> pair;
    QTextStream textStream(&file);

    while (!textStream.atEnd()){
        string = textStream.readLine();
        if(string == ""){
            continue;
        }
        stringList = string.split(';');
        pair.first = stringList[0];
        pair.second = stringList[1].toInt();
        highscoreList << pair;
    }
    file.close();
    qDebug() << "Loaded highscore: " << highscoreList;
}

QList<QPair<QString, int> > Highscore::getHighscore()
{
    return highscoreList;
}

void Highscore::addHighscore(QString name, int score)
{
    QPair<QString, int> pair;
    pair.first = name;
    pair.second = score;
    highscoreList.push_back(pair);
}

void Highscore::saveHighscore()
{
    //set the filename to qapp's path
    QString filename = QCoreApplication::applicationDirPath() + "/savefile.txt";

    QFile file(filename);

    //write new data to file
    if (file.open(QIODevice::ReadWrite)){
        for (int i = 0; i < highscoreList.length(); i++){
            QTextStream stream(&file);
            stream << highscoreList[i].first << ";" << QString::number(highscoreList[i].second) << endl;
        }
    }
    file.close();
    qDebug() << "Saved score.";
}

bool sortBySec(QPair<QString, int> &a,QPair<QString, int> &b)
{
    return (a.second > b.second);
}

void Highscore::sortHighscore()
{
    std::sort(highscoreList.begin(), highscoreList.end(), sortBySec);
}

QList<QPair<QString, int>> Highscore::topFive()
{
    //first get correct order
    sortHighscore();

    QList<QPair<QString, int>> topFive;

    //append maximum of five items from sorted highscorelist to topFive
    for (int i = 0; i < highscoreList.size(); i++) {
        //don't go over 5 items in the list
        if (i > 4){
            break;
        }
        topFive << highscoreList[i];

    }

    while (topFive.size() < 5) {
        topFive << qMakePair(QString("Erkki Esimerkki"), 0);
    }

    return topFive;
}


