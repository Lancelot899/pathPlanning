

#include <QPainter>
#include <QPixmap>
#include <QDebug>

#include "CurrentMapView.h"
#include "Mapper.h"


CurrentMapView::CurrentMapView(QWidget *parent) :
    QWidget(parent)
{
}


void CurrentMapView::paintEvent(QPaintEvent *)
{
    //qDebug() << this->size();
    QPixmap pixmap(this->size().width() - MarginWidth, this->size().height() - MarginHeight);
    pixmap.fill(Qt::white);
    QPainter p(&pixmap);
    p.setPen(QColor(0, 0, 0));
    //qDebug();
    int bw = pixmap.width() / map->getWidth();
    int bh = pixmap.height() / map->getHeight();
    for(int j = 0; j < map->getHeight(); ++j)
        p.drawLine(QPoint(0, j * bh), QPoint(map->getWidth() * bw, j * bh));
    for(int i = 0; i < map->getWidth(); ++i)
        p.drawLine(QPoint(i * bw, 0), QPoint(i * bw, map->getHeight() * bh));

    for(std::map<int, int>::const_iterator it = map->getObstacle()->begin(); it != map->getObstacle()->end(); ++it)
    {
        //qDebug() << it->second;
        if(it->second == MapType::Obstacle)
            p.setBrush(Qt::black);
        int idx = it->first % map->getMaxWidth();
        int idy = it->first / map->getMaxWidth();
//        qDebug() << "GlobalMapView::paintEvent: idx, idy, bw, bh" <<
//                    idx << idy << bw << bh;
        //qDebug() << idx << idy << idx + bw << idy + bh;
        p.drawRect(idx * bw, idy * bw, bw, bh);
    }

    int stPt = map->getStartPt();
    int stEd = map->getEndPt();
    int curPt = map->getCurPt();
    if(stPt != -1)
    {
        p.setBrush(Qt::green);
        int idx = stPt % map->getMaxWidth();
        int idy = stPt / map->getMaxWidth();
        p.drawRect(idx * bw, idy * bw, bw, bh);
    }

    if(stEd != -1)
    {
        p.setBrush(Qt::blue);
        int idx = stEd % map->getMaxWidth();
        int idy = stEd / map->getMaxWidth();
        p.drawRect(idx * bw, idy * bw, bw, bh);
    }

    if(curPt != -1)
    {
        p.setBrush(Qt::red);
        int idx = curPt % map->getMaxWidth();
        int idy = curPt / map->getMaxWidth();
        p.drawRect(idx * bw, idy * bw, bw, bh);
    }

    p.end();

    p.begin(this);
    p.drawPixmap(MarginWidth / 2, MarginHeight / 2, pixmap);
    p.end();
}

void CurrentMapView::setMap(Map *map_)
{
    map = map_;
}
