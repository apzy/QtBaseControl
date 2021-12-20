#pragma once

#include "RichEditor.h"

class RichEditorPrivate
{
    Q_DECLARE_PUBLIC(RichEditor)

private:
    RichEditorPrivate(RichEditor* editor);

    void set_text_format(const QTextCharFormat& format);

private:
    RichEditor* q_ptr;
};

