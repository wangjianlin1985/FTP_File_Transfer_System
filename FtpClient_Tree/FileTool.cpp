#include "FileTool.h"
#include <QUrl>
#include <QFile>

FileTool * FileTool::ptr = nullptr;

void FileTool::openFile(const QString &filepath)
{
    QUrl thisurl= QUrl::fromLocalFile(filepath);
}

void FileTool::openFileDilago(QString title)
{

}

FileTool::FileTool()
{

}
