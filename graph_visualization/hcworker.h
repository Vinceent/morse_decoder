#ifndef HCWORKER_H
#define HCWORKER_H

#include <QObject>
#include <QMutex>
#include <../decoder/graph.h>
#include <QLocalSocket>
#include <QThread>
#include <QTime>
#include <messagelistmodel.h>
class HCWorker : public QObject
{
    Q_OBJECT
public:
    explicit HCWorker(QObject *p, QMutex &m, QList<QPair<QString,QVector<graph_data>>>& gdvl, QLocalSocket * cli,MessageListModel *model) :
        QObject(p), mutex(m), newdata(gdvl),clientConnection(cli), data_model(model) {qDebug()<<">>>>>>>>>>>>>>>>>>~~~~constructed thread";}
    ~HCWorker() {qDebug()<<">>>>>>>>>>>>>>>>>>>>>>>>HCWorker object deleted";}
private:
    QMutex &mutex;
    QList<QPair<QString,QVector<graph_data>>> &newdata;
    QLocalSocket * clientConnection;
    MessageListModel *data_model;
public slots:
    void handleConnection();
    void read_when_ready();
signals:
    void finnished();

};

#endif // HCWORKER_H
