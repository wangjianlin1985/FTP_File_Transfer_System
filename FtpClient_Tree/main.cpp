#include "ftpclient.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FtpClient ftpclient;
    ftpclient.show();

    return a.exec();
}
