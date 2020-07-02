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
    ui->fileButton->setIcon(QIcon::fromTheme("folder"));
    ui->lineEdit->setReadOnly(true);
    ui->headLabel->setText("Pick a file with the coded morse sequence.");
    ui->computeButton->setHidden(true);
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
    std::ifstream inp_f(code_filename);
    if(!inp_f.is_open()) {
        QMessageBox::question(this, "Error!", "The file wasn't opened.", QMessageBox::Ok);
        return;

    }
    getline(inp_f, code_string);                                                                //The code file should only contain one string.
    if(code_string.find_first_not_of(".-")!=std::string::npos) {
        code_string = "";
        ui->label->setText("ERROR:\nFile contains non-morse symbols.\nPlease, pick andother file.");
        ui->computeButton->setHidden(true);
        return ;
    }
    if(code_string.empty()) {
        ui->label->setText("ERROR:\nThe file is empty.");
        ui->computeButton->setHidden(true);
        return;
    }
    ui->label->clear();
    ui->headLabel->setText("Now you're able to compute the strings.");
    ui->computeButton->setHidden(false);
    ui->lineEdit->setText(QString(code_filename.c_str()));

}

static std::string buff_vec_str_outp(std::ofstream& ofs, const size_t buffer_size, const QVector<std::string>& outp_vector)
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
    char buff[1000];
    getcwd(buff, sizeof(buff));
    return buff;
}

void MainWindow::on_computeButton_clicked()
{
    if(code_string.size()>25) {
        auto answ = QMessageBox::question(this, "Warning!", "The chosen sequence is longer than 25 symbols.\n"
                                                            "It might cause lags due to enormous memory consuption\n"
                                                            "Do you wish to proceed with the current sequence?",QMessageBox::No| QMessageBox::Yes);
        if(answ == QMessageBox::No ) {
            return;
        }
    }
    ui->headLabel->setText("Processing...");
    std::string save_filename(code_filename ,code_filename.find_last_of("/")+1);            //Gettin a filename from the absolute path
    save_filename.insert(0, "decoded_");
    std::ofstream out_f(save_filename);
    if(!out_f.is_open()) {
        QMessageBox::question(this, "Error!","The output file wasn't opened/generated for some reason.\n"
                                             "Check the setting of the folder.", QMessageBox::Ok);
        ui->headLabel->setText("File wasnt's opened. Sad!");
        return;
    }
    MorseDecoder Decoder(m_table, code_string);
    auto run_t1 = std::chrono::high_resolution_clock::now();
    QVector<std::string>& valids = Decoder.result();
    auto run_t2 = std::chrono::high_resolution_clock::now();
    ui->headLabel->setText("Done!");
    ui->label_2->setText(QString::number( valids.size()) +" lines were generated.");
    auto save_t1 = std::chrono::high_resolution_clock::now();
    ui->label->setText( ("The decoded file was saved at:\n" +\
                         (buff_vec_str_outp(out_f, 16000,valids).c_str() +('/'+ save_filename))).c_str());
    auto save_t2 = std::chrono::high_resolution_clock::now();
    auto duration_save = std::chrono::duration_cast<std::chrono::microseconds>( save_t2 - save_t1 ).count();
    auto duration_run = std::chrono::duration_cast<std::chrono::microseconds>( run_t2 - run_t1 ).count();
    qDebug() << duration_run<<" msec was the computation running\n";
    qDebug() << duration_save<<" msec was the data begin saved\n";
    out_f.close();
}
