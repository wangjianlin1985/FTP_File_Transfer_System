#include "ftpclient.h"

FtpClient::FtpClient(QWidget *parent)
    : QWidget(parent)
{
    initialize();
    setUserDefineStyle();
    setWinLayout();
    sigAndSlotConnect();
}

FtpClient::~FtpClient()
{
    release();
}

void FtpClient::initialize()
{
    p_ftp = 0;
    p_FtpServerLabel = new QLabel("Ftp Server:");
    p_PasswordLabel = new QLabel("Password:");
    p_UserNameLabel = new QLabel("Login Name:");

    p_FtpServerEdit = new QLineEdit();
    p_PasswordEdit = new QLineEdit();
    p_UserNameEdit = new QLineEdit();

    p_ConnectButton = new QPushButton("Connect");
    p_DisConnectButton = new QPushButton("DisConnect");

    p_HlayoutServer = new QHBoxLayout();
    p_HlayoutPassword = new QHBoxLayout();
    p_HlayoutUserName = new QHBoxLayout();
    p_HlayoutButton = new QHBoxLayout();

    p_VlayoutMain = new QVBoxLayout();
}

void FtpClient::release()
{
    delete p_FtpServerLabel;
    delete p_FtpServerEdit;
    delete p_PasswordLabel;
    delete p_PasswordEdit;
    delete p_UserNameLabel;
    delete p_UserNameEdit;
    delete p_ConnectButton;
    delete p_DisConnectButton;
    delete p_HlayoutServer;
    delete p_HlayoutPassword;
    delete p_HlayoutUserName;
    delete p_HlayoutButton;
    delete p_VlayoutMain;
}

void FtpClient::setUserDefineStyle()
{
    p_DisConnectButton->setEnabled(false);
    p_FtpServerEdit->setText("192.168.1.3");
    p_UserNameEdit->setText("test");
    p_PasswordEdit->setText("test");

    p_FtpServerLabel->setFixedWidth(90);
    p_PasswordLabel->setFixedWidth(90);
    p_UserNameLabel->setFixedWidth(90);

    p_HlayoutServer->setSpacing(0);
    p_HlayoutServer->setContentsMargins(0,0,0,0);

    p_HlayoutPassword->setSpacing(0);
    p_HlayoutPassword->setContentsMargins(0,0,0,0);

    p_HlayoutUserName->setSpacing(0);
    p_HlayoutUserName->setContentsMargins(0,0,0,0);

    p_HlayoutButton->setSpacing(0);
    p_HlayoutButton->setContentsMargins(0,0,0,0);

    p_HlayoutButton->setSpacing(0);
    resize(300,200);
}

void FtpClient::setWinLayout()
{
    p_HlayoutServer->addWidget(p_FtpServerLabel);
    p_HlayoutServer->addWidget(p_FtpServerEdit);

    p_HlayoutPassword->addWidget(p_PasswordLabel);
    p_HlayoutPassword->addWidget(p_PasswordEdit);

    p_HlayoutUserName->addWidget(p_UserNameLabel);
    p_HlayoutUserName->addWidget(p_UserNameEdit);

    p_HlayoutButton->addStretch();
    p_HlayoutButton->addWidget(p_ConnectButton);
    p_HlayoutButton->addStretch();
    p_HlayoutButton->addWidget(p_DisConnectButton);
    p_HlayoutButton->addStretch();

    p_VlayoutMain->addLayout(p_HlayoutServer);
    p_VlayoutMain->addLayout(p_HlayoutPassword);
    p_VlayoutMain->addLayout(p_HlayoutUserName);
    p_VlayoutMain->addLayout(p_HlayoutButton);

    setLayout(p_VlayoutMain);
}

void FtpClient::sigAndSlotConnect()
{
    connect(p_ConnectButton,SIGNAL(clicked()),this,SLOT(slotConnected()));
    connect(p_DisConnectButton,SIGNAL(clicked()),this,SLOT(slotDisConnected()));
    connect(&m_FileView,SIGNAL(sigcdDir(QString)),this,SLOT(slotCdDir(QString)));
    connect(&m_FileView,SIGNAL(sigGetFile(QString)),this,SLOT(slotDownFile(QString)));
    connect(&m_FileView,SIGNAL(sigUpFile(QString,QString)),this,SLOT(slotUpFile(QString,QString)));
    connect(&m_FileView,SIGNAL(sigDeleteFile(QString,int)),this,SLOT(slotDeleteFile(QString,int)));
    connect(&m_FileView,SIGNAL(sigMkdir(QString)),this,SLOT(slotMkdir(QString)));
    connect(&m_FileView,SIGNAL(sigRename(QString,QString)),this,SLOT(slotRename(QString,QString)));
}

