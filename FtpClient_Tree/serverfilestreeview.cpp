#include "serverfilestreeview.h"

ServerFilesTreeView::ServerFilesTreeView(QWidget *parent) :
    QWidget(parent)
{
    initialize();
    setUserDefineStyle();
    setWinLayout();
    sigAndSlotConnect();
}

ServerFilesTreeView::~ServerFilesTreeView()
{
    release();
}

void ServerFilesTreeView::updatePath(QString path)
{
    QString CurrentPath = QString("%1%2/%3").arg("Path=").arg(m_currentPath).arg(path);
    //p_label_path->setText(CurrentPath);
}

void ServerFilesTreeView::clearItems()
{
    while(p_fileList->topLevelItemCount() > 0)
    {
        delete p_fileList->topLevelItem(0);
    }
}

void ServerFilesTreeView::initialize()
{
    m_filename = new FileNameWidget();
    m_filename->hide();

    p_label_path = new QLabel();
    p_label_path->setFixedHeight(30);
    for(int i = 0; i< 7;i++)
    {
        p_pushButton[i] = new QPushButton();
    }
    p_frameLeft = new QFrame();
    p_frameTop = new QFrame();
    p_VlayoutRight = new QVBoxLayout();
    p_VayoutMaint = new QVBoxLayout();
    p_HlayoutLeft = new QHBoxLayout();
    p_HlayoutBottom = new QHBoxLayout();
    p_HlayoutTop = new QHBoxLayout();

    p_fileList = new QTreeWidget();
    p_fileList->setStyleSheet("QTreeView::item::selected{background-color:rgb(130, 255, 241);} QTreeView::item::hover{background-color:green;}");
    p_fileList->setEnabled(false);
    p_fileList->setRootIsDecorated(false);
    p_fileList->setHeaderLabels(QStringList() << tr("Name") << tr("Size")<< tr("Date Modifiled"));
    p_fileList->header()->setStretchLastSection(true);
    p_progressDialog = new QProgressDialog(this);
}

void ServerFilesTreeView::release()
{
    delete p_label_path;
    delete p_progressDialog;
    for(int i = 0; i< 7;i++)
    {
        delete p_pushButton[i];
    }
    delete p_fileList;
    delete p_VlayoutRight;
    delete p_HlayoutLeft;
    delete p_frameTop;
    delete p_frameLeft;
    delete p_HlayoutTop;
    delete p_HlayoutBottom;
    delete p_VayoutMaint;
}

void ServerFilesTreeView::setUserDefineStyle()
{
    p_pushButton[0]->setText("下载");
    p_pushButton[1]->setText("复制");
    p_pushButton[2]->setText("上传");
    p_pushButton[3]->setText("重命名");
    p_pushButton[4]->setText("删除");
    p_pushButton[5]->setText("返回上级");
    p_pushButton[6]->setText("新建文件夹");

    p_HlayoutLeft->setSpacing(0);
    p_HlayoutLeft->setContentsMargins(0,0,0,0);

    p_HlayoutLeft->setSpacing(2);
    p_HlayoutLeft->setContentsMargins(2,2,2,2);

    p_HlayoutTop->setSpacing(0);
    p_HlayoutTop->setContentsMargins(2,2,2,2);
    resize(800,600);
}

void ServerFilesTreeView::setWinLayout()
{
    p_HlayoutTop->addWidget(p_label_path);
    p_frameTop->setLayout(p_HlayoutTop);

    p_VlayoutRight->addWidget(p_pushButton[0]);
    p_VlayoutRight->addWidget(p_pushButton[1]);
    p_VlayoutRight->addWidget(p_pushButton[2]);
    p_VlayoutRight->addWidget(p_pushButton[3]);
    p_VlayoutRight->addWidget(p_pushButton[4]);
    p_VlayoutRight->addWidget(p_pushButton[5]);
    p_VlayoutRight->addWidget(p_pushButton[6]);

    p_HlayoutLeft->addWidget(p_fileList);
    p_frameLeft->setLayout(p_HlayoutLeft);

    p_HlayoutBottom->addWidget(p_frameLeft,9);
    p_HlayoutBottom->addLayout(p_VlayoutRight,1);

    p_VayoutMaint->addWidget(p_frameTop);
    p_VayoutMaint->addLayout(p_HlayoutBottom);
    setLayout(p_VayoutMaint);
}

void ServerFilesTreeView::sigAndSlotConnect()
{
    connect(p_fileList, SIGNAL(itemActivated(QTreeWidgetItem*,int)),this, SLOT(slotEnterDir(QTreeWidgetItem*,int)));
    connect(p_fileList, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),this, SLOT(slotEnableDownloadButton()));
    connect(p_pushButton[5],SIGNAL(clicked()),this,SLOT(slotCdToParent()));
    connect(p_pushButton[0],SIGNAL(clicked()),this,SLOT(slotDownloadFile()));
    connect(p_pushButton[4],SIGNAL(clicked()),this,SLOT(slotDeleteFile()));
    connect(p_pushButton[6],SIGNAL(clicked()),this,SLOT(slotMkdir()));
    connect(p_pushButton[3],SIGNAL(clicked()),this,SLOT(slotRename()));
    connect(p_pushButton[2],SIGNAL(clicked()),this,SLOT(slotUpFile()));
    connect(m_filename,&FileNameWidget::signalFileName,this,[=](QString filename){
        QString oldName = p_fileList->currentItem()->text(0);
        m_filename->hide();
        emit sigRename(oldName,filename);
    });
    connect(m_filename,&FileNameWidget::signalMakeDir,this,[=](QString filename){
        emit sigMkdir(m_currentPath + "/"+filename);
        m_filename->hide();
    });
}

