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

#include <QtWidgets>
#include <QWidget>
#include <QTextCursor>
#include <QDebug>
#include "Headers/code_editor.h"

bool is_auto;

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

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

    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space + 2;
}



void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}



void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width()+6, rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}



void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);
    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left()-3, cr.top(), lineNumberAreaWidth()+6, cr.height()+1));
}

void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(45,45,45);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}



void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    QRect rec = event->rect();
    painter.fillRect(rec, QColor(40,40,40));


    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
#ifdef Q_OS_MAC
            QFont font("DejaVu Sans Mono", fontsize + 1);
#elif defined (Q_OS_WIN)
            QFont font("Consolas", fontsize);
#else
            QFont font("Consolas", fontsize);
#endif
            QString number = " ";
            number += QString::number(blockNumber + 1);
            number += " ";
            painter.setPen(QColor(100,100,100));
            painter.setFont(font);
            painter.drawText(1, top, lineNumberArea->width() + 7, fontMetrics().height() + 2,
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int)blockBoundingRect(block).height();
        ++blockNumber;
    }
}


void CodeEditor::keyPressEvent(QKeyEvent *event) {
    QPlainTextEdit::keyPressEvent(event);

    update_layer(toPlainText());


    if (event->key() == Qt::Key_BraceLeft) {
        qDebug() << "GOT A BRACE LEFT";
        qDebug() << textCursor().position();
        is_auto = true;
    }

    if(event->key() == Qt::Key_BraceRight) {
        int pos = textCursor().position();
        int lay = layer[pos];
        QString text = toPlainText();
        int start;
        for(int i = pos - 1; ; i--) {
            if(text[i] == '\n') {
                qDebug() << i;
                start = i;
                break;
            }
//            if(text[i]!= '\t' || text[i] != ' ') goto ed;
            textCursor().deletePreviousChar();
        }
        textCursor().setPosition(start);
        for(int i = 1; i <= layer[pos]; i++) {
            qDebug() << "AT LAYER " << layer[pos];
            insertPlainText(QString("\t"));
        }
        insertPlainText("}");
    }
    ed:
    QKeyEvent* e = event;
    if(e->key() == Qt::Key_Return) {
        int pos = textCursor().position();
        qDebug() << "RETURN AT POS " << pos <<" WITH LAYER " << layer[pos];
        for(int i = 1; i <= layer[pos]; i++) {
            qDebug() << "AT LAYER " << layer[pos];
            insertPlainText(QString("\t"));
        }
        if(is_auto) {
            int curp = textCursor().position();
            qDebug() << curp ;
            int cur_p = textCursor().position();
            insertPlainText(QString("\n"));
            update_layer(toPlainText());

            for(int i = 1; i < layer[cur_p]; i++) {
                insertPlainText(QString("\t"));
            }
            insertPlainText("}");
            QTextCursor tc = textCursor();
            tc.setPosition(curp, QTextCursor::MoveAnchor);
            setTextCursor(tc);
            is_auto = false;
        }
    }
}

void CodeEditor::update_layer(QString text) {
    qDebug() << text;
    int tot = 0;
    int size = text.size();
    int i;

    for(i = 0; i < size; i++){
        if(text[i] == '{') tot++;
        if(text[i] == '}') tot--;
        layer[i] = tot;
        qDebug() << "layer[" << i << "] = " << layer[i];
    }
    for(int j = i; j <= i + 1000; j++) layer[i] = tot;
}
