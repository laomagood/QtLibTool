#include "MainWindow.h"
#include <QDebug>
#include <QApplication>
#include <QLayout>
#include <QFileDialog>
#include <QMimeData>
#include <QClipboard>
#include <QLabel>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    initializeUi();
    initializeData();
    handleEvents();
}

MainWindow::~MainWindow()
{
    //    m_pMediator->deleteLater();
    //    m_pMediator = nullptr;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list")) {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty()) {
        return;
    }

    QStringList strList;
    // [1] 文件夹
    QString str = urls.first().toLocalFile();
    QFileInfo fileInfo(str);
    if (fileInfo.isDir()) {
        strList = getLibFileName(str);
        emit dropPath(strList);
        return;
    }
    // [2] 文件
    for(auto i : urls) {
        fileInfo.setFile(i.toLocalFile());
        if(fileInfo.suffix() == "lib") {
            strList.append(fileInfo.completeBaseName());
        }
    }
    emit dropPath(strList);
}

void MainWindow::initializeUi()
{
    this->setWindowTitle(QStringLiteral("Qt Lib 生成器"));
    this->setAcceptDrops(true);

    m_pLabel = new QLabel(QStringLiteral("请拖入文件或文件夹"));
    m_pLabel->setAlignment(Qt::AlignCenter);
    this->takeCentralWidget();
    this->setCentralWidget(m_pLabel);

    // 样式
    QString strStyle =
            "QLabel{"
            "color: rgba(255, 255, 255, 200);"
            "background: rgb(34, 37, 53);"
            "font-family: Microsoft YaHei;"
            "font-size: 18px;"
            "font-weight: bold;}";

    m_pLabel->setStyleSheet(strStyle);

    this->resize(320, 175);

}

void MainWindow::initializeData()
{

}

void MainWindow::handleEvents()
{
    connect(this, &MainWindow::dropPath, [this](QStringList t_strList) {
        qDebug() << t_strList;
        toClipboard(t_strList);
        m_pLabel->setText(QStringLiteral("已复制到剪贴板"));

        QTimer::singleShot(2000, this, [this] {
            m_pLabel->setText(QStringLiteral("请拖入文件或文件夹"));
        });
    });
}


QStringList MainWindow::getLibFileName(const QString& t_path)
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

void MainWindow::toClipboard(const QStringList &t_strList)
{
    QString str;
    for(auto i : t_strList) {
        /*    -lqwt \    */
        str += "-l" + i + " \\" + "\n";
    }
    QClipboard *clip = QApplication::clipboard();
    clip->setText(str);
}

void MainWindow::writeFile(const QString& t_path, const QStringList &t_strList)
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

QString MainWindow::readFile(const QString &fileName)
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