QString ServerFilesTreeView::formatSize(qint64 size)
{
    QString result = "0";
    double value = 0.00;
    if(size > 0 && size < ValueMax_B)
    {
        result = QString("%1").arg(size)+QString("B");
    }
    else if((size >= ValueMax_B) && (size < ValueMax_KB))
    {
        value = size/ValueMax_B;
        result = QString::number(value,'f',1)+QString("KB");
    }
    else if((size >= ValueMax_KB) && (size < ValueMax_MB))
    {
        value = size/ValueMax_KB;
        result = QString::number(value,'f',1)+QString("MB");
    }
    else if((size >= ValueMax_MB) && (size < ValueMax_G))
    {
        value = size/ValueMax_MB;
        result = QString::number(value,'f',1)+QString("G");
    }
    return result;
}

void ServerFilesTreeView::updateTreeWidget()
{
    p_fileList->clear();
}

void ServerFilesTreeView::slotAddToList(const QUrlInfo &urlInfo)
{

    QTreeWidgetItem *item = new QTreeWidgetItem;
    item->setText(0, urlInfo.name());
    if(urlInfo.isDir())
    {
        item->setText(1, QString(""));
    }
    else
    {
        item->setText(1, formatSize(urlInfo.size()));
    }
    item->setText(2, urlInfo.lastModified().toString("MMM dd yyyy"));

    QPixmap pixmap(urlInfo.isDir() ? ":/images/dir.png" : ":/images/file.png");
    item->setIcon(0, pixmap);

    m_isDirectory[urlInfo.name()] = urlInfo.isDir();
    if(urlInfo.isDir())
    {
        p_fileList->insertTopLevelItem(0,item);
    }
    else if(urlInfo.isFile())
    {
        int index = p_fileList->model()->rowCount();
        p_fileList->insertTopLevelItem(index,item);
    }

    if (!p_fileList->currentItem())
    {
        p_fileList->setCurrentItem(p_fileList->topLevelItem(0));
        p_fileList->setEnabled(true);
    }
    updatePath(p_fileList->currentItem()->text(0));
}

void ServerFilesTreeView::slotEnterDir(QTreeWidgetItem *item, int /*column*/)
{
    QString name = item->text(0);
    if (m_isDirectory.value(name))
    {
        p_fileList->clear();
        m_isDirectory.clear();
        m_currentPath += '/';
        m_currentPath += name;
        emit sigcdDir(name);
        return;
    }
}

void ServerFilesTreeView::slotCdToParent()
{
    p_fileList->clear();
    m_isDirectory.clear();
    m_currentPath = m_currentPath.left(m_currentPath.lastIndexOf('/'));
    if (m_currentPath.isEmpty())
    {
        updatePath("");
        emit sigcdDir("/");
    }
    else
    {
        updatePath(m_currentPath);
        emit sigcdDir(m_currentPath);
    }
}
#include <QFileDialog>
#include <QDebug>
void ServerFilesTreeView::slotDownloadFile()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("选择下载路径"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    //QString filepath = QFileDialog::getExistingDirectory();
    QString fileName = dir+"/"+p_fileList->currentItem()->text(0);
    if (QFile::exists(fileName))
    {
        QMessageBox::information(this, tr("FTP"),
                                 tr("There already exists a file called %1 in "
                                    "the current directory.")
                                 .arg(fileName));
        return;
    }
    qDebug()<<"down path"<<fileName;
    emit sigGetFile(fileName);
    p_progressDialog->setLabelText(tr("Downloading %1...").arg(fileName));
    p_progressDialog->exec();
}

void ServerFilesTreeView::slotDeleteFile()
{
    QString fileName = p_fileList->currentItem()->text(0);
    if(m_isDirectory.value(fileName))
    {
        fileName = m_currentPath + "/" +fileName;
        emit sigDeleteFile(fileName,0);
    }
    else
    {
        emit sigDeleteFile(fileName,1);
    }
    delete p_fileList->currentItem();
}

void ServerFilesTreeView::slotMkdir()
{
    m_filename->setTitleName(true);
    m_filename->resetInfo();
    m_filename->show();
}

void ServerFilesTreeView::slotRename()
{
    m_filename->setTitleName(false);
    m_filename->resetInfo();
    m_filename->show();

}

void ServerFilesTreeView::slotEnableDownloadButton()
{
    QTreeWidgetItem *current = p_fileList->currentItem();
    if (current)
    {
        QString currentFile = current->text(0);
        p_pushButton[0]->setEnabled(!m_isDirectory.value(currentFile));
    }
    else
    {
        p_pushButton[0]->setEnabled(false);
    }
}
#include<QDir>

void ServerFilesTreeView::slotUpFile()
{
    QString dir = QFileDialog::getOpenFileName(this,tr("file"),"/",tr("All files(*.*)"));
    //QString filepath = QFileDialog::getExistingDirectory();
    QFileInfo fileinfo = QFileInfo(dir);
    auto filename = fileinfo.fileName();
    emit sigUpFile(filename,dir);
}
