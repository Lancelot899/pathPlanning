#ifndef GLOBALMAPVIEW_H
#define GLOBALMAPVIEW_H

#include <QWidget>
#include <QMenu>

#include "Mapper.h"

class Map;

class GlobalMapView : public QWidget
{
    Q_OBJECT

public:
    enum {
        MarginWidth = 36,
        MarginHeight = 35
    };

public:
    explicit GlobalMapView(QWidget *parent = 0);

    void setMap(Map *map_);

public:
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    QPoint& getRobotPos() { return robotPos;}


protected:
    void paintEvent(QPaintEvent *);

signals:

public slots:
    void actSetBegPoint();
    void actSetEndPoint();
    void actDrawMap();
    void actClrMap();

private:
    void setMenu();
    void setMapType(QPoint &point, MapType);

private:
    //! mouse right click menu
    QMenu                       *_popMenu;
    QAction                     *_AsetBeg;
    QAction                     *_AsetEnd;
    QAction                     *_AdrawMap;
    QAction                     *_AclrMap;


    //! draw helper
    QPoint                      mousePoint;
    bool                        isDrawMod;
    bool                        isDraw;


private:
    Map            *map;
    QPoint         robotPos;
};

#endif // GLOBALMAPVIEW_H
