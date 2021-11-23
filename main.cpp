#include <QApplication>
#include <QDebug>
#include <QFileDialog>
#include <QDir>
#include <QClipboard>
#include "MainWindow.h"


QStringList getLibFileName(const QString& t_path);
void toClipboard(const QStringList &t_strList);
void writeFile(const QString& t_path, const QStringList &t_strList);
QString readFile(const QString &fileName);


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}

QStringList getLibFileName(const QString& t_path)
{
    QStringList strList;

    QDir dir(t_path);
    if(!dir.exists()) {
        return strList;
    }

    dir.setFilter(QDir::Files);

    QFileInfoList list = dir.entryInfoList();
    if(list.size() == 0) {
        return strList;
    }

    for(int i = 0; i < list.size(); i++) {
        QFileInfo fileInfo = list.at(i);
        if(fileInfo.suffix() == "lib") {
            strList.append(list.at(i).completeBaseName());
        }
    }
    return strList;
}

void toClipboard(const QStringList &t_strList)
{
    QString str;
    for(auto i : t_strList) {
        /*    -lqwt \    */
        str += "-l" + i + " \\n";
    }
    QClipboard *clip = QApplication::clipboard();
    clip->setText(str);
}

void writeFile(const QString& t_path, const QStringList &t_strList)
{
    QFile file(t_path);
    file.remove();

    file.open(QIODevice::WriteOnly);
    file.close();

    if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream txtOutput(&file);
        for(auto i : t_strList) {
            /*    -lqwt \    */
            txtOutput << "-l" << i << " \\" << endl;
        }
        file.close();
    }
}

QString readFile(const QString &fileName)
{
    QString str;
    if(fileName.isEmpty()) {
        return str;
    }
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QByteArray line = file.readAll();
        str = line;
    }
    return str;
}
