#include "graph.h"

Graph::Graph(const std::string &line, const QVector<std::string> &valid_strings)
{
    data.morse_line = line;
    data.valids = valid_strings;
    makeGraph(data.valids);
    BFS(data.graph);
}


bool Graph::is_one_space_diff(const std::string& s1, const std::string& s2)
{
    size_t pos=0;
    while((pos !=std::min(s1.size(), s2.size())) && s1[pos] == s2[pos]){
        ++pos;
    }
    if(pos == std::min(s1.size(), s2.size())) { return false;}
    return (s1[pos] ==' ')?
        (s1.substr(pos+1) == s2.substr(pos)):
        (s1.substr(pos) == s2.substr(pos+1));
}

QVector<QVector<int>> Graph::makeGraph(QVector<std::string>& valids)
{
    size_t valid_count = valids.size();
    data.graph.resize(valid_count);
    for(size_t raws =0; raws!=valid_count;++raws){
        for(size_t columns =0; columns!=valid_count; ++columns) {
            data.graph[raws].push_back( is_one_space_diff(valids[raws], valids[columns]));
        }
    }
    return data.graph;
}

QVector<QVector<int> > Graph::BFS(QVector<QVector<int> > graph)
{
    enum {WHITE, GRAY, BLACK};
    data.spanning_tree.resize(graph.size());
    for(auto& x:data.spanning_tree) {
        x.fill(0,graph.size());
    }
    QVector<int> colors(graph.size(), WHITE);
    colors[0] = GRAY;
    QQueue<int> vert_in_progress;
    vert_in_progress.enqueue(0);
    while(!vert_in_progress.empty()) {
        //qDebug()<<"size of queue: "<<vert_in_progress.size();
        int index = vert_in_progress.head();
        vert_in_progress.dequeue();
        //qDebug()<<"pop. "<<"size of a queue: "<<vert_in_progress.size();
        auto it = graph[index].begin();
        while((it = std::find(it, graph[index].end(), 1))!=graph[index].end()) {
            size_t next_vert_pos = std::distance(graph[index].begin(), it);
            //qDebug()<<"nextvpos: "<<next_vert_pos;
            ++it;
            //qDebug()<<"colors: ";
            std::for_each(colors.begin(), colors.end(), [](int i) { qDebug()<<i<<' ';});
            if (colors[next_vert_pos] == WHITE) {

                colors[next_vert_pos] =GRAY;
                data.spanning_tree[index][next_vert_pos] = data.spanning_tree[next_vert_pos][index] = 1;
                vert_in_progress.enqueue(next_vert_pos);
            }
        }
        colors[index] = BLACK;
    }
    return data.spanning_tree;
}

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

QByteArray Graph::getDataFromByteArr()
{
    QByteArray ret_ar;
    QDataStream wdstr(&ret_ar, QIODevice::WriteOnly);
    wdstr<<data;
    return ret_ar;
}

Graph& Graph::setDataFromByteArr(QByteArray &ba)
{
    QDataStream rdstr(ba);
    rdstr>>data;
    return *this;
}

QDebug &operator<<(QDebug &d, const std::string &str)
{
    d<<QString::fromStdString(str);
    return d;
}
