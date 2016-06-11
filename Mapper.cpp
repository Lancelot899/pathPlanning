
#include <QDebug>

#include "Mapper.h"


Mapper::Mapper(int GlobalWidth, int GlobalHeight, int CurrentWidth, int CurrentHeight)
    : curMap(CurrentWidth, CurrentHeight), globalMap(GlobalWidth, GlobalHeight) {
    globalMap.setWidth(GlobalWidth);
    globalMap.setHeight(GlobalHeight);
    global_x = global_y = 0;
}

Mapper::~Mapper()
{

}

Map &Mapper::getCurMap(int i, int j)
{
    curMap.clear();
    global_x = i;
    global_y = j;
    int viewWidth = curMap.getMaxWidth();
    int viewHeight = curMap.getMaxHeight();
    int width = 0;
    int height = 0;
    int idx, idy;
    int xStart = i - viewWidth / 2 > 0? i - viewWidth / 2 : 0;
    int yStart = j - viewHeight / 2 > 0? j - viewHeight / 2 : 0;
//    qDebug() << "Mapper::getCurMap start" << xStart << yStart;
//    qDebug() << globalMap.getHeight();
    for(idx = xStart;
        idx < i + viewWidth / 2 && idx < globalMap.getWidth(); ++idx, ++width)
    {
//        qDebug() << "Mapper::getCurMap mapwidth " << width;
//        qDebug() << "Mapper::getCurMap idx idy " << idx << idy;
        for(idy = yStart;
            idy < j + viewHeight / 2 && idy < globalMap.getHeight(); ++idy)
        {
            int data = globalMap.at(idx, idy);
            if(data)
                curMap.setVal(idx - xStart, idy - yStart, data);
            if(width == 0) ++height;
        }
    }
//    qDebug() << "Mapper::getCurMap" <<width << height;
    curMap.setWidth(width);
    curMap.setHeight(height);
    return curMap;
}

void Mapper::setRobot(int x, int y)
{
    globalMap.setCurPt(x, y);
    curMap.setCurPt(x, y);
}

void Mapper::clearAll()
{
    curMap.clear();
    globalMap.clear();
}
