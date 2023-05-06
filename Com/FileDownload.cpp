#include "FileDownload.h"

FileDownload::FileDownload(QObject* parent)
    :QObject(parent)
    ,m_tmpDir("")
{}

FileDownload::~FileDownload()
{}

void FileDownload::set_temp_dir(const QString& dir)
{
    m_tmpDir = dir;
}

void FileDownload::push_url(const QString& url, const QString& dstName)
{}

void FileDownload::start_download()
{}
