#include "game.h"
#include <QApplication>
#include <QIcon>
#include <QDebug>

Game *game;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //set icon
    a.setWindowIcon(QIcon(":/images/icon.ico"));

    //set title
    a.setApplicationDisplayName("Wrath of the Elements TD");

    game = new Game();
    game->show();
    game->displayMenu();

    return a.exec();
}
