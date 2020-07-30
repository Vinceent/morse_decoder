#include "hcworker.h"

void HCWorker::handleConnection()
{
    connect(clientConnection,&QLocalSocket::readyRead, this, &HCWorker::read_when_ready);
}

void HCWorker::read_when_ready()
{
    qDebug()<<"runs readwhenready";
    QVector<graph_data> inp;
    size_t size =0;
    qDebug() <<clientConnection->state();
    clientConnection->read((char*)&size, sizeof(size_t));
    qDebug()<<"size: "<<size;
    QDataStream inp_str(clientConnection);
    graph_data temp;
    for(size_t i =0; i!=size; ++i) {
        inp_str>>temp;
        qDebug()<<temp.graph.size();
        inp.push_back(std::move(temp));
    }
    QString time = QTime::currentTime().toString("h:m:s.zzz");
    mutex.lock();
    qDebug()<<"mutex locked in thread";
    newdata.push_back({std::move(time),std::move(inp)});
    data_model->layoutChanged();        //questionable decision
    qDebug()<<"mutex unlocked in thread";
    mutex.unlock();
    clientConnection->disconnectFromServer();
    emit finnished();
}
