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
#include<QThread>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->visualButton->setHidden(true);
    ui->fileButton->setIcon(QIcon::fromTheme("folder"));
    ui->lineEdit->setReadOnly(true);
    ui->headLabel->setText("Выберите файл с последовательностями кодов морзе");
    //testing
    ui->logBrowser->setHidden(true);
    //\testing
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
        QMessageBox::question(this, "Файл таблицы морзе \'morse\' не обнаружен", "Директория может содержать файл \" morse\" \n"
                                                                                "формата {символ} {код} для каждой строки.\n Т.к. он отсутствует, будет использована стандартная таблица.",QMessageBox::Ok);
        m_table = MorseDecoder::default_morse_table;
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

}

bool MainWindow::set_code_strings()
{
    code_strings.clear();
    std::ifstream fs(code_filename);
    if(!fs.is_open()) {
        QMessageBox::question(this, "Ошибка!", "Не удалось открыть файл", QMessageBox::Ok);
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
            ui->logBrowser->append(QString("Внимание: Строка %1 символ %2 не является частью кода морзе. Строка %1 пропускается. ").arg(counter +1).arg(pos +1));
        } else if (temp.size()!=0) {
            code_strings.push_back(temp);
        }
        ++counter;
    }
    if(counter ==0) {
        ui->label->setText("Ошибка:\nФайл пуст.");
        ok = false;
    } else if(!ui->logBrowser->toPlainText().isEmpty()) {
        ui->label->setText("Некоторые строки были пропущенны. Проверьте логи.");
        if(code_strings.isEmpty()) {
            ui->label->setText("Ошибка:\nНе обнаружено подходящих строк. Проверьте логи");
            ok = false;
        }
    } else if (ok){
        ui->label->clear();
    }
    fs.close();
    return ok;
}

static std::ofstream& buff_vec_str_outp(std::ofstream& ofs, const size_t buffer_size, const QVector<std::string>& outp_vector)
// буферезированный вывод в файл
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

    if(!set_code_strings()) {           //  считывает строки из файла ввода, выбирает подходящие из них
        ui->visualButton->setHidden(true);
        return;
    }
    std::string::size_type slash_pos = code_filename.find_last_of("/");
    std::string save_filename("decoded_" +std::string(code_filename ,slash_pos+1));
    std::ofstream out_f(save_filename);
    size_t generated_count = 0;
    std::string full_filename(std::string(code_filename,0,slash_pos+1) + save_filename);
    std::string delim(80, '=');

    graphs.clear();
    if(!out_f.is_open()) {
        QMessageBox::question(this, "Ошибка!","По неизвестной причине файл вывода не был создан/открыт.\n"
                                             "Проверьте настройки доступа папки.", QMessageBox::Ok);
        ui->headLabel->setText("К сожалению не удалось открыть файл.");
        return;
    }
    for(int i =0; i!=code_strings.size(); ++i) {
        if(i) {
            out_f<<'\n'<<delim<<'\n';
        }
        std::string &code_string = code_strings[i];
        out_f<<"String "<< i<<'('<<code_string<<"):\n";
        if(code_string.size()>25) {
            auto answ = QMessageBox::question(this, "Предупреждение!",QString("Строка %1 содержит более 25 знаков.\n"
                                                                "Это может привести к значительному потреблению памяти\n"
                                                                "Пропустить строку?").arg(i+1),QMessageBox::No| QMessageBox::Yes);
            if(answ == QMessageBox::Yes ) {
                continue;
            }
        }
        ui->headLabel->setText("Обрабатывается...");

        MorseDecoder Decoder(m_table, code_string);

        auto run_t1 = std::chrono::high_resolution_clock::now();
        QVector<std::string>& valids = Decoder.result();
        auto run_t2 = std::chrono::high_resolution_clock::now();

        ui->headLabel->setText("Готово!");
        generated_count+=valids.size();

        auto save_t1 = std::chrono::high_resolution_clock::now();
        buff_vec_str_outp(out_f, 16000,valids);
        auto save_t2 = std::chrono::high_resolution_clock::now();

        ui->logBrowser->append( QString("%1 строк создано из (%2), %3-я считаная строка").arg(valids.size()).arg(code_string.c_str()).arg(i +1));
        auto duration_save = std::chrono::duration_cast<std::chrono::microseconds>( save_t2 - save_t1 ).count();
        auto duration_run = std::chrono::duration_cast<std::chrono::microseconds>( run_t2 - run_t1 ).count();
        out_f<<"computation time (msec):"<<duration_run<<"\tsave time (msec):"<<duration_save<<'\n';

        if(code_string.size() <15) {
            graphs.push_back({code_string, valids});        // подходящие строки для визуализации
        }
    }
    ui->label_2->setText(QString::number(generated_count) +" строк было сгенерировано.\n" +
                         "Валидные строки сохранены в :\n" + QString::fromStdString(full_filename));
    out_f.close();
    ui->visualButton->setHidden(false);
}


