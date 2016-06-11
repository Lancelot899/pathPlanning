#ifndef CURRENTMAPVIEW_H
#define CURRENTMAPVIEW_H

#include <memory>


#include <QWidget>

class Map;

class CurrentMapView : public QWidget
{
    Q_OBJECT

public:
    enum {
        MarginWidth = 32,
        MarginHeight = 13
    };

public:
    explicit CurrentMapView(QWidget *parent = 0);

    void paintEvent(QPaintEvent *);

    void setMap(Map *map_);

signals:

public slots:


private:
    Map   *map;
};



#endif // CURRENTMAPVIEW_H
