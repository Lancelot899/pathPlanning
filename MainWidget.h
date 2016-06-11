#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTimer>

class GlobalMapView;
class CurrentMapView;
class PathPlanner;

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    enum {
        CurMapWidth      =  64,
        CurMapHeight     =  48,
        GlobalMapWidth   =  175,
        GlobalMapHeight  =  235
    };

    enum {
        WinWidth = 1500,
        WinHeight = 1030
    };

public:
    explicit MainWidget(QWidget *parent = 0);

signals:

public slots:
    void actSimuBegin();
    void actSimuStop();
    void actSimuPause();
    void tmRunning();

private:
    GlobalMapView           *globalMapView;
    CurrentMapView          *currentMapView;
    QLabel                  *agentXLabel;
    QLabel                  *agentYLabel;
    QLineEdit               *agentX;
    QLineEdit               *agentY;
    QTimer                  timer;

private:
    QPushButton             *beginButton;
    QPushButton             *stopButton;
    QPushButton             *pauseButton;

private:
    PathPlanner            *pathPlanner;
};



#endif // MAINWIDGET_H
