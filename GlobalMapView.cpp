

#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QMessageBox>

#include "GlobalMapView.h"

GlobalMapView::GlobalMapView(QWidget *parent) :
    QWidget(parent)
{
    isDrawMod = true;
    isDraw = false;

    setMenu();
}

void GlobalMapView::setMap(Map *map_)
{
    map = map_;
}

void GlobalMapView::paintEvent(QPaintEvent *)
{
   // qDebug() << this->size();
    QPixmap pixmap(this->size().width() - MarginWidth, this->size().height() - MarginHeight);
    pixmap.fill(Qt::white);
    QPainter p(&pixmap);
    p.setPen(QColor(0, 0, 0));
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
        p.setBrush(Qt::red);
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
        p.setBrush(Qt::green);
        int idx = curPt % map->getMaxWidth();
        int idy = curPt / map->getMaxWidth();
        p.drawRect(idx * bw, idy * bw, bw, bh);
    }



    p.end();
    p.begin(this);
    p.drawPixmap(MarginWidth / 2, MarginHeight / 2, pixmap);
    p.end();
}


void GlobalMapView::mousePressEvent(QMouseEvent *ev)
{
    QPoint pt = this->pos() + ev->pos();
    mousePoint.rx() = pt.rx() - MarginWidth;
    mousePoint.ry() = pt.ry() - MarginHeight;
//    pt.rx() += MarginWidth;
    pt.ry() += MarginHeight;
    if(ev->button() == Qt::RightButton)
    {
        _popMenu->popup(pt);
    }
    else if(ev->button() == Qt::LeftButton)
    {
        if(isDrawMod == true) {
            isDraw = true;
            setMapType(mousePoint, Obstacle);
            update();
        }
    }

    QWidget::mousePressEvent(ev);
}

void GlobalMapView::mouseMoveEvent(QMouseEvent *ev)
{
    if(isDraw)
    {
        QPoint pt = this->pos() + ev->pos();
        pt.rx() -= MarginWidth;
        pt.ry() -= MarginHeight;
        setMapType(pt, Obstacle);
        update();
    }
    QWidget::mouseMoveEvent(ev);
}

void GlobalMapView::mouseReleaseEvent(QMouseEvent *ev)
{
    if(isDraw)
    {
        QPoint pt = this->pos() + ev->pos();
        pt.rx() -= MarginWidth;
        pt.ry() -= MarginHeight;
        setMapType(pt, Obstacle);
        isDraw = false;
        update();
    }

    QWidget::mouseReleaseEvent(ev);
}



void GlobalMapView::actSetBegPoint()
{
    //qDebug() << mousePoint;
    setMapType(mousePoint, Begin);
    setMapType(mousePoint, RobotPos);
    update();
}

void GlobalMapView::actSetEndPoint()
{
    setMapType(mousePoint, End);
    update();
}

void GlobalMapView::actDrawMap()
{
     isDrawMod = !isDrawMod;
}

void GlobalMapView::actClrMap()
{
    if(QMessageBox::Yes ==
            QMessageBox::question(this, "really clear map?", "really clear map?",
            QMessageBox::Yes, QMessageBox::No) ) {
        map->clear();
        update();
    }
}

void GlobalMapView::setMenu()
{
    _AsetBeg = new QAction(tr("set begin point"), this);
    connect(_AsetBeg, SIGNAL(triggered()), this, SLOT(actSetBegPoint()));

    _AsetEnd = new QAction(tr("set end point"), this);
    connect(_AsetEnd, SIGNAL(triggered()), this, SLOT(actSetEndPoint()));

    _AdrawMap = new QAction(tr("draw map model"), this);
    _AdrawMap->setCheckable(true);
    _AdrawMap->setChecked(true);
    connect(_AdrawMap, SIGNAL(triggered()), this, SLOT(actDrawMap()));
    //_AdrawMap->setDisabled(true);

    _AclrMap = new QAction(tr("clear map"), this);
    connect(_AclrMap, SIGNAL(triggered()), this, SLOT(actClrMap()));

    _popMenu = new QMenu("menu");
    _popMenu->addAction(_AsetBeg);
    _popMenu->addAction(_AsetEnd);
    _popMenu->addAction(_AdrawMap);
    _popMenu->addAction(_AclrMap);
}

void GlobalMapView::setMapType(QPoint &point, MapType type)
{
    int bw = (this->size().width() - MarginWidth) / map->getWidth();
    int bh = (this->size().height() - MarginHeight) / map->getHeight();
    //qDebug() << point.rx() / bw << point.ry() / bh;
    map->setVal(point.rx() / bw, point.ry() / bh, int(type));
}


