// RobotInfoWidget.cpp
#include "robotinfowidget.h"

RobotInfoWidget::RobotInfoWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    idLabel = new QLabel("ID: ", this);
    positionLabel = new QLabel("Position: ", this);
    orientationLabel = new QLabel("Orientation: ", this);

    layout->addWidget(idLabel);
    layout->addWidget(positionLabel);
    layout->addWidget(orientationLabel);
    setLayout(layout);
    setMinimumWidth(800); // You can increase this if needed
    layout->setSizeConstraint(QLayout::SetMinimumSize);
}

void RobotInfoWidget::updateInfo(int id, const QPointF &pos, float orientation)
{
    idLabel->setText(QString("ID: %1").arg(id));
    positionLabel->setText(QString("Position: (%1, %2)").arg(pos.x()).arg(pos.y()));
    orientationLabel->setText(QString("Orientation: %1°").arg(orientation));
}
