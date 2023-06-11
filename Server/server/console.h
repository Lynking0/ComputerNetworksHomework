#ifndef CONSOLE_H
#define CONSOLE_H

#include <QTextEdit>

class Console : public QTextEdit
{
public:
    Console(QWidget *parent = nullptr) : QTextEdit(parent)
    {
        setReadOnly(true);
        setPlainText("Welcome to Console.\n");
    }

    void write(const QString &text)
    {
        moveCursor(QTextCursor::End);
        insertPlainText(text);
        moveCursor(QTextCursor::End);
    }

    void clear()
    {
        setPlainText("");
    }
};

#endif // CONSOLE_H
