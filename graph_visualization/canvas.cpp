#include "canvas.h"

Canvas::Canvas(QWidget *parent) : QWidget(parent)
{
    setMinimumSize(400,600);
    setBaseSize(600,800);
}

void Canvas::paintEvent(QPaintEvent * pe)
{
    if(data) {
        QPainter painter(this);
        QVector<QPoint> vertx_points = computeGraphVerticesPos(data);
        QBrush circle_brush(Qt::green, Qt::BrushStyle::SolidPattern);
        painter.setBrush(circle_brush);
        QFont font = painter.font();
        font.setPointSize(font.pointSize()*1.5);
        painter.setFont(font);
        int circle_diameter = 30;
        QRect circle_rect;

        for(int i =0; i!=data->spanning_tree.size();++i) {
            for(int j = i; j!=data->spanning_tree[i].size(); ++j) {
                if(data->spanning_tree[i][j]) {
                    painter.drawLine(vertx_points[i], vertx_points[j]);
                }
            }
            circle_rect.setRect(vertx_points[i].x()-circle_diameter/2, vertx_points[i].y()-circle_diameter/2,circle_diameter,circle_diameter);
            painter.drawEllipse(circle_rect);
            painter.drawText(vertx_points[i] + QPoint(-10,-20),QString::fromStdString(data->valids[i]));
            painter.setPen(Qt::red);
            painter.drawText(circle_rect, Qt::AlignCenter,QString::number(i+1));    //  Номера для сопоставления с сгенерированным файлом
            painter.setPen(Qt::black);
        }
    }
}

QVector<QPoint> Canvas::computeGraphVerticesPos(const graph_data* dg)
{
    QPoint painting_point = {this->width()/2, 50};
    QVector<QPoint> vertx_points(data->valids.size());
    QQueue<int> current_lvl, next_lvl;
    current_lvl.enqueue(0);
    vertx_points[0] = painting_point;
    int horizontal_inc, vertical_inc = this->height()/(dg->morse_line.size() +1);
    while(!current_lvl.isEmpty()){
        int node = current_lvl.dequeue();
        for(int i = node; i!=dg->valids.size();++i) {   //i = node  т.к. симметрия
            if(dg->spanning_tree[node][i]) {
                next_lvl.enqueue(i);
            }
        }
        if(current_lvl.isEmpty() && !next_lvl.isEmpty()) {
            current_lvl = next_lvl;
            next_lvl.clear();
            horizontal_inc = this->size().width()/(current_lvl.size() +1); //инкремент ширины в зависимости от кол-ва верщин на уровне
            int width = horizontal_inc;
            painting_point.ry()+=vertical_inc;
            for(int i =0; i!=current_lvl.size(); ++i) {
                vertx_points[current_lvl[i]] = {width,painting_point.y()};
                width+=horizontal_inc;
            }
        }
    }
    return vertx_points;
}
