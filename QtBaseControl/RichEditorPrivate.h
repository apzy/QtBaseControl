#pragma once

#include "RichEditor.h"

class RichEditorPrivate
{
    Q_DECLARE_PUBLIC(RichEditor)

private:
    RichEditorPrivate(RichEditor* editor);

    void set_text_format(const QTextCharFormat& format);

    bool create_file(const QString& filePath);

    bool create_dir(const QString& dirPath);

private:
    RichEditor* q_ptr;
};

