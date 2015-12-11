#include "main.h"

QString decodeHTML(const QString &encoded) {
    QTextDocument text;
    text.setHtml(encoded);
    return text.toPlainText();
}

bool readHash(const QString &fileName, QHash<QString, QVariant> &hash) {
    hash.clear();
    QFile f(fileName);
    if(!f.open(QFile::ReadOnly | QFile::Text)) return false;
    QTextStream in(&f);
    while(!in.atEnd()) {
        QStringList sl = in.readLine().split('=');
        if(sl.length()==2) hash[ sl[0] ] = sl[1];
    }
    f.close();
    return true;
}

bool saveHash(const QString &fileName, QHash<QString, QVariant> &hash) {
    QFile f(fileName);
    if(!f.open(QFile::WriteOnly | QFile::Text)) return false;
    QTextStream out(&f);
    QHashIterator<QString, QVariant> i(hash);
    while(i.hasNext()) {
        i.next();
        out << i.key() << '=' << i.value().toString() << endl;
    }
    out.flush();
    f.close();
    return true;
}
