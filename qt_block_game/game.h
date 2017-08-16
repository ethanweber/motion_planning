#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <iostream>

// include gamepiece
#include "gamepiece.h"

class Game : public QWidget
{
    Q_OBJECT
public:
    explicit Game(QWidget *parent = nullptr);

private:
    std::vector< std::vector<int> > _grid_array;
    int _square_size;
    std::vector< GamePiece > _game_pieces;
    void move(QPoint pnt);
    void sort(QPoint pnt);
    QPoint moves[4] = { QPoint(1,0), QPoint(0,1), QPoint(-1,0), QPoint(0,-1) };

    void solve_puzzle();
    std::vector< QPoint > get_path();
    bool exists(int i, int j, std::map<int, std::map<int, QPoint> > hash_map);

    struct Cell
    {
        Cell *parent = NULL;
        QPoint position;
        std::vector< QPoint > total_visited;
        std::vector< QPoint > children;
    };

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent( QKeyEvent* event ) override;

signals:

public slots:
};

#endif // GAME_H
