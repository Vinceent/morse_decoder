#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QPushButton>
#include <QPainter>
#include <../decoder/graph.h>

class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event) override;
    void setGraph(const graph_data * gd) { data = gd; update();}

private:
    QVector<QPoint> computeGraphVerticesPos(const graph_data* dg);
    const graph_data* data = nullptr;
signals:

};

#endif // CANVAS_H
