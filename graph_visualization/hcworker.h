#ifndef HCWORKER_H
#define HCWORKER_H

#include <QObject>
#include <QMutex>
#include <../decoder/graph.h>
#include <QLocalSocket>
#include <QTime>
#include <messagelistmodel.h>
class HCWorker : public QObject
{
    Q_OBJECT
public:
    explicit HCWorker(QObject *p, QMutex &m, QList<QPair<QString,QVector<graph_data>>>& gdvl, QLocalSocket * cli,MessageListModel *model) :
        QObject(p), mutex(m), newdata(gdvl),clientConnection(cli), data_model(model) {qDebug()<<">>>>>>>>>>>>>>>>>>~~~~constructed HCWorker";}
    ~HCWorker() {qDebug()<<">>>>>>>>>>>>>>>>>>>>>>>>HCWorker object deleted";}
private:
    int grsize =-1;
    int expectedSize =0;
    QByteArray buff;
    QMutex &mutex;
    QList<QPair<QString,QVector<graph_data>>> &newdata;
    QLocalSocket * clientConnection;
    MessageListModel *data_model;
    void send_data();
public slots:
    //void handleConnection();
    void read_when_ready();
signals:
    void finnished();

};

#endif // HCWORKER_H
