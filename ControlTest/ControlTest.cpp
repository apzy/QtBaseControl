#include "ControlTest.h"


ControlTest::ControlTest(QWidget *parent)
    : FramelessWidget(parent)
{
    ui.setupUi(this);

    connect(ui.max, SIGNAL(clicked()), this, SLOT(slot_max_clicked()));
    connect(ui.close, SIGNAL(clicked()), this, SLOT(slot_close_clicked()));

    set_resizable(false);
}

void ControlTest::slot_max_clicked()
{
#if 0
    if (isMaximized())
    {
        showNormal();
    }
    else
    {
        showMaximized();
    }
#else
    if (isFullScreen())
    {
        showNormal();
    }
    else
    {
        showFullScreen();
    }
#endif
}

void ControlTest::slot_close_clicked()
{
    close();
}
