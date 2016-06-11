
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QDebug>
#include <QString>
#include <QMouseEvent>
#include <QMessageBox>



#include "MainWidget.h"
#include "PathPlanner.h"
#include "CurrentMapView.h"
#include "GlobalMapView.h"


MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent)
{
    pathPlanner = new PathPlanner(GlobalMapWidth, GlobalMapHeight, CurMapWidth, CurMapHeight);
    currentMapView = new CurrentMapView;
    globalMapView = new GlobalMapView;
    beginButton = new QPushButton;
    stopButton = new QPushButton;
    pauseButton = new QPushButton;

    this->setFixedSize(WinWidth, WinHeight);

    QGridLayout *globalLayout = new QGridLayout;
    globalLayout->addWidget(globalMapView, 0, 0, 4, 4);
    QVBoxLayout *agentViewLayout = new QVBoxLayout;
    globalLayout->addLayout(agentViewLayout, 0, 4, 3, 4);
    agentViewLayout->addWidget(currentMapView, 3);
    globalLayout->addWidget(beginButton, 5, 1, 1, 1);
    globalLayout->addWidget(stopButton, 5, 3, 1, 1);
    globalLayout->addWidget(pauseButton, 5, 5, 1, 1);

    beginButton->setText("begin");
    stopButton->setText("stop");
    pauseButton->setText("pause");

    connect(beginButton, SIGNAL(clicked()), this, SLOT(actSimuBegin()));
    connect(stopButton, SIGNAL(clicked()), this, SLOT(actSimuStop()));
    connect(pauseButton, SIGNAL(clicked()), this, SLOT(actSimuPause()));


    QGroupBox *dataBox = new QGroupBox;
    agentViewLayout->addWidget(dataBox, 1);
    dataBox->setTitle("agent state");

    agentXLabel = new QLabel;
    agentYLabel = new QLabel;
    agentX      = new QLineEdit;
    agentY      = new QLineEdit;

    agentX->setReadOnly(true);
    agentY->setReadOnly(true);
    agentXLabel->setText(QString("pos X"));
    agentYLabel->setText(QString("pos Y"));
    agentX->setText("0");
    agentY->setText("0");

    QGridLayout *dataBoxLayout = new QGridLayout;
    dataBoxLayout->addWidget(agentXLabel, 0, 0, 1, 1);
    dataBoxLayout->addWidget(agentYLabel, 1, 0, 1, 1);
    dataBoxLayout->addWidget(agentX, 0, 1, 1, 1);
    dataBoxLayout->addWidget(agentY, 1, 1, 1, 1);

    dataBox->setLayout(dataBoxLayout);

    currentMapView->setMap(&(pathPlanner->getCurMap(100, 100)));
    globalMapView->setMap(&(pathPlanner->getGlobalMap()));
    this->setLayout(globalLayout);

    connect(&timer, SIGNAL(timeout()), this, SLOT(tmRunning()));

    update();
}

void MainWidget::actSimuBegin()
{
    timer.start(50);
    pathPlanner->running();
}

void MainWidget::actSimuStop()
{
    pathPlanner->stop();
    timer.stop();
    //qDebug() << "stop";
    if(QMessageBox::question(nullptr, "really stop?", "if you stop the simulation, all the data will be cleared",
                             QMessageBox::Yes, QMessageBox::No | QMessageBox::Default) == QMessageBox::Yes)
        pathPlanner->clearAll();
    else
    {
        timer.start(50);
        pathPlanner->running();
    }
}

void MainWidget::actSimuPause()
{
    pathPlanner->stop();
    timer.stop();
}

void MainWidget::tmRunning()
{
    //qDebug() << "running!";
    pathPlanner->running();
    update();
}
