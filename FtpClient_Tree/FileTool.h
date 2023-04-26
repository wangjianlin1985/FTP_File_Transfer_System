#ifndef FILETOOL_H
#define FILETOOL_H

#include <QObject>


class FileTool
{
public:
    static FileTool * getInstance(){
        if(ptr){
            return ptr = new FileTool();
        }
    }

public:
    void openFile(const QString & filepath);

    void openFileDilago(QString title);
private:
    FileTool();

    static FileTool * ptr;
};

#endif // FILETOOL_H