void MainWindow::on_logButton_clicked()     //TODO: добавить изменение size policy у vertivalSpacer (а как?)
{
    if(ui->logButton->arrowType() == Qt::ArrowType::RightArrow) {
        ui->logButton->setArrowType(Qt::ArrowType::LeftArrow);
        ui->logBrowser->setHidden(false);
        qDebug()<<ui->verticalSpacer->sizePolicy();
        ui->verticalSpacer->changeSize(0,0,QSizePolicy::Fixed, QSizePolicy::Fixed);
        qDebug()<<ui->verticalSpacer->sizePolicy();
        ui->verticalLayout->invalidate();
    } else {
        ui->logButton->setArrowType(Qt::ArrowType::RightArrow);
        ui->logBrowser->setHidden(true);
        ui->verticalSpacer->changeSize(20,40, QSizePolicy::Expanding, QSizePolicy::Expanding);
        qDebug()<<ui->verticalSpacer->sizePolicy();
    }
}

void MainWindow::on_visualButton_clicked()
{
    QLocalSocket *sock =new QLocalSocket(this);
    sock->connectToServer("UniqueServerName");
    if(!sock->waitForConnected(1000000)) {
        ui->logBrowser->append("Ошибка подключения:" +sock->errorString());
        ui->logBrowser->append("Не удалось подключится, завершение операции.\n");
        ui->label->setText("Не удалось подключится к серверу");
        return;
    }
    qDebug()<<"sock desk: "<<sock->socketDescriptor();
    ui->logBrowser->append(QString("\nСокет %1 подключен к серверу %2\n").arg(sock->socketDescriptor()).arg(sock->serverName()));
    int graph_size = graphs.size();
    QByteArray send_buff;
    QDataStream send_str(&send_buff, QIODevice::WriteOnly);
    for(auto &x: graphs) {
        send_str<<x.getData();
        ui->logBrowser->append(QString::fromStdString("Строка "+x.getData().morse_line) + " добавлена в буфер отправки");
    }
    int passed_size = send_buff.size();
    ui->logBrowser->append("Байт к отправке: "+QString::number(passed_size));
    sock->write((char*)&passed_size, sizeof(int) );
    //testing
    sock->flush();
    QThread::sleep(1);
    //\testing
    sock->write((char*)&graph_size,sizeof(graph_size));
    sock->write(send_buff);
    ui->label->setText(QString::number(graph_size) + "  строк отправлено.");
    connect(sock, &QLocalSocket::bytesWritten, [this](int b){ui->logBrowser->append(QString::number(b)+ " байт отправлено.");});
    connect(sock, &QLocalSocket::disconnected, sock, &QObject::deleteLater);
    connect(sock, &QObject::destroyed,[this](){ ui->logBrowser->append("Сокет отключен и удален\n");});
}
