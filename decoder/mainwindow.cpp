#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "morsedecoder.h"
#include<algorithm>
#include<fstream>
#include "unistd.h"
#include<QMessageBox>
#include<chrono>
#include<QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->visualButton->setHidden(true);
    ui->fileButton->setIcon(QIcon::fromTheme("folder"));
    ui->lineEdit->setReadOnly(true);
    ui->headLabel->setText("Pick a file with the coded morse sequence.");
    ui->logBrowser->setHidden(true);
    ui->label->clear();
    ui->label_2->clear();
    std::string morse_filename ="morse";
    std::ifstream m_file(morse_filename);                                                   //Reading morse table form the file named "morse".
    std::string temp;
    if(m_file.is_open()) {
        while(getline(m_file, temp)){
            m_table.insert(temp.substr(2));
        }
        m_file.close();
    } else {
        QMessageBox::question(this, "The morse table wasn't found", "The folder should contain a file named \" morse\" \n"
                                                                                "with format {letter} {code} on each line\nProgram interrupted.",QMessageBox::Ok);
        exit(0);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_fileButton_clicked()
{
    ui->label_2->clear();
    code_filename = QFileDialog::getOpenFileName(this, "", "~/").toLocal8Bit().constData();     //Convert to cstring, then construct the std::string.
    ui->lineEdit->setText(QString(code_filename.c_str()));
//    std::ifstream inp_f(code_filename);
//    if(!inp_f.is_open()) {
//        QMessageBox::question(this, "Error!", "The file wasn't opened.", QMessageBox::Ok);
//        return;

//    }
//    set_code_strings(inp_f);
}

bool MainWindow::set_code_strings()
{
    code_strings.clear();
    std::ifstream fs(code_filename);
    if(!fs.is_open()) {
        QMessageBox::question(this, "Error!", "The file wasn't opened.", QMessageBox::Ok);
        return false;

    }
    bool ok = true;
    std::string temp;
    size_t counter =0;
    ui->logBrowser->clear();
    std::string::size_type pos;
    while(getline(fs, temp)) {
        pos = (temp.find_first_not_of(".-"));
        if(pos!=std::string::npos) {
            ui->logBrowser->append(QString("Error:line %1 contains a non-morse symbol on pos %2, skipped").arg(counter +1).arg(pos +1));
        } else if (temp.size()!=0) {
            code_strings.push_back(temp);
        }
        ++counter;
    }
    if(counter ==0) {
        ui->label->setText("ERROR:\nThe file is empty.");
        ok = false;
    } else if(!ui->logBrowser->toPlainText().isEmpty()) {
        ui->label->setText("Non-morse lines encountered, check logs.");
        if(code_strings.isEmpty()) {
            ui->label->setText("ERROR:\nno valid lines found, check logs.");
            ok = false;
        }
    } else if (ok){
        ui->label->clear();
        ui->headLabel->setText(QString
                               ("%1 valid strings were read.\nNow you're able to compute morse strings.")
                               .arg(code_strings.size()));
    }
    fs.close();
    return ok;
}

static std::ofstream& buff_vec_str_outp(std::ofstream& ofs, const size_t buffer_size, const QVector<std::string>& outp_vector)
//buffered file output
{
    std::string buffer;
    buffer.reserve(buffer_size);
    for(auto& x:outp_vector){
        if(buffer.size() + x.size() + 1>=buffer_size) {
            ofs<<buffer;
            buffer.resize(0);
        }
        buffer.append(x).append(1,'\n');
    }
    ofs<<buffer;
    return ofs;
}

void MainWindow::on_computeButton_clicked()
{

    if(!set_code_strings()) {
        ui->visualButton->setHidden(true);
        return;
    }
    std::string::size_type slash_pos = code_filename.find_last_of("/");
    std::string save_filename("decoded_" +std::string(code_filename ,slash_pos+1)); //Gettin a filename from the absolute path
    std::ofstream out_f(save_filename);
    size_t generated_count = 0;
    std::string full_filename(std::string(code_filename,0,slash_pos+1) + save_filename);
    std::string delim(80, '=');

    if(!out_f.is_open()) {
        QMessageBox::question(this, "Error!","The output file wasn't opened/generated for some reason.\n"
                                             "Check the setting of the folder.", QMessageBox::Ok);
        ui->headLabel->setText("File wasnt's opened. Sad!");
        return;
    }
    for(int i =0; i!=code_strings.size(); ++i) {
        if(i) {
            out_f<<'\n'<<delim<<'\n';
        }
        std::string &code_string = code_strings[i];
        out_f<<"String "<< i<<'('<<code_string<<"):\n";
        if(code_string.size()>25) {
            auto answ = QMessageBox::question(this, "Warning!",QString("The line № %1 is longer than 25 symbols.\n"
                                                                "It might cause lags due to enormous memory consuption\n"
                                                                "Skip it?").arg(i+1),QMessageBox::No| QMessageBox::Yes);
            if(answ == QMessageBox::Yes ) {
                continue;
            }
        }
        ui->headLabel->setText("Processing...");

        MorseDecoder Decoder(m_table, code_string);

        auto run_t1 = std::chrono::high_resolution_clock::now();
        QVector<std::string>& valids = Decoder.result();
        auto run_t2 = std::chrono::high_resolution_clock::now();

        ui->headLabel->setText("Done!");
        generated_count+=valids.size();

        auto save_t1 = std::chrono::high_resolution_clock::now();
        buff_vec_str_outp(out_f, 16000,valids);
        auto save_t2 = std::chrono::high_resolution_clock::now();

        ui->logBrowser->append( QString("%1 lines generated from (%2) on line %3 of valid strings").arg(valids.size()).arg(code_string.c_str()).arg(i +1));
        auto duration_save = std::chrono::duration_cast<std::chrono::microseconds>( save_t2 - save_t1 ).count();
        auto duration_run = std::chrono::duration_cast<std::chrono::microseconds>( run_t2 - run_t1 ).count();
        out_f<<"computation time (msec):"<<duration_run<<"\tsave time (msec):"<<duration_save<<'\n';
        if(code_string.size() <6) {
            graphs.push_back({code_string, valids});
        }
    }
    ui->label->setText("valid morse strings saved at:\n" + QString::fromStdString(full_filename));
    ui->label_2->setText(QString::number(generated_count) +" lines were generated.");
    out_f.close();
    ui->visualButton->setHidden(false);
//testing area
    QByteArray a= graphs[0].getDataFromByteArr();
    QDataStream w(a);
    graph_data d;
    w>>d;
    qDebug()<<QString::fromStdString(d.morse_line);
    qDebug()<<d.valids;
    for(auto&x :d.graph) {
        qDebug()<<x;
    }
    qDebug()<<'\n';
    for(auto &x: d.spanning_tree) {
        qDebug()<<x;
    }
}


void MainWindow::on_logButton_clicked()
{
    if(ui->logButton->arrowType() == Qt::ArrowType::RightArrow) {
        ui->logButton->setArrowType(Qt::ArrowType::LeftArrow);
        ui->logBrowser->setHidden(false);

    } else {
        ui->logButton->setArrowType(Qt::ArrowType::RightArrow);
        ui->logBrowser->setHidden(true);
    }
}

void MainWindow::on_visualButton_clicked()
{
    QLocalSocket *sock =new QLocalSocket(this);
    sock->connectToServer("me serv");
    for(int i =0; i!=10; ++i) {
        if(!sock->waitForConnected()) {
            qDebug()<<"Connection error:"<< sock->error();
            ui->logBrowser->append("Unable to connect, trying again\n");
            return;
        }
    }
    size_t graph_amount = graphs.size();
    sock->write((char*)&graph_amount, sizeof(graph_amount));
    QDataStream outp_stream(sock);
    for(auto &x: graphs) {
        outp_stream<<x.getDataFromByteArr();
    }
    sock->disconnectFromServer();
    sock->close();
}
