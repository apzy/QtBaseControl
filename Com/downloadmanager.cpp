#include "downloadmanager.h"

#include <QTextStream>

#include <cstdio>

using namespace std;

DownloadManager::DownloadManager(QObject* parent)
    : QObject(parent)
{}

void DownloadManager::append(const QUrl& url, const QString& fileName)
{
    downloadQueue.enqueue(url);
    if (!fileName.isEmpty())
        m_downloadName.insert(url, fileName);
}

QString DownloadManager::saveFileName(const QUrl& url)
{
    QString path = url.path();
    QString basename = QFileInfo(path).fileName();

    if (basename.isEmpty())
        basename = "download";

    if (QFile::exists(basename))
    {
        // already exists, don't overwrite
        int i = 0;
        basename += '.';
        while (QFile::exists(basename + QString::number(i)))
            ++i;

        basename += QString::number(i);
    }

    return basename;
}

void DownloadManager::start()
{
    downloadedCount = 0;
    totalCount = downloadQueue.size();
    startNextDownload();
}

void DownloadManager::startNextDownload()
{
    if (downloadQueue.isEmpty())
    {
        printf("%d/%d files downloaded successfully\n", downloadedCount, totalCount);
        output_log(QString("%1 / %2 file download finish").arg(QString::number(downloadedCount)).arg(QString::number(totalCount)));
        emit finished(downloadedCount == totalCount);
        return;
    }

    QUrl url = downloadQueue.dequeue();
    QString filename;
    if (m_downloadName.contains(url))
    {
        filename = m_downloadName.value(url);
        m_downloadName.remove(url);
    }
    else
    {
        filename = saveFileName(url);
    }

    output.setFileName(filename);

    if (output.exists())
    {
        ++downloadedCount;
        startNextDownload();
        return;
    }

    output_log(QString("download file name = %1").arg(filename));
    if (!output.open(QIODevice::WriteOnly))
    {
        fprintf(stderr, "Problem opening save file '%s' for download '%s': %s\n",
            qPrintable(filename), url.toEncoded().constData(),
            qPrintable(output.errorString()));

        output_log("open save file failed");
        startNextDownload();
        return;                 // skip this download
    }

    QNetworkRequest request(url);
    currentDownload = manager.get(request);
    connect(currentDownload, &QNetworkReply::downloadProgress,
        this, &DownloadManager::downloadProgress);
    connect(currentDownload, &QNetworkReply::finished,
        this, &DownloadManager::downloadFinished);
    connect(currentDownload, &QNetworkReply::readyRead,
        this, &DownloadManager::downloadReadyRead);

    // prepare the output
    downloadTimer.start();
}

void DownloadManager::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if (bytesTotal == 0)
        return;

    // calculate the download speed
    double speed = bytesReceived * 1000.0 / downloadTimer.elapsed();
    QString unit;
    if (speed < 1024)
    {
        unit = "bytes/sec";
    }
    else if (speed < 1024 * 1024)
    {
        speed /= 1024;
        unit = "kB/s";
    }
    else
    {
        speed /= 1024 * 1024;
        unit = "MB/s";
    }
    static int beforeProgress = 100.0f * bytesReceived / bytesTotal;
    int progress = 100.0f * bytesReceived / bytesTotal;
    if (progress != beforeProgress)
    {
        beforeProgress = progress;
        output_log(QString().sprintf("progress %d speed %lf%s", progress, speed, qPrintable(unit)));
    }
}

void DownloadManager::downloadFinished()
{
    output.close();

    if (currentDownload->error())
    {
        // download failed
        output_log("download failed " + currentDownload->errorString());
        output.remove();
    }
    else
    {
        // let's check if it was actually a redirect
        if (isHttpRedirect())
        {
            output_log("download failed http");
            reportRedirect();
            output.remove();
        }
        else
        {
            printf("Succeeded.\n");
            ++downloadedCount;
        }
    }

    currentDownload->deleteLater();
    startNextDownload();
}

void DownloadManager::downloadReadyRead()
{
    output.write(currentDownload->readAll());
}

bool DownloadManager::isHttpRedirect() const
{
    int statusCode = currentDownload->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    return statusCode == 301 || statusCode == 302 || statusCode == 303
        || statusCode == 305 || statusCode == 307 || statusCode == 308;
}

void DownloadManager::reportRedirect()
{
    int statusCode = currentDownload->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QUrl requestUrl = currentDownload->request().url();
    QTextStream(stderr) << "Request: " << requestUrl.toDisplayString()
        << " was redirected with code: " << statusCode
        << '\n';

    QVariant target = currentDownload->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (!target.isValid())
        return;
    QUrl redirectUrl = target.toUrl();
    if (redirectUrl.isRelative())
        redirectUrl = requestUrl.resolved(redirectUrl);
    QTextStream(stderr) << "Redirected to: " << redirectUrl.toDisplayString()
        << '\n';
}

void DownloadManager::output_log(const QString& log)
{
    qDebug("%s", qPrintable(log));
    Q_EMIT sig_log(log);
}
