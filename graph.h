#ifndef GRAPH_H
#define GRAPH_H
#include<string>
#include<QVector>
#include<QQueue>
#include <QDebug>
#include <algorithm>

struct graph_data{
    std::string morse_line;
    QVector<std::string> valids;
    QVector<QVector<int>> graph;
    QVector<QVector<int>> spanning_tree;

};

class Graph
{
public:
    Graph(const std::string &line,const QVector<std::string> &valis_strings);
    QVector<QVector<int>> makeGraph(QVector<std::string>&);
    QVector<QVector<int>> BFS(QVector<QVector<int>> graph);
private:
    bool is_one_space_diff(const std::string& s1, const std::string& s2);
    graph_data data;
};

#endif // GRAPH_H
