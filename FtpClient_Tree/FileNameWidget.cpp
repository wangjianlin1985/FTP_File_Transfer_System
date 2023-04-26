#include "FileNameWidget.h"
#include "ui_FileNameWidget.h"

FileNameWidget::FileNameWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileNameWidget)
{
    ui->setupUi(this);
}

FileNameWidget::~FileNameWidget()
{
    delete ui;
}

void FileNameWidget::resetInfo()
{
    ui->lineEdit->clear();
    ui->lineEdit->setPlaceholderText("请输入文件名");
}

void FileNameWidget::setTitleName(bool isDir)
{
    m_isDir = isDir;
    if(isDir){
        ui->label->setText("新建文件夹");
    }
    else{
        ui->label->setText("重命名文件");
    }
}

void FileNameWidget::on_pushButtonsure_clicked()
{
    auto filename = ui->lineEdit->text();
    if(m_isDir){
        emit signalMakeDir(filename);
    }
    else{
        emit signalFileName(filename);
    }
    this->hide();

}

void FileNameWidget::on_pushButton_2_clicked()
{
    this->hide();
}
