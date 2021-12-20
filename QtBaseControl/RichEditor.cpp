#include "RichEditor.h"

#include "RichEditorPrivate.h"

#include <qfile.h>
#include <QMimeDatabase>
#include <QTextDocumentWriter>
#include <QFont>

RichEditor::RichEditor(QWidget *parent)
    : QTextEdit(parent)
    , d_ptr(new RichEditorPrivate(this))
{
    m_filePath = "";
}

RichEditor::~RichEditor()
{
}

bool RichEditor::open_file(const QString& filePath)
{
    if (!QFile::exists(filePath))
        return false;
    QFile file(filePath);
    if (!file.open(QFile::ReadOnly))
        return false;

    QByteArray data = file.readAll();
    QMimeDatabase db;
    const QString& mimeTypeName = db.mimeTypeForFileNameAndData(filePath, data).name();
    if (mimeTypeName == u"text/html")
    {
        auto encoding = QStringDecoder::encodingForHtml(data);
        QString str = QStringDecoder(encoding ? *encoding : QStringDecoder::Utf8)(data);
        QUrl fileUrl = filePath.startsWith(u':') ? QUrl(filePath) : QUrl::fromLocalFile(filePath);
        document()->setBaseUrl(fileUrl.adjusted(QUrl::RemoveFilename));
        setHtml(str);
#if QT_CONFIG(textmarkdownreader)
    }
    else if (mimeTypeName == u"text/markdown")
    {
        setMarkdown(QString::fromUtf8(data));
#endif
    }
    else
    {
        setPlainText(QString::fromUtf8(data));
    }

    m_filePath = filePath;

    return true;
}

bool RichEditor::save_file()
{
    return save_file(m_filePath);
}

bool RichEditor::save_file(const QString& filePath)
{
    bool bSuccess = true;
    if (filePath.isEmpty() || filePath.startsWith(u":/"))
        bSuccess = false;

    if (bSuccess)
    {
        QTextDocumentWriter writer(filePath);
        bSuccess = writer.write(document());
    }

    return bSuccess;
}

void RichEditor::text_normal()
{
    Q_D(RichEditor);
    QTextCharFormat fmt;
    fmt.setFontWeight(QFont::Normal);
    d->set_text_format(fmt);
}

void RichEditor::text_bold()
{
    Q_D(RichEditor);
    QTextCharFormat fmt;
    fmt.setFontWeight(QFont::Bold);
    d->set_text_format(fmt);
}

void RichEditor::text_under_line(const bool& bUnder)
{
    Q_D(RichEditor);
    QTextCharFormat fmt;
    fmt.setFontUnderline(bUnder);
    d->set_text_format(fmt);
}

void RichEditor::text_del_line(const bool& bDel)
{
    Q_D(RichEditor);
    QTextCharFormat fmt;
    fmt.setFontStrikeOut(bDel);
    d->set_text_format(fmt);
}

void RichEditor::text_italic(const bool& bItalic)
{
    Q_D(RichEditor);
    QTextCharFormat fmt;
    fmt.setFontItalic(bItalic);
    d->set_text_format(fmt);
}

QString RichEditor::get_file_path()
{
    return m_filePath;
}
