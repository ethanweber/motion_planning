#include "widget.h"
#include "game.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(tr("Block Game"));
    resize(750,750);

    Game *game_page = new Game(this);
}

Widget::~Widget()
{

}
