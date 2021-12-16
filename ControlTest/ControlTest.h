#pragma once

#include <QtWidgets/QWidget>
#include "ui_ControlTest.h"
#include "FramelessWidget.h"

class ControlTest : public FramelessWidget 
{
    Q_OBJECT

public:
    ControlTest(QWidget *parent = Q_NULLPTR);

private Q_SLOTS:
    void slot_max_clicked();

    void slot_close_clicked();

private:
    Ui::ControlTestClass ui;
};
