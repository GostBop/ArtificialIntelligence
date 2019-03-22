#ifndef STEP_H
#define STEP_H

#include <QObject>
#include <Item.h>

class Step : public QObject
{
    Q_OBJECT
public:
    explicit Step(QObject *parent = 0);
    ~Step();

    Item move_item;
    Item kill_item;
    QPoint from_pt;
    QPoint to_pt;


signals:

public slots:
};

#endif // STEP_H
