#ifndef PATHPLANNER_H
#define PATHPLANNER_H

#include "game.h"

class PathPlanner
{
private:

public:

    PathPlanner();
    std::vector< QPoint > get_path(Game *game);
};

#endif // PATHPLANNER_H
