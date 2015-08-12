#include "main.h"

QString decodeHTML(const QString &encoded) {
    QTextDocument text;
    text.setHtml(encoded);
    return text.toPlainText();
}
