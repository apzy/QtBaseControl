#ifndef __RICHEDITOR_H__
#define __RICHEDITOR_H__

#include <QWidget>
#include <QTextEdit>

class RichEditorPrivate;

class Q_DECL_EXPORT RichEditor : public QTextEdit 
{
    Q_OBJECT

public:
    RichEditor(QWidget *parent = Q_NULLPTR);
    ~RichEditor();

public Q_SLOTS:
    bool open_file(const QString& filePath);

    bool save_file();

    /// @brief 保存文件到指定路径 保存的文件一定要加后缀
    /// @param filePath 保存的文件路径
    /// @return 是否保存成功
    bool save_file(const QString& filePath);

    void text_normal();
    void text_bold();
    void text_italic(const bool& bItalic);
    void text_under_line(const bool& bUnder);
    void text_del_line(const bool& bDel);

    QString get_file_path();

private:
    
    Q_DECLARE_PRIVATE(RichEditor);
    QScopedPointer<RichEditorPrivate> d_ptr;

    QString m_filePath;
};

#endif
