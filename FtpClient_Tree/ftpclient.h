#ifndef FTPCLIENT_H
#define FTPCLIENT_H

#include <QWidget>
#include <QApplication>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QPushButton>
#include <QtFtp/qftp.h>
#include <QtFtp/qurlinfo.h>
#include <QtNetwork/QNetworkConfigurationManager>
#include <QtNetwork/QNetworkSession>
#include <QSettings>
#include <QUrl>
#include <QMessageBox>
#include "serverfilestreeview.h"

class FtpClient : public QWidget
{
    Q_OBJECT
public:
    FtpClient(QWidget *parent = 0);
    ~FtpClient();
private:
    void initialize();//初始化按键界面
    void release();//清除界面按键
    void setUserDefineStyle();//设置用户信息
    void setWinLayout();//设置窗口界面布局
    void sigAndSlotConnect();//qt信号和槽函数绑定
    void connectToFtp();//连接登录ftp
private slots:
    void slotConnected();//获取ip 用户名等连接ftp
    void slotDisConnected();//取消ftp连接
    void slotftpCommandFinished(int, bool error);//ftp连接是否成功判断函数
    void slotCdDir(QString name);//返回上一级函数
    void slotDownFile(QString fileName);//下载ftp文件函数
    void slotDeleteFile(QString fileName, int type);//删除ftp文件函数
    void slotMkdir(QString fileName);//创建文件夹函数
    void slotRename(QString oldname,QString newName);//重命名文件函数
    void slotUpFile(QString fileName,QString path);//上传文件函数
private:
    QLabel *p_FtpServerLabel;
    QLabel *p_PasswordLabel;
    QLabel *p_UserNameLabel;
    QLineEdit *p_FtpServerEdit;
    QLineEdit *p_PasswordEdit;
    QLineEdit *p_UserNameEdit;
    QHBoxLayout *p_HlayoutServer;
    QHBoxLayout *p_HlayoutPassword;
    QHBoxLayout *p_HlayoutUserName;
    QPushButton *p_ConnectButton;
    QPushButton *p_DisConnectButton;
    QHBoxLayout *p_HlayoutButton;
    QVBoxLayout *p_VlayoutMain;
    /*********************************/
    ServerFilesTreeView m_FileView;
    QFtp *p_ftp;
    QNetworkSession *p_networkSession;
    QNetworkConfigurationManager m_manager;
    QFile *p_file;
};

#endif // FTPCLIENT_H
