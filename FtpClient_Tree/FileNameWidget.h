#ifndef FILENAMEWIDGET_H
#define FILENAMEWIDGET_H

#include <QWidget>

namespace Ui {
class FileNameWidget;
}

class FileNameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FileNameWidget(QWidget *parent = nullptr);
    ~FileNameWidget();

    void resetInfo();

    void setTitleName(bool isDir);//设置文件名

signals:
    void signalFileName(QString filename);
    void signalMakeDir(QString dirname);
private slots:
    void on_pushButtonsure_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::FileNameWidget *ui;
    bool m_isDir;
};

#endif // FILENAMEWIDGET_H
