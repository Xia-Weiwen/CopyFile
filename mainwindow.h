#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "copyfilethread.h"
#include "filecopier.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_grpOperation1_clicked(bool checked);

    void on_grpOperation2_clicked(bool checked);

    void on_grpOperation3_clicked(bool checked);

    void on_btnQuit_clicked();

    void on_btnCopy1_clicked();

    void on_btnCancel1_clicked();

    void on_btnCopy2_clicked();

    void on_btnCancel2_clicked();

    void on_btnCopy3_clicked();

    void on_btnCancel3_clicked();

    void errorHandleSlot();

    void updateCopyProgress(double percent);

    void copyFinishSlot();

signals:
    void startCopyRsquested();

    void cancelCopuRequested();


private:
    Ui::MainWindow *ui;
    CopyFileThread * m_cpyThread; // 用于复制文件的子线程
    FileCopier * m_copier; // 用户复制文件的类
    QThread * m_childThread; // m_copier将被移动到此线程执行
    FileCopier * m_copier2; // 用户复制文件的类

    void closeEvent(QCloseEvent *);
    void connectCopier(FileCopier * copier);
};

#endif // MAINWINDOW_H
