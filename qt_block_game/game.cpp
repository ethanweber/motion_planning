#include "game.h"

#include "gamepiece.h"

#include <QPainter>
#include <QPen>

#include <QKeyEvent>
#include <QPaintEvent>


Game::Game(QWidget *parent) : QWidget(parent)
{
    this->resize(parent->width(), parent->height());

    // so key presses can be used
     setFocusPolicy(Qt::StrongFocus);

    _grid_array = std::vector< std::vector<int> >(25, std::vector<int>(25) );


    // grab a random time seed
//    srand (time(0));

    // set percent black obstacles
    float p_black = 0.1;
    int p_num = (int)(1.0/p_black) - 1;

    // initialize the array to 0 and with a percent that is black (obstacles)
    for (int i = 0; i < _grid_array.size(); i++) {
        for (int j = 0; j < _grid_array[i].size(); j++) {
            if ( (rand() % p_num) == 0) _grid_array[i][j] = 5;
            else _grid_array[i][j] = 0;
        }
    }

    // set the game pieces
    GamePiece temp_piece;
    temp_piece.num = 2;
    temp_piece.target = 4;
    temp_piece.location.setX(1);
    temp_piece.location.setY(0);
    _game_pieces.push_back(temp_piece);

    //    temp_piece = GamePiece();
    //    temp_piece.num = 2;
    //    temp_piece.location.setX(2);
    //    temp_piece.location.setY(0);
    //    _game_pieces.push_back(temp_piece);

    // set a goal
//    _grid_array[_grid_array.size()-1][_grid_array[0].size()-1] = 4;
    _grid_array[5][5] = 4;

    std::cout << _grid_array[0][5] << std::endl;

    // set block size
    _square_size = (int) (width() / _grid_array.size());

    //set the game blocks
    for (int i = 0; i < _game_pieces.size(); i++) {
        int x = _game_pieces[i].location.x();
        int y = _game_pieces[i].location.y();
        _grid_array[x][y] = _game_pieces[i].num;
    }

    update();
}

void Game::keyPressEvent( QKeyEvent* event ) {
    // check for the arrow keys
    int key = event->key();
    switch (key) {
        case Qt::Key_Right:
            std::cout << "Right" << std::endl;
            move(QPoint(1,0));
            break;
        case Qt::Key_Left:
            std::cout << "Left" << std::endl;
            move(QPoint(-1,0));
            break;
        case Qt::Key_Up:
            std::cout << "Up" << std::endl;
            move(QPoint(0,-1));
            break;
        case Qt::Key_Down:
            std::cout << "Down" << std::endl;
            move(QPoint(0,1));
            break;
    }
   update();
}

// move by this amount
void Game::move(QPoint pnt) {
    sort(pnt);
    for (int i = 0; i < _game_pieces.size(); i++) {
        QPoint current_point = _game_pieces[i].location;
        int x = current_point.x();
        int y = current_point.y();
        int x_new = x+pnt.x();
        int y_new = y+pnt.y();

        // check that it doesn't go off the screen
        if (x_new < 0 || x_new >= _grid_array.size()) continue;
        if (y_new < 0 || y_new >= _grid_array[0].size()) continue;

        if (_grid_array[x_new][y_new] == 0 || _grid_array[x_new][y_new] == _game_pieces[i].target) {
            _grid_array[x][y] = 0;
            _grid_array[x_new][y_new] = _game_pieces[i].num;
            // set new location value
            _game_pieces[i].location = QPoint(x_new, y_new);
            std::cout << "Updated location" << std::endl;
        }
    }
}

void Game::sort(QPoint pnt) {
    int x = pnt.x();
    int y = pnt.y();

    std::vector< GamePiece > copy = _game_pieces;
    std::vector< GamePiece > new_pieces;

    // going left
    if (x < 0)
    {
        while (new_pieces.size() < _game_pieces.size()) {
            int index = 0;
            // sort by smallest x value
            int x_val = copy[0].location.x();
            for (int i = 1; i < copy.size(); i++) {
                if (copy[i].location.x() < x_val) {
                    index = i;
                    x_val = copy[i].location.x();
                }
            }
            new_pieces.push_back(copy[index]);
            copy.erase(copy.begin() + index);
        }

    }
    // going right
    else if (x > 0)
    {
        while (new_pieces.size() < _game_pieces.size()) {
            int index = 0;
            // sort by largest x value
            int x_val = copy[0].location.x();
            for (int i = 1; i < copy.size(); i++) {
                if (copy[i].location.x() > x_val) {
                    index = i;
                    x_val = copy[i].location.x();
                }
            }
            new_pieces.push_back(copy[index]);
            copy.erase(copy.begin() + index);
        }
    }
    // going up
    else if (y < 0)
    {
        while (new_pieces.size() < _game_pieces.size()) {
            int index = 0;
            // sort by smallest y value
            int y_val = copy[0].location.y();
            for (int i = 1; i < copy.size(); i++) {
                if (copy[i].location.y() < y_val) {
                    index = i;
                    y_val = copy[i].location.y();
                }
            }
            new_pieces.push_back(copy[index]);
            copy.erase(copy.begin() + index);
        }
    }
    // going down
    else if (y > 0)
    {
        while (new_pieces.size() < _game_pieces.size()) {
            int index = 0;
            // sort by largest y value
            int y_val = copy[0].location.y();
            for (int i = 1; i < copy.size(); i++) {
                if (copy[i].location.y() > y_val) {
                    index = i;
                    y_val = copy[i].location.y();
                }
            }
            new_pieces.push_back(copy[index]);
            copy.erase(copy.begin() + index);
        }
    }
    _game_pieces = new_pieces;
}

void Game::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(QRect(0,0, width(), height()), Qt::red);

    for (int i = 0; i < _grid_array.size(); i++) {
        for (int j = 0; j < _grid_array[i].size(); j++) {
            QColor temp;
            if (_grid_array[i][j] == 0) temp = QColor(255,255,255); // white
            else if (_grid_array[i][j] == 1) temp = QColor(0,255,0); // green
            else if (_grid_array[i][j] == 2) temp = QColor(255,0,0); // red
            else if (_grid_array[i][j] == 3) temp = QColor(0,0,255); // blue
            else if (_grid_array[i][j] == 4) temp = QColor(255,255,0); // yellow
            else if (_grid_array[i][j] == 5) temp = QColor(0,0,0); // black

            painter.fillRect(QRect(i*_square_size,j*_square_size, _square_size, _square_size), temp);

            QPen pen(QColor(0,255,255));
            pen.setWidth(2);
            painter.setPen(pen);
            painter.drawRect(QRect(i*_square_size,j*_square_size, _square_size, _square_size));
        }
    }
}