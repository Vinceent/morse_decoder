#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <set>
#include <QVector>
#include "graph.h"
#include <QLocalSocket>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_fileButton_clicked();

    void on_computeButton_clicked();

    void on_logButton_clicked();

    void on_visualButton_clicked();

private:
    bool set_code_strings();
    std::set<std::string> m_table;
    Ui::MainWindow *ui;
    std::string code_filename;
    QVector<std::string> code_strings;
    QVector<Graph> graphs;

};
#endif // MAINWINDOW_H
