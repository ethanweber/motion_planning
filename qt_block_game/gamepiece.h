#ifndef GAMEPIECE_H
#define GAMEPIECE_H

#include <QPoint>
#include <QColor>

#include <iostream>

class GamePiece
{
public:
    GamePiece();
public:
    QPoint location;
    int num;
    int target;
    void print() {
        std::cout << "Loc (x, y): " << location.x() << ", " << location.y() << "; Num: " << num << std::endl;
    }
};

#endif // GAMEPIECE_H
