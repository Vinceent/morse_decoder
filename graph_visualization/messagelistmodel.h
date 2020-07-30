#ifndef MESSAGELISTMODEL_H
#define MESSAGELISTMODEL_H
#include<QAbstractListModel>
#include<../decoder/graph.h>

class GraphListModel;

class MessageListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    MessageListModel(QList<QPair<QString,QVector<graph_data>>> &d,QObject *parent = nullptr):
        QAbstractListModel(parent), model_data(d), selected_msg_model(nullptr) {}
    int rowCount(const QModelIndex &parent = QModelIndex()) const override { return model_data.size();}
    QVariant data(const QModelIndex &index, int role)const override;
    void setModelForSelection(GraphListModel *model) { selected_msg_model = model;}
    static const int msgDataRole;
private:
    QList<QPair<QString,QVector<graph_data>>>& model_data;
    GraphListModel *selected_msg_model;

public slots:
    void msg_chosen(const QModelIndex &index);
};

class GraphListModel: public QAbstractListModel
{
    Q_OBJECT
public:
    GraphListModel(QObject *parent = nullptr):
        QAbstractListModel(parent) {}
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    void setModelData(const QVector<graph_data>* newdata);
private:
    const QVector<graph_data> *model_data = nullptr;
public slots:
    void graph_chosen(const QModelIndex& index);

signals:
    void layoutUpdated();
    void repaint_graph(const graph_data *);
};

#endif // MESSAGELISTMODEL_H
