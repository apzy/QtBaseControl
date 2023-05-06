#ifndef __FILEDOWNLOAD_H__
#define __FILEDOWNLOAD_H__

#include <QtNetwork>
#include <QtCore>

class FileDownload : public QObject
{
    Q_OBJECT
public:
    FileDownload(QObject* parent = nullptr);
    ~FileDownload();

    void set_temp_dir(const QString& dir);

    void push_url(const QString& url, const QString& dstName = "");

    void start_download();

private:
    QString m_tmpDir;
};

#endif // !__FILEDOWNLOAD_H__
