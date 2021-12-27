#include "RichEditorPrivate.h"
#include <QDir>

RichEditorPrivate::RichEditorPrivate(RichEditor* editor)
    :q_ptr(editor)
{
}

void RichEditorPrivate::set_text_format(const QTextCharFormat& format)
{
    Q_Q(RichEditor);
    QTextCursor cursor = q->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    q->mergeCurrentCharFormat(format);
}

bool RichEditorPrivate::create_file(const QString& filePath)
{
    bool bSuccess = true;
    QString path = filePath;
    path.replace("\\", "/");
    QDir dir(path.left(path.lastIndexOf("/")));
    if (!dir.exists())
    {
        bSuccess = create_dir(dir.path());
    }
    if (bSuccess)
    {
        QFile file(filePath);
        bSuccess = file.open(QIODevice::ReadWrite | QIODevice::Text);
        file.close();
    }
    return bSuccess;
}

bool RichEditorPrivate::create_dir(const QString& dirPath)
{
    QDir dir;
    return dir.mkpath(dirPath);
}
