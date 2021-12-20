#include "RichEditorPrivate.h"

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
