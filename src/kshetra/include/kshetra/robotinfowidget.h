#ifndef ROBOTINFOWIDGET_H
#define ROBOTINFOWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class RobotInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RobotInfoWidget(QWidget *parent = nullptr);
    void updateInfo(int id, const QPointF &pos, float orientation);
private:
    QLabel *idLabel;
    QLabel *positionLabel;
    QLabel *orientationLabel;


};

#endif // ROBOTINFOWIDGET_H
