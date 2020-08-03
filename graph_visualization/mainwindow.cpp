#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
#include <vector>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    server = new QLocalServer(this);

    if(!server->listen("UniqueServerName")) {
        QMessageBox::critical(this,"Ошибка!","UniqueServerName: "+ server->errorString());
        exit(0);
    }
    connect(server, &QLocalServer::newConnection,this, &MainWindow::handle_connection);
    msModel = new MessageListModel(recieved_data,this);
    grModel = new GraphListModel(this);

    msModel->setModelForSelection(grModel);

    ui->listView->setModel(msModel);
    connect(ui->listView, &QListView::clicked, msModel, &MessageListModel::msg_chosen);
    ui->listView_2->setModel(grModel);
    connect(ui->listView_2, &QListView::clicked, grModel, &GraphListModel::graph_chosen);
    connect(ui->listView, &QListView::clicked,[this](){ui->listView_2->clearSelection();});
    connect(grModel, &GraphListModel::repaint_graph, ui->canvasWidget, &Canvas::setGraph);
    qDebug()<<"constructed";

    QFont font = ui->listView->font();
    font.setPointSize(font.pointSize()*1.3);
    ui->listView->setFont(font);
    ui->listView_2->setFont(font);
}

void MainWindow::handle_connection()
{
    qDebug()<<"handle connection";
    QLocalSocket *new_connection = server->nextPendingConnection();

    HCWorker* worker = new HCWorker(this,acess_mutex,recieved_data,new_connection, msModel);
    connect(new_connection, &QLocalSocket::readyRead, worker, &HCWorker::read_when_ready);
    connect(worker, &HCWorker::finnished, worker, &QObject::deleteLater);

}


MainWindow::~MainWindow()
{
    delete ui;
}


