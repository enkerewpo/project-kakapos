/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "include/CodeEditor.h"
#include "include/MainWindow.h"
#include <QByteArray>
#include <QDebug>
#include <QTextCursor>
#include <QChar>
#include <QTypeInfo>
#include <QWidget>
#include <QtWidgets>
#include <cctype>

bool is_auto;

CodeEditor::CodeEditor(QWidget* parent)
    : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);
    setAcceptDrops(true);
    connect(this,
            SIGNAL(blockCountChanged(int)),
            this,
            SLOT(updateLineNumberAreaWidth(int)));
    connect(this,
            SIGNAL(updateRequest(QRect, int)),
            this,
            SLOT(updateLineNumberArea(QRect, int)));
    connect(
                this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }
    QFontMetrics met(font);
    int space = 2 * digits + 9;
//    int space = 0;
    return space;
}

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth() + 16, 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect& rect, int dy)
{
    if (dy) {
        lineNumberArea->scroll(0, dy);
    } else {
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
    }
    if (rect.contains(viewport()->rect())) {
        updateLineNumberAreaWidth(0);
    }
}

void CodeEditor::resizeEvent(QResizeEvent* e)
{
    QPlainTextEdit::resizeEvent(e);
    QRect cr = contentsRect();
    lineNumberArea->setGeometry(
                QRect(cr.left(), cr.top(), lineNumberAreaWidth() + 15, cr.height()));
}