void FtpClient::connectToFtp()
{
    p_ftp = new QFtp(this);
    connect(p_ftp, SIGNAL(commandFinished(int,bool)),this, SLOT(slotftpCommandFinished(int,bool)));
    connect(p_ftp, SIGNAL(listInfo(const QUrlInfo &)),&m_FileView, SLOT(slotAddToList(const QUrlInfo &)));


    QUrl url(p_FtpServerEdit->text());
    if (!url.isValid() || url.scheme().toLower() != QLatin1String("ftp"))
    {
        p_ftp->connectToHost(p_FtpServerEdit->text(), 21);
        p_ftp->login(p_UserNameEdit->text(),p_PasswordEdit->text());
    }
    else
    {
        p_ftp->connectToHost(url.host(), url.port(21));

        if (!url.userName().isEmpty())
        {
            p_ftp->login(QUrl::fromPercentEncoding(url.userName().toLatin1()), url.password());
        }
        else
        {
            p_ftp->login();
        }
        if (!url.path().isEmpty())
        {
            p_ftp->cd(url.path());
        }
    }
}

void FtpClient::slotConnected()
{

    if (!p_networkSession)
    {
        if (m_manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired)
        {
            if (!p_networkSession)
            {
                QSettings settings(QSettings::UserScope, QLatin1String("Trolltech"));
                settings.beginGroup(QLatin1String("QtNetwork"));
                const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
                settings.endGroup();


                QNetworkConfiguration config = m_manager.configurationFromIdentifier(id);
                if ((config.state() & QNetworkConfiguration::Discovered) !=
                        QNetworkConfiguration::Discovered) {
                    config = m_manager.defaultConfiguration();
                }

                p_networkSession = new QNetworkSession(config, this);
                connect(p_networkSession, SIGNAL(opened()), this, SLOT(connectToFtp()));
                connect(p_networkSession, SIGNAL(error(QNetworkSession::SessionError)), this, SLOT(slotEnableConnectButton()));
            }
            p_networkSession->open();
            return;
        }
    }
    connectToFtp();
}

void FtpClient::slotDisConnected()
{
    m_FileView.clearItems();
    if (p_ftp)
    {
        p_ftp->abort();
        p_ftp->deleteLater();
        p_ftp = 0;
        return;
    }
}

void FtpClient::slotftpCommandFinished(int, bool error)
{
    if (p_ftp->currentCommand() == QFtp::ConnectToHost)
    {
        if (error)
        {
            QMessageBox::information(this, tr("FTP"),
                                     tr("Unable to connect to the FTP server "
                                        "at %1. Please check that the host "
                                        "name is correct.").arg(p_FtpServerEdit->text()));
            slotConnected();
            return;
        }
        QApplication::setActiveWindow(&m_FileView);
        m_FileView.activateWindow();
        m_FileView.show();
        p_ConnectButton->setEnabled(false);
        p_DisConnectButton->setEnabled(true);
        return;
    }
    if (p_ftp->currentCommand() == QFtp::Login)
    {
        p_ftp->list();
    }
    if (p_ftp->currentCommand() == QFtp::Get)
    {
        if (error)
        {
            p_file->close();
            p_file->remove();
        }
        else
        {
            p_file->close();
        }
        delete p_file;
        m_FileView.slotEnableDownloadButton();
        m_FileView.p_progressDialog->hide();
    }
    else if (p_ftp->currentCommand() == QFtp::List)
    {
        if(m_FileView.m_isDirectory.isEmpty())
        {
            m_FileView.p_fileList->addTopLevelItem(new QTreeWidgetItem(QStringList() << tr("<empty>")));
            m_FileView.p_fileList->setEnabled(false);
        }
    }
}

void FtpClient::slotCdDir(QString name)
{
    p_ftp->cd(name);
    p_ftp->list();
}

void FtpClient::slotDownFile(QString fileName)
{
    p_file = new QFile(fileName);
    if (!p_file->open(QIODevice::WriteOnly))
    {
        QMessageBox::information(this, tr("FTP"),
                                 tr("Unable to save the file %1: %2")
                                 .arg(fileName).arg(p_file->errorString()));
        delete p_file;
        return;
    }
    p_ftp->get(m_FileView.p_fileList->currentItem()->text(0), p_file);
}

void FtpClient::slotDeleteFile(QString fileName,int type)
{
    switch(type)
    {
    case 0: //删除文件夹
        p_ftp->rmdir(fileName);
        break;
    case 1://删除文件
        p_ftp->remove(fileName);
        break;
    }
}

void FtpClient::slotMkdir(QString fileName)
{
    p_ftp->mkdir(fileName);
    m_FileView.updateTreeWidget();
    p_ftp->list();
}

void FtpClient::slotRename(QString oldname,QString newName)
{
    p_ftp->rename(oldname,newName);
    m_FileView.updateTreeWidget();
    p_ftp->list();
}
#include <QDebug>
void FtpClient::slotUpFile(QString fileName,QString path)
{
   // QString filepatj = "D:\\wewe.txt";
    if (fileName.isEmpty()) return;
    QFile file;
    file.setFileName(path);

    if (!file.open(QIODevice::ReadOnly)) return;

    QByteArray data = file.readAll();
    p_ftp->put(data, fileName);
    m_FileView.updateTreeWidget();
    p_ftp->list();
}
