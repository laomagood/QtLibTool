#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QDragEnterEvent>
#include <QTimer>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void dropPath(QStringList str);

protected:
    virtual void dragEnterEvent(QDragEnterEvent *event) override;   // 拖动进入事件
    virtual void dropEvent(QDropEvent*event) override;              // 放下事件

private:
    void initializeUi();                                            // 初始化界面
    void initializeData();                                          // 初始化数据
    void handleEvents();                                            // 信号与槽处理

    QStringList getLibFileName(const QString& t_path);
    void toClipboard(const QStringList &t_strList);
    void writeFile(const QString& t_path, const QStringList &t_strList);
    QString readFile(const QString &fileName);

private:
    QLabel *m_pLabel;
};

#endif // MAINWINDOW_H
