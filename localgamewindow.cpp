#include "localgamewindow.h"
#include <QPixmap>

LocalGameWindow::LocalGameWindow(QWidget *parent)
    : QGraphicsView(parent)
{
setFixedSize(800,400);
m_localgamescene = new LocalGameScene();
setScene(m_localgamescene);
this->setBackgroundBrush(QBrush(
                             QPixmap(":/images/startui_background.gif").scaled(
                                 this->size(),
                                 Qt::IgnoreAspectRatio,
                                 Qt::SmoothTransformation)));
setStyleSheet("border: 0px;"
              "padding: 0px;");

}

LocalGameWindow::~LocalGameWindow()
{
    if(m_localgamescene != nullptr)
        delete m_localgamescene;
}
