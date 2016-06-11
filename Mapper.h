#ifndef MAPPER_H
#define MAPPER_H

#include <memory.h>
#include <QPoint>

#include <map>

enum MapType {
    Obstacle = 1,
    Begin    = 2,
    End      = 3,
    RobotPos = 4
};


class Map
{
public:
    Map(int width_, int height_)
        : maxWidth(width_), maxHeight(height_)
    {
        width = 0;
        height = 0;
        beginPt = -1;
        endPt = -1;
        currentPt = -1;
    }

    ~Map() {}

public:
    int at(int i, int j)
    {
        if(j * maxWidth + i == beginPt)
            return int(MapType::Begin);
        if(j * maxWidth + i == endPt)
            return int(MapType::End);
        std::map<int, int>::iterator it = data.find(j * maxWidth + i);
        if(it == data.end())
            return 0;
        return it->second;
    }

    int  getWidth() {return width;}
    int  getHeight() { return height;}
    int  getMaxHeight() { return  maxHeight;}
    int  getMaxWidth() { return maxWidth;}
    int  getStartPt() {return beginPt;}
    int  getEndPt() {return endPt;}
    int  getCurPt() {return currentPt;}

    void setCurPt(int x, int y) {currentPt = y * maxWidth + x;}

    const std::map<int, int>* getObstacle() {return &data;}

    void setWidth(int width_) { width = width_ > 0? width_:width;}
    void setHeight(int height_){ height = height_ > 0? height_: height;}

    void setVal(int i, int j, int val) {
        if(val == MapType::Obstacle)
            data.insert(std::pair<int,int>(j * maxWidth + i, val));
        else
        {
            if(val == MapType::Begin)
                beginPt = j * maxWidth + i;
            else if(val == MapType::End)
                endPt = j * maxWidth + i;
            else if(val == MapType::RobotPos)
                currentPt =  j * maxWidth + i;
        }
    }

    void clear()
    {
        data.clear();
        beginPt = -1;
        endPt = -1;
        currentPt = -1;
    }

private:
    std::map<int, int>    data;
    int beginPt;
    int endPt;
    int maxWidth;
    int maxHeight;
    int width;
    int height;
    int currentPt;
};

class Mapper
{
public:
    Mapper(int GlobalWidth, int GlobalHeight, int CurrentWidth, int CurrentHeight);
    ~Mapper();

    Map& getCurMap(int i, int j);
    Map& getGlobalMap() {return globalMap;}
    void setRobot(int x, int y);

    void clearAll();

    int gx() {return global_x;}
    int gy() {return global_y; }

private:
    Map        curMap;
    int        global_x;
    int        global_y;
    Map        globalMap;
};

#endif // MAPPER_H
