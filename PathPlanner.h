#ifndef PATHPLANNER_H
#define PATHPLANNER_H


#include <QPoint>

#include "AStar.h"

class Mapper;
class Map;


class PathPlanner
{
    enum {
        PlanStep = 3,
        InitMin = 100000000
    };

public:
    PathPlanner(int GlobalWidth, int GlobalHeight, int CurrentWidth, int CurrentHeight);
    ~PathPlanner();

public:
    Map& getCurMap(int i, int j);
    Map& getGlobalMap();

    bool getRobotPos(QPoint &pt);

    bool running();
    void stop();
    bool isRunning() {return isRunning_; }
    void clearAll();

private:
    bool planning();

private:
    Mapper       *mapper;
    Map          *map;
    AStarBase    *plan;

    bool         isRunning_;
    int          robotStep;          //! planning per 3 step
    int          steps[PlanStep];
};

#endif // PATHPLANNER_H


