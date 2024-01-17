#pragma once

#include <QtWidgets/QMainWindow>

#include "GOL.h"


class QGOLWidget;


class GOLApp : public QMainWindow
{
    Q_OBJECT

public:
    GOLApp(QWidget * parent = nullptr);

    void addEngine(GOL * gol);

private:
    QGOLWidget * mGOLWidget;
};
