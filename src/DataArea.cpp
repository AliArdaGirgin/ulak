#include <QWidget>
#include <QTextEdit>
#include <QGridLayout>
#include <QDebug>
#include <QFileDialog>
#include <iostream>
#include <QTime>
#include <QTimer>
#include <QTabWidget>
#include <QVector>
#include <QMessageBox>
#include "DataArea.h"
#include "PortHandler.h"
#include "DataType.h"
#include <string>


const int DataArea::TIMER_RES=100; // in ms

DataArea::DataArea(PortHandler *pHandler,QWidget *parent):
    QWidget(parent),port_handler(pHandler){
    layout = new QGridLayout();
    tabbed = new QTabWidget();
    ascii  = new QTextEdit();
    hex    = new QTextEdit();

    ascii_index = tabbed->addTab(ascii, "ASCII");
    hex_index   = tabbed->addTab(hex, "HEX");
    current_index = ascii_index;

    timestampChanged = false;
    timer  = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(run()));
    timer->start(TIMER_RES);

    layout->addWidget(tabbed,0,1);
    layout->setColumnStretch(1,10);
    layout->setSizeConstraint(QLayout::SetMaximumSize);
    setLayout(layout);

    connect(tabbed, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
}

void DataArea::write(QByteArray &data_in, DataType dataType){
    TimestampedData dt;
    QString temp;
    if(data.end()->type != dataType || timestampChanged){
        dt.dt = data_in;
        dt.timestamp = lastTimestamp;
        dt.type = dataType;
        data.push_back(dt);

    }else{
        data.end()->dt.append(data_in);
    }
    if(tabbed->currentIndex() == ascii_index){
        ascii->clear();
        for(auto &m:data){
            temp.clear();
            if(m.type == DataType::TX)
                temp.append("[TX] ");
            else
                temp.append("[RX] ");
            temp.append( m.timestamp );
            temp.append(" ");
            temp.append( QString(m.dt) );

            ascii->append(temp);
        }
    }else{
        hex->clear();
        for(auto &m:data){
            temp.clear();
            if(m.type == DataType::TX)
                temp.append("[TX] ");
            else
                temp.append("[RX] ");
            temp.append( m.timestamp );
            temp.append(" ");
            temp.append( m.dt.toHex(' ') );

            hex->append(temp);
        }
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

void DataArea::tabChanged(int index){
    QString temp;
    current_index = index;
    if(index == ascii_index){
        ascii->clear();
        for(auto &m:data){
            temp.clear();
            if(m.type == DataType::TX)
                temp.append("[TX] ");
            else
                temp.append("[RX] ");
            temp.append( m.timestamp );
            temp.append(" ");
            temp.append( QString(m.dt) );

            ascii->append(temp);
        }
    }else if(index == hex_index){
        hex->clear();
        for(auto &m:data){
            temp.clear();
            if(m.type == DataType::TX)
                temp.append("[TX] ");
            else
                temp.append("[RX] ");
            temp.append( m.timestamp );
            temp.append(" ");
            temp.append( m.dt.toHex(' ') );

            hex->append(temp);
        }
    }else{
        std::cout << "Error :" << __FILE__ << " " << __LINE__ << std::endl;
    }

}

void DataArea::run(){
    lastTimestamp = QTime::currentTime().toString();
    timestampChanged = true;
}
