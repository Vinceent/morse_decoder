#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QLocalServer>
#include <QList>
#include<../decoder/graph.h>
#include"hcworker.h"
#include"messagelistmodel.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void reply();
    ~MainWindow();

private:
    QMutex acess_mutex;
    QLocalServer* server;
    Ui::MainWindow *ui;
    MessageListModel *msModel;
    GraphListModel *grModel;
    QList<QPair<QString,QVector<graph_data>>> recieved_data;

private slots:
    void handle_connection();
};
#endif // MAINWINDOW_H
