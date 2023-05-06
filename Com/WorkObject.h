#ifndef __WORKOBJECT_H__
#define __WORKOBJECT_H__

#include <QThread>
#include <QDir>
#include <QJsonObject>
#include <qnetworkreply.h>

class WorkObject : public QObject
{
    Q_OBJECT

public:
    enum WorkType
    {
        REMOVE_FOLDER,
        COPY_DIR
    };

public:
    WorkObject();
    ~WorkObject();

    static bool remove_folder(const QString& folderDir);

    static bool copy_dir(const QDir& fromDir, const QDir& toDir, bool bCoverIfFileExists = true);

    static bool find_process(const QString process);

    static void run_cmd_show(const QStringList& args);

    static QString get_mac_address();

    QString post(const QString& url, const QJsonObject& json);

    QString get(const QString& url);

private Q_SLOTS:

Q_SIGNALS:
    void sig_work_finished(WorkType, bool);

private:
    QNetworkAccessManager* m_manager;

};

#endif // !__UPDATETHREAD_H__
