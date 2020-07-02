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

class Graph
{
public:
    Graph() = default;
    Graph(const Graph& g): data(g.data) { }
    Graph& operator=(const Graph& g){data = g.data; return *this;}
    Graph(const Graph&& g): data(std::move(g.data)) {}
    Graph& operator=(const Graph&& g){data = std::move(g.data); return *this;}
    Graph(const std::string &line,const QVector<std::string> &valis_strings);
    QByteArray getDataFromByteArr();
    void setDataFromByteArr(QByteArray& ba);
private:
    QVector<QVector<int>> makeGraph(QVector<std::string>&);
    QVector<QVector<int>> BFS(QVector<QVector<int>> graph);
    bool is_one_space_diff(const std::string& s1, const std::string& s2);
    graph_data data;
};

QDataStream &operator>>(QDataStream &is, graph_data& grd);
QDataStream &operator<<(QDataStream &os, const graph_data& grd);
QDataStream &operator<<(QDataStream&os, const std::string &str);
QDataStream &operator>>(QDataStream& os, std::string &str);
QDebug &operator<<(QDebug& d, const std::string& str);


#endif // GRAPH_H
