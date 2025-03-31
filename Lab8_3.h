#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Lab8_3.h"

class Lab8_3 : public QMainWindow
{
    Q_OBJECT

public:
    Lab8_3(QWidget *parent = nullptr);
    ~Lab8_3();

private:
    Ui::Lab8_3Class ui;
};