void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;
        QColor lineColor = QColor(Qt::white);
        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }
    setExtraSelections(extraSelections);
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent* event)
{
    QPainter painter(lineNumberArea);
    QRect rec = event->rect();
    painter.fillRect(rec, QColor("#ececec"));
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int)blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int)blockBoundingRect(block).height();
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = " ";
            number += QString::number(blockNumber + 1);
            number += " ";
            painter.setPen(Qt::black);
            painter.setFont(font);
            painter.drawText(0,
                             top,
                             lineNumberArea->width() + 3,
                             fontMetrics().height() + 2,
                             Qt::AlignRight,
                             number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int)blockBoundingRect(block).height();
        ++blockNumber;
    }
}
QString text;
QTextCursor backup;
bool flag;
void CodeEditor::keyPressEvent(QKeyEvent* event)
{
    emit modified();
    if (filetype == "cplusplus") {
        if (event->key() == Qt::Key_Backspace) {
            text = toPlainText();
            int pos1 = textCursor().position() - 1;
            int pos2 = pos1 + 1;
            QChar c = text[pos1];
            QChar d = text[pos2];
            qDebug() << c << " " << d;
            if (c == '(' && d == ')') {
                textCursor().deleteChar();
            }
            if (c == '[' && d == ']') {
                textCursor().deleteChar();
            }
            if (c == '\"' && d == '\"') {
                textCursor().deleteChar();
            }
            if (c == '\'' && d == '\'') {
                textCursor().deleteChar();
            }
        }

        if (event->key() == Qt::Key_ParenRight) {
            text = toPlainText();
            int pos = textCursor().position();
            qDebug() << pos;
            if ((QString)(QString)text[pos] ==  ")") {
                QTextCursor tc = textCursor();
                tc.setPosition(pos + 1, QTextCursor::MoveAnchor);
                setTextCursor(tc);
                goto ed;
            }
        }

        if (event->key() == Qt::Key_QuoteDbl) {
            text = toPlainText();
            int pos = textCursor().position();
            qDebug() << pos;
            if ((QString)(QString)text[pos] ==  "\"") {
                QTextCursor tc = textCursor();
                tc.setPosition(pos + 1, QTextCursor::MoveAnchor);
                setTextCursor(tc);
                goto ed;
            }
        }

        if (event->key() == Qt::Key_Apostrophe) {
            text = toPlainText();
            int pos = textCursor().position();
            qDebug() << pos;
            if ((QString)text[pos] ==  "\'") {
                QTextCursor tc = textCursor();
                tc.setPosition(pos + 1, QTextCursor::MoveAnchor);
                setTextCursor(tc);
                goto ed;
            }
        }

        if (event->key() == Qt::Key_BracketRight) {
            text = toPlainText();
            int pos = textCursor().position();
            qDebug() << pos;
            if ((QString)text[pos] ==  "]") {
                QTextCursor tc = textCursor();
                tc.setPosition(pos + 1, QTextCursor::MoveAnchor);
                setTextCursor(tc);
                goto ed;
            }
        }

        QPlainTextEdit::keyPressEvent(event);
        text = toPlainText();
        update_layer(text);
        if (event->key() == Qt::Key_BraceLeft) {
            is_auto = true;
        }

        if (event->key() == Qt::Key_BraceRight) {
            int pos = textCursor().position();
//            int lay = layer[pos];
            text = toPlainText();
            int start;
            for (int i = pos - 1;; i--) {
                if (text[i] == '\n') {
                    start = i;
                    break;
                }
                textCursor().deletePreviousChar();
            }
            textCursor().setPosition(start);
            for (int i = 1; i <= layer[pos]; i++) {
                insertPlainText(QString("\t"));
            }
            insertPlainText("}");
        }
ed:

        QKeyEvent* e = event;
        if (e->key() == Qt::Key_Return) {
            textCursor().deletePreviousChar();
            insertPlainText("\r");
            int pos = textCursor().position();
            qDebug() << "RETURN AT POS " << pos << " WITH LAYER " << layer[pos];
            for (int i = 1; i <= layer[pos]; i++) {
                insertPlainText(QString("\t"));
            }
            if (is_auto) {
                int curp = textCursor().position();
                int cur_p = textCursor().position();
                insertPlainText(QString("\n"));
                update_layer(toPlainText());

                for (int i = 1; i < layer[cur_p]; i++) {
                    insertPlainText(QString("\t"));
                }
                insertPlainText("}");
                QTextCursor tc = textCursor();
                tc.setPosition(curp, QTextCursor::MoveAnchor);
                setTextCursor(tc);
                is_auto = false;
            }
        }
        if (e->key() == Qt::Key_ParenLeft) {
            text = toPlainText();
            QChar cc = text[textCursor().position()];
            QString c = "";
            c += cc;
            QByteArray ch = c.toLatin1();
            const char* chs = ch.data();
            if (isalpha(*chs) || isdigit(*chs))
                goto ed_2;
            insertPlainText(")");
            QTextCursor tc = textCursor();
            tc.setPosition(textCursor().position() - 1, QTextCursor::MoveAnchor);
            setTextCursor(tc);
        }
        if (e->key() == Qt::Key_QuoteDbl) {
            text = toPlainText();
            QChar cc = text[textCursor().position()];
            QString c = "";
            c += cc;
            QByteArray ch = c.toLatin1();
            const char* chs = ch.data();
            if (isalpha(*chs) || isdigit(*chs))
                goto ed_2;
            insertPlainText("\"");
            QTextCursor tc = textCursor();
            tc.setPosition(textCursor().position() - 1, QTextCursor::MoveAnchor);
            setTextCursor(tc);
        }

        if (e->key() == Qt::Key_Apostrophe) {
            text = toPlainText();
            QChar cc = text[textCursor().position()];
            QString c = "";
            c += cc;
            QByteArray ch = c.toLatin1();
            const char* chs = ch.data();
            if (isalpha(*chs) || isdigit(*chs))
                goto ed_2;
            insertPlainText("\'");
            QTextCursor tc = textCursor();
            tc.setPosition(textCursor().position() - 1, QTextCursor::MoveAnchor);
            setTextCursor(tc);
        }

        if (e->key() == Qt::Key_BracketLeft) {
            text = toPlainText();
            QChar cc = text[textCursor().position()];
            QString c = "";
            c += cc;
            QByteArray ch = c.toLatin1();
            const char* chs = ch.data();
            if (isalpha(*chs) || isdigit(*chs))
                goto ed_2;
            insertPlainText("]");
            QTextCursor tc = textCursor();
            tc.setPosition(textCursor().position() - 1, QTextCursor::MoveAnchor);
            setTextCursor(tc);
        }
ed_2:;
    } else {
        if (event->key() == Qt::Key_Return) {
            QPlainTextEdit::keyPressEvent(event);
            textCursor().deletePreviousChar();
            insertPlainText("\r");
        } else
            QPlainTextEdit::keyPressEvent(event);
    }
}

void CodeEditor::update_layer(QString text)
{
    int tot = 0;
    int size = text.size();
    int i;

    for (i = 0; i < size; i++) {
        if (text[i] == '{')
            tot++;
        if (text[i] == '}')
            tot--;
        layer[i] = tot;
    }
    for (int j = i; j <= i + 1000; j++)
        layer[i] = tot;
}

void CodeEditor::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
    }
}

void CodeEditor::dropEvent(QDropEvent *e)
{
    foreach (const QUrl &url, e->mimeData()->urls()) {
        QString fileNameFromDrop = url.toLocalFile();
        emit droppedFile(fileNameFromDrop);
    }
}
