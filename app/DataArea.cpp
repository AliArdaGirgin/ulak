#include <QWidget>
#include <QTextEdit>
#include <QGridLayout>
#include <QDebug>
#include <QFileDialog>
#include <QTime>
#include <QTimer>
#include <QTabWidget>
#include <QVector>
#include <QMessageBox>
#include <ctime>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "DataArea.h"
#include "PortHandler.h"
#include "DataType.h"
#include "Conf.h"
#include "DataType.h"
#include "ProjectSettings.h"

DataArea::DataArea(PortHandler *pHandler,QWidget *parent):
    QWidget(parent),port_handler(pHandler){
    layout = new QGridLayout();
    tabbed = new QTabWidget();
    ascii  = new QTextEdit();
    ascii->setReadOnly(true);
    hex    = new QTextEdit();
    hex->setReadOnly(true);

    ascii_index = tabbed->addTab(ascii, VIEW_TYPE_ASCII_NAME);
    hex_index   = tabbed->addTab(hex, VIEW_TYPE_HEX_NAME);
    tabbed->setCurrentIndex(
        static_cast<int>(ProjectSettings::getDefaultViewType())
    );
    current_index = tabbed->currentIndex();

    // Start timer to update timestamp
    timestampChanged = false;
    timer  = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(run()));
    timer->start(DATA_AREA_TIMESTAMP_UPDATE_PERIOD);

    layout->addWidget(tabbed,0,1);
    layout->setColumnStretch(1,10);
    layout->setSizeConstraint(QLayout::SetMaximumSize);
    setLayout(layout);

    connect(tabbed, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));

    time_prog_start = std::chrono::system_clock::now();
}

void DataArea::write(QByteArray data_in, DATA_TYPE dataType){
    TimestampedData dt;
    QString temp;

    // insetr new row, if first data or data type changed or timestamp changed
    if(!data.size() || data.back().type != dataType || timestampChanged){
        timestampChanged = false;
        dt.dt = data_in;
        dt.timestamp = lastTimestamp;
        dt.type = dataType;
        data.push_back(dt);

    // if not, append to last roe
    }else{
        data.back().dt.append(data_in);
    }

    //
    if(current_index == ascii_index){
        textFieldUpdate(ascii, [](QByteArray& ba){ return QString(ba);});
    }else if(current_index == hex_index){
        textFieldUpdate(hex, [](QByteArray& ba){return ba.toHex(' ');});
    }else{
        std::cout << "Error :" << __FILE__ << " " << __LINE__ << std::endl;
    }
}


void DataArea::clear(){
    data.clear();
    if(current_index == ascii_index)
        ascii->clear();
    else
        hex->clear();
}

void DataArea::save(){
    int dataSize = 0;
    if(current_index == ascii_index)
        dataSize = ascii->toPlainText().length();
    else
        dataSize = hex->toPlainText().length();

    if(dataSize == 0){
        QMessageBox *msg = new QMessageBox();
        msg->setText("No data to save!");
        msg->exec();
        return;
    }

    QString  file_name = QFileDialog::getSaveFileName(this,tr("Open File"),"","All Files(*)");
    if(file_name.isEmpty())
        return;
        
    QFile file(file_name);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        return;
    }
    if(current_index == ascii_index)
        file.write( ascii->toPlainText().toLocal8Bit() );
    else
        file.write( hex->toPlainText().toLocal8Bit() );
    file.close();
}

void DataArea::tabChanged(int tab){
    current_index = tab;
    if(current_index == ascii_index){
        textFieldUpdate(ascii, [](QByteArray& ba){ return QString(ba);});
    }else if(current_index == hex_index){
        textFieldUpdate(hex, [](QByteArray& ba){return ba.toHex(' ');});
    }else{
        std::cout << "Error :" << __FILE__ << " " << __LINE__ << std::endl;
    }

}

void DataArea::run(){
    lastTimestamp = getTimestamp();
    timestampChanged = true;
}

QString DataArea::getTimestamp(){
    QString ret;

    using namespace std::chrono;
    std::stringstream ss;

    // Holds date time
    std::time_t t = system_clock::to_time_t(system_clock::now());

    // Hold ms since
    time_point<system_clock> ms_now = system_clock::now();

    switch(ProjectSettings::getDefaultTimestampFormat()){
        case TIMESTAMP_FORMAT_TYPE::DATE:
            ss << std::put_time(std::localtime(&t), "%Y-%m-%d");
            ret = QString::fromStdString(ss.str());
            break;

        case TIMESTAMP_FORMAT_TYPE::TIME:
            ss << std::put_time(std::localtime(&t), "%H:%M:%S");
            ret = QString::fromStdString(ss.str());
            break;

        case TIMESTAMP_FORMAT_TYPE::DATE_TIME:
            ss << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S");
            ret = QString::fromStdString(ss.str());
            break;

        case TIMESTAMP_FORMAT_TYPE::TIME_MS:
            ss << std::put_time(std::localtime(&t), "%H:%M:%S");
            // add ms
            ss << "," << time_point_cast<milliseconds>(ms_now).time_since_epoch().count()%1000;
            ret = QString::fromStdString(ss.str());
            break;

        case TIMESTAMP_FORMAT_TYPE::DATE_TIME_MS:
            ss << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S");
            // add ms
            ss << "," << time_point_cast<milliseconds>(ms_now).time_since_epoch().count()%1000;
            ret = QString::fromStdString(ss.str());
            break;

        case TIMESTAMP_FORMAT_TYPE::MS_FROM_START:
            ss << duration_cast<milliseconds>(ms_now - time_prog_start).count();
            ret = QString::fromStdString(ss.str());
            break;

        case TIMESTAMP_FORMAT_TYPE::SEC_FROM_START:
            ss << duration<float>(ms_now - time_prog_start).count();
            ret = QString::fromStdString(ss.str());
        break;

        case TIMESTAMP_FORMAT_TYPE::MAX:
        default:
            break;
    }

    return ret;
}

void DataArea::textFieldUpdate(QTextEdit* te, std::function<QString(QByteArray&)> byteToStr){
    te->clear();
    QString temp;
    for(auto &m:data){
        temp.clear();

        // add type and timestamp
        if(m.type == DATA_TYPE::TX)
            temp.append("[TX] ");
        else
            temp.append("[RX] ");
        temp.append( m.timestamp );

        // write type and timestanp with colors
        if(m.type == DATA_TYPE::TX)
            te->setTextColor(Qt::red);
        else
            te->setTextColor(Qt::darkGreen);
        te->append(temp);
        te->setTextColor(Qt::black);

        // get text from lambda
        te->append(byteToStr(m.dt));
        te->append("\n");
    }
}

void DataArea::setCurrentTab(VIEW_TYPE tab){
    tabbed->setCurrentIndex(static_cast<int>(tab));
}
