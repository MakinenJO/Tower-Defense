#ifndef MAPTYPES_H
#define MAPTYPES_H

#include <QObject>

class MAPTYPES: public QObject
{
    Q_ENUMS(TYPE)
public:
    enum TYPE{NORMAL, HARD, CUSTOM};

};

#endif // MAPTYPES_H
