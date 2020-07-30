#include "canvas.h"

Canvas::Canvas(QWidget *parent) : QWidget(parent)
{
    setMinimumSize(400,600);
    setBaseSize(600,800);
}

//for(x:items in lvl) {
//    for(children C with number > x) {
//        put in the next lvl
//    }
//    if next lvl is not empty go to next lvl
//            else end;
//}

void Canvas::paintEvent(QPaintEvent * pe)
{
    if(data) {
        QPainter painter(this);
        QPoint painting_point = {this->width()/2, 50}; //change 50 to a non magical number please
        QVector<QPoint> vert_points(data->valids.size());
        QQueue<int> current_lvl;
        QQueue<int> next_lvl;
        current_lvl.enqueue(0);
        vert_points[0] = painting_point;
        int horizontal_inc = 0, vertical_inc = this->height()/(data->morse_line.size() +1);
        while(!current_lvl.isEmpty()){
            int node = current_lvl.dequeue();
            for(int i = node; i!=data->valids.size();++i) {
                if(data->spanning_tree[node][i]) {
                    next_lvl.enqueue(i);
                }
            }
            qDebug()<<next_lvl.size();
            if(current_lvl.isEmpty() && !next_lvl.isEmpty()) {
                qDebug()<<"inside";
                current_lvl = next_lvl;
                next_lvl.clear();
                horizontal_inc = this->size().width()/(current_lvl.size() +1);
                int width = horizontal_inc;
                painting_point.ry()+=vertical_inc;
                for(int i =0; i!=current_lvl.size(); ++i) {
                    vert_points[current_lvl[i]] = {width,painting_point.y()};
                    width+=horizontal_inc;
                    qDebug()<<QPoint({width,painting_point.y()});
                }
            }
        }
        qDebug()<<vert_points;

        for(int i =0; i!=data->spanning_tree.size();++i) {
            for(int j = i; j!=data->spanning_tree[i].size(); ++j) {
                if(data->spanning_tree[i][j]) {
                    painter.drawLine(vert_points[i], vert_points[j]);
                }
            }
        }
        QBrush circle_brush(Qt::green, Qt::BrushStyle::SolidPattern);
        painter.setBrush(circle_brush);
        QFont font = painter.font();
        font.setPointSize(font.pointSize()*1.5);
        painter.setFont(font);


        for(int i = 0; i!= vert_points.size(); ++i) {
            painter.drawEllipse(vert_points[i], 10, 10);
            painter.drawText(vert_points[i] + QPoint(-10,-20),QString::fromStdString(data->valids[i]));
        }
    }
}
