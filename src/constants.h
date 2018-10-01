#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QObject>

class CONSTANTS: public QObject
{
    Q_ENUMS(TYPE)
public:
    enum TYPE{FIRE, WATER, EARTH, WIND};
    const static int FIRECOST = 180;
    const static int WATERCOST = 250;
    const static int EARTHCOST = 300;
    const static int WINDCOST = 200;
    const static int FIREDMG = 40;
    const static int WATERDMG = 80;
};

#endif // CONSTANTS_H
