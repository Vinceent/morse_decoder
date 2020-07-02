#ifndef GRAPH_H
#define GRAPH_H
#include<string>
#include<QVector>
#include<QQueue>
#include <QDebug>
#include <algorithm>
#include <QByteArray>
#include <QDataStream>

struct graph_data{
    std::string morse_line;
    QVector<std::string> valids;
    QVector<QVector<int>> graph;
    QVector<QVector<int>> spanning_tree;

};

QDataStream &operator<<(QDataStream&os, const std::string &str)
{
    os <<str.c_str();
    return os;
}

QDataStream &operator>>(QDataStream& os, std::string &str)
{
    char *data;
    os>>data;
    str = data;
    delete[] data; //Space for the string is allocated using new [] -- the caller must destroy it with delete [].
    data = nullptr;
    return os;
}

QDataStream &operator<<(QDataStream &os, const graph_data& grd)
{
    os<<QString::fromStdString(grd.morse_line)<<grd.valids<<grd.graph<<grd.spanning_tree;
    return os;
}

QDataStream &operator>>(QDataStream &is, graph_data& grd)
{
    QString temp;
    is>>temp>>grd.valids>>grd.graph>>grd.spanning_tree;
    grd.morse_line = temp.toStdString();
    return is;
}

class Graph
{
public:
    Graph(const std::string &line,const QVector<std::string> &valis_strings);
    QByteArray getDataFromByteArr();
    void setDataFromByteArr(QByteArray& ba);
private:
    QVector<QVector<int>> makeGraph(QVector<std::string>&);
    QVector<QVector<int>> BFS(QVector<QVector<int>> graph);
    bool is_one_space_diff(const std::string& s1, const std::string& s2);
    graph_data data;
};

#endif // GRAPH_H
