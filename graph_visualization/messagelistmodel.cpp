#include "messagelistmodel.h"

const int MessageListModel::msgDataRole = Qt::UserRole +1;

QVariant MessageListModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid() && (model_data.size()>index.row())) {

        if(role == Qt::DisplayRole) {
            return model_data.at(index.row()).first;
            qDebug()<<"should've painted";
        }
    }
    return QVariant();
}

void MessageListModel::msg_chosen(const QModelIndex &index)
{
    const QVector<graph_data> *newdata = &(model_data.at(index.row()).second);
    selected_msg_model->setModelData(newdata);
}

int GraphListModel::rowCount(const QModelIndex &parent) const
{
    if (model_data == nullptr) {
        return 0;
    } return model_data->size();
}

QVariant GraphListModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid() && model_data && (model_data->size()>index.row())) {
        if(role == Qt::DisplayRole) {
            return QString::fromStdString(model_data->at(index.row()).morse_line);
        }
    }
    return QVariant();
}

void GraphListModel::setModelData(const QVector<graph_data> *newdata)
{
    model_data = newdata;

    layoutChanged();
    //graph_chosen(QAbstractItemModel::createIndex(0,0));

}

void GraphListModel::graph_chosen(const QModelIndex &index)
{
    emit repaint_graph(&model_data->at(index.row()));
}
