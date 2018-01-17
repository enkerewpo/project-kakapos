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

#include "include/highlighter_C.h"

Hightlighter_C::Hightlighter_C(QTextDocument *parent): QSyntaxHighlighter(parent) {
    HighlightingRule rule;

    numbersFormat.setForeground(Qt::black);
    rule.pattern = QRegularExpression("[0-9]+[uU]?[lL]?[(ll)(LL)]?");
    rule.format = numbersFormat;
    highlightingRules.append(rule);

    over.setForeground(Qt::black);
    rule.pattern = QRegularExpression("[a-zA-Z]+[0-9]+");
    rule.format = over;
    highlightingRules.append(rule);

    numbersFormat.setForeground(Qt::black);
    rule.pattern = QRegularExpression("0[xX][0-9a-fA-F]+");
    rule.format = numbersFormat;
    highlightingRules.append(rule);

    functionFormat.setForeground(Qt::black);
    functionFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    keywordFormat.setForeground(Qt::blue);
    keywordFormat.setFontItalic(true);
    QStringList keywordPatterns;
    keywordPatterns << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b" <<"\\b__inline\\b" << "\\b__int64\\b"<<"\\b__int32\\b"<<"\\b__int128\\b"
                    << "\\bdouble\\b" << "\\benum\\b" << "\\bexplicit\\b" << "\\bauto\\b" <<"\\bfor\\b"
                    << "\\bfriend\\b" << "\\binline\\b" << "\\bint\\b" << "\\bconstexpr\\b"
                    << "\\blong\\b" << "\\bnamespace\\b" << "\\boperator\\b" << "\\bwhile\\b"
                    << "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b" << "\\bswitch\\b" << "\\bfinal\\b"
                    << "\\bshort\\b" << "\\bsignals\\b" << "\\bsigned\\b" << "\\breturn\\b" <<"\\boverride\\b"
                    << "\\bslots\\b" << "\\bstatic\\b" << "\\bstruct\\b" << "\\bif\\b" << "\\bbreak\\b"
                    << "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b" << "\\bgoto\\b"
                    << "\\bunion\\b" << "\\bunsigned\\b" << "\\bvirtual\\b" << "\\bcontinue\\b" << "\\bdelete\\b"
                    << "\\bvoid\\b" << "\\bvolatile\\b" << "\\bbool\\b" << "\\busing\\b" << "\\bnew\\b"
                    << "\\band\\b" << "\\bor\\b" << "\\bxor\\b" << "\\bfloat\\b" << "\\bcase\\b" << "\\bstatic_assert\\b"
                    << "\\bwchar_t\\b" <<"\\bthis\\b" << "\\bthrow\\b" << "\\btry\\b" << "\\balignas\\b" << "\\balignof\\b"
                    << "\\btypeid\\b" << "\\bregister\\b" << "\\bstatic_cast\\b" << "\\breinterpret_cast\\b" << "\\brequires\\b"
                    << "\\bsizeof\\b" << "\\bmutable\\b" << "\\band_eq\\b" << "\\bor_eq\\b" << "\\bcompl\\b"
                    << "\\bunion\\b" << "\\bnot_eq\\b" << "\\bor_eq\\b" << "\\bconst_cast\\b" << "\\bdecltype\\b"
                    << "\\bxor_eq\\b" << "\\bnoexcept\\b" << "\\basm\\b" << "\\bregister\\b" << "\\bdefault\\b" << "\\bconcept\\b"
                    << "\\bexplicit\\b" << "\\bdynamic_cast\\b" << "\\bnullptr\\b" << "\\bbitand\\b" << "\\bbitor\\b";
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    forformat.setForeground(Qt::blue);
    QStringList forlist;
    forlist << "\\bfor\\b" << "\\bswitch\\b" << "\\bwhile\\b" << "\\bdo\\b"
            << "\\bif\\b" << "\\bforeach\\b";
    foreach (const QString &pattern, forlist) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = forformat;
        highlightingRules.append(rule);
    }

    signsFormat.setForeground(Qt::black);
    rule.pattern = QRegularExpression("[!~@#$%^&*=+-/?:{}<>\\[\\]\\(\\);]");
    rule.format = signsFormat;
    highlightingRules.append(rule);

    quotationFormat.setForeground(Qt::magenta);
    rule.pattern = QRegularExpression("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);


    modulenumFormat.setForeground(Qt::darkCyan);
    rule.pattern = QRegularExpression("((%d)?(%lld)?(%I64d)?(%c)?(%s)?(%u)?(%lf)?(%llf)?)+");
    rule.format = modulenumFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(Qt::gray);
    rule.pattern = QRegularExpression("//[^\n]*[\"]*[^\n]*[\"]*[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(QColor("#6d7257"));

    sharpFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression("\#.*[^\n]");
    rule.format = sharpFormat;
    highlightingRules.append(rule);

    commentStartExpression = QRegularExpression("/\\*");
    commentEndExpression = QRegularExpression("\\*/");
}

void Hightlighter_C::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0) {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}
