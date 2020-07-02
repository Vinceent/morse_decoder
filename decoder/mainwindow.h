#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <set>
#include <QVector>
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

private:
    bool set_code_strings();
    std::set<std::string> m_table;
    Ui::MainWindow *ui;
    std::string code_filename;
    QVector<std::string> code_strings;

};
#endif // MAINWINDOW_H
