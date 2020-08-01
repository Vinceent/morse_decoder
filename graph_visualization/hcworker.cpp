#include "hcworker.h"

void HCWorker::handleConnection()
{
    //connect(clientConnection,&QLocalSocket::readyRead, this, &HCWorker::read_when_ready);
}

void HCWorker::read_when_ready()
{
    qDebug()<<"readwhenreadyyo runs ";
    if(!expectedSize && clientConnection->bytesAvailable()) {
        clientConnection->read((char*)&expectedSize, sizeof(int));
        qDebug()<<"expected: "<<expectedSize;
        if(grsize == -1) {
            clientConnection->read((char*)&grsize, sizeof(int));
        }
    }
    if(expectedSize && clientConnection->bytesAvailable()) {
        qDebug()<<"expected & aviable";
        QByteArray chunk = clientConnection->read(clientConnection->bytesAvailable());
        qDebug()<<chunk.size();
        buff+=chunk;
        expectedSize-=chunk.size();
        qDebug()<<"Recieved chunk of: "<<chunk.size();
        if(!expectedSize) {
            qDebug()<<"Recieved block of size: "<<buff.size();
            qDebug()<<"bytes left in a socket: "<<clientConnection->bytesAvailable();
            clientConnection->disconnectFromServer();
            clientConnection->deleteLater();
            send_data();
        }
    }
    qDebug()<<"exp: "<<expectedSize<<" avaliable: "<<clientConnection->bytesAvailable();
}
void HCWorker::send_data()
{
    QVector<graph_data> inp;
    graph_data temp;
    QDataStream dstream(buff);
    qDebug()<<dstream.status();
    for(int i =0; i!=grsize;++i) {
        dstream>>temp;
        qDebug()<<temp.morse_line.size()<<' '<<temp.valids.size()<<' '<<temp.graph.size();

        inp.push_back(temp);
    }
//    QDataStream inp_str(clientConnection);
//    graph_data temp;
//    for(size_t i =0; i!=size; ++i) {
//        inp_str>>temp;
//        inp.push_back(std::move(temp));
//    }
    QString time ="Получено "+ QTime::currentTime().toString("h:m:s.zzz");
    qDebug()<<"vec size: "<<inp.size();
    mutex.lock();
    qDebug()<<"mutex locked in thread";
    newdata.push_back({std::move(time),std::move(inp)});
    data_model->layoutChanged();
    qDebug()<<"mutex unlocked in thread";
    mutex.unlock();
    emit finnished();
}
