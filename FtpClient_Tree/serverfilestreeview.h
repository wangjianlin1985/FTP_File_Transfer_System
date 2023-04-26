#ifndef SERVERFILESTREEVIEW_H
#define SERVERFILESTREEVIEW_H

#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QHash>
#include <QFile>
#include <QMessageBox>
#include <QtFtp/qurlinfo.h>
#include <QProgressDialog>
#include <QDebug>
#include "FileNameWidget.h"
#include "math.h"

#define ValueMax_B    pow(1024,1)
#define ValueMax_KB  pow(1024,2)
#define ValueMax_MB pow(1024,3)
#define ValueMax_G    pow(1024,4)

class ServerFilesTreeView : public QWidget
{
    Q_OBJECT
public:
    explicit ServerFilesTreeView(QWidget *parent = 0);
    ~ServerFilesTreeView();
    void updatePath(QString path);//更新所在ftp文件路径
    void clearItems();//清除文件树子节点
    void updateTreeWidget();//更新树状文件显示
private:
    void initialize();//初始化树样式
    void release();//清除组件
    void setUserDefineStyle();//下载等功能函数按钮创建
    void setWinLayout();//窗口样式
    void sigAndSlotConnect();//信号槽函数绑定
    QString formatSize(qint64 size);//ftp通信网速单位自适应


signals:
    void sigcdDir(QString name);//
    void sigGetFile(QString fileName);//
    void sigDeleteFile(QString fileName,int type);//
    void sigMkdir(QString fileName);//
    void sigRename(QString oldname,QString newName);//
    void sigUpFile(QString fileName,QString path);//
public slots:
    void slotAddToList(const QUrlInfo &urlInfo);//文件树结构添加子节点槽函数
    void slotEnterDir(QTreeWidgetItem *item, int);//点击进入文件夹槽函数
    void slotCdToParent();//按键返回上一级槽函数
    void slotDownloadFile();//按键下载槽函数
    void slotDeleteFile();//按键删除槽函数
    void slotMkdir();//按键创建文件夹函数
    void slotRename();//按键重命名函数
    void slotEnableDownloadButton();//下载进度
    void slotUpFile();//按键上传函数
public:
    QTreeWidget *p_fileList;
    QHash<QString, bool> m_isDirectory;
    QProgressDialog *p_progressDialog;
private:
    QPushButton *p_pushButton[7];
    QVBoxLayout *p_VlayoutRight,*p_VayoutMaint;
    QHBoxLayout *p_HlayoutLeft,*p_HlayoutBottom,*p_HlayoutTop;

    QFrame *p_frameLeft,*p_frameTop;
    QString m_currentPath;
    QLabel *p_label_path;

    FileNameWidget * m_filename = nullptr;
};

#endif // SERVERFILESTREEVIEW_H
