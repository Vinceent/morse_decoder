#include "hcworker.h"

void HCWorker::read_when_ready()
{
    if(!expectedSize && clientConnection->bytesAvailable()) {
        clientConnection->read((char*)&expectedSize, sizeof(int));
        //qDebug()<<"expected: "<<expectedSize;
        if(grsize == -1) {
            clientConnection->read((char*)&grsize, sizeof(int));
        }
    }
    if(expectedSize && clientConnection->bytesAvailable()) {
        if(grsize == -1) {
            clientConnection->read((char*)&grsize, sizeof(int));
        }
        QByteArray chunk = clientConnection->read(clientConnection->bytesAvailable());
        buff+=chunk;
        expectedSize-=chunk.size();
        if(!expectedSize) {
            //qDebug()<<"Recieved block of size: "<<buff.size();
            //qDebug()<<"bytes left in a socket: "<<clientConnection->bytesAvailable();
            clientConnection->disconnectFromServer();
            clientConnection->deleteLater();
            send_data();
        }
    }
}
void HCWorker::send_data()
{
    QVector<graph_data> inp;
    graph_data temp;
    QDataStream dstream(buff);
    for(int i =0; i!=grsize;++i) {
        dstream>>temp;
        inp.push_back(temp);
    }
    QString time ="Получено "+ QTime::currentTime().toString("hh:mm:ss.zzz");
    mutex.lock();
    qDebug()<<"mutex locked";
    newdata.push_back({std::move(time),std::move(inp)});

    data_model->layoutChanged();
    qDebug()<<"mutex unlocked";
    mutex.unlock();
    emit finnished();
}
