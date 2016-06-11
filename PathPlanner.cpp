#include <QDebug>

#include "Mapper.h"
#include "PathPlanner.h"


PathPlanner::PathPlanner(int GlobalWidth, int GlobalHeight, int CurrentWidth, int CurrentHeight)
{
    plan = new AStarBase;
    map = nullptr;
    mapper = new Mapper(GlobalWidth, GlobalHeight, CurrentWidth, CurrentHeight);
    isRunning_ = false;
    robotStep = 0;
}

PathPlanner::~PathPlanner()
{
    delete mapper;
}

Map &PathPlanner::getCurMap(int i, int j)
{
    return mapper->getCurMap(i, j);
}

Map& PathPlanner::getGlobalMap()
{
    return mapper->getGlobalMap();
}

bool PathPlanner::getRobotPos(QPoint &pt) {
    int robot = getGlobalMap().getCurPt();
    if(robot == -1)
        return false;
    pt.rx() = robot % mapper->getGlobalMap().getMaxWidth();
    pt.ry() = robot / mapper->getGlobalMap().getMaxWidth();
    return true;
}

bool PathPlanner::running()
{
    if(isRunning() == false)
    {
        isRunning_ = true;
    }
    Map& globalMap = mapper->getGlobalMap();
    int robot = globalMap.getCurPt();
    int x = robot % globalMap.getMaxWidth();
    int y = robot / globalMap.getMaxWidth();

    Map& curMap = mapper->getCurMap(x, y);
    int viewWidth = curMap.getMaxWidth();
    int viewHeight = curMap.getMaxHeight();
    int xStart = x - viewWidth / 2 > 0? x - viewWidth / 2 : 0;
    int yStart = y - viewHeight / 2 > 0? y - viewHeight / 2 : 0;
    int cx = x - xStart;
    int cy = y - yStart;

    curMap.setVal(cx, cy, MapType::Begin);
    int g_Ed = globalMap.getEndPt();
    int g_x = g_Ed % globalMap.getMaxWidth();
    int g_y = g_Ed / globalMap.getMaxWidth();
    g_x = g_x < x + curMap.getWidth() / 2 ? g_x : x + curMap.getWidth() / 2 - 1;
    g_y = g_y < y + curMap.getHeight() / 2 ? g_y : y + curMap.getHeight() / 2 - 1;
    cx = g_x - xStart < 0? 0: g_x - xStart;
    cy = g_y - yStart< 0? 0: g_y - yStart;
    curMap.setVal(cx, cy, MapType::End);

    if(curMap.getEndPt() != -1 && curMap.getStartPt() != -1)
        map = &curMap;
    else
        map = nullptr;
    plan->Create(map);

    PAPoint pPoint = plan->CalcNextPoint(nullptr);
    if(pPoint == nullptr)
    {
        qDebug() << "no path";
        return false;
    }

    Map& g_map = mapper->getGlobalMap();

    map->setCurPt(pPoint[0].x, pPoint[0].y);

    int lx = map->getStartPt() % map->getMaxWidth();
    int ly = map->getStartPt() / map->getMaxWidth();

    int dx = pPoint[0].x - lx;
    int dy = pPoint[0].y - ly;

    g_map.setCurPt(mapper->gx() + dx, mapper->gy() + dy);

    return true;
}

void PathPlanner::stop()
{
    isRunning_ = false;
}

void PathPlanner::clearAll()
{
    mapper->clearAll();
}

bool PathPlanner::planning()
{
    if(map == nullptr)
    {
        qDebug() << "not initial or no begin point or end, error!";
        return false;
    }

}

