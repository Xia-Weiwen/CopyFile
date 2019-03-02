#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "common.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusBar->showMessage("程序启动");
    ::createSrcFile();
    // 使用自定义线程，重写run()函数，复制任务在run中完成
    m_cpyThread = new CopyFileThread(this);
    connect(m_cpyThread, SIGNAL(errorOccurred()),
            this, SLOT(errorHandleSlot()));
    connect(m_cpyThread, SIGNAL(percentCopied(double)),
            this, SLOT(updateCopyProgress(double)));
    connect(m_cpyThread, SIGNAL(finished()),
            this, SLOT(copyFinishSlot()));
    // 使用MoveToThread
    m_copier = new FileCopier; // 这个实例要负责复制任务，不要设置parent
    m_childThread = new QThread; // 子线程，本身不负责复制
    connectCopier(m_copier); // 连接信号-槽，复制的开始和取消指令是通过信号发送的
    m_copier->moveToThread(m_childThread); // 将实例移动到新的线程，实现多线程运行
    m_childThread->start(); // 启动子线程
    // 单线程
    m_copier2 = new FileCopier;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_grpOperation1_clicked(bool checked)
{
    ui->grpOperation2->setChecked(!checked);
    ui->grpOperation3->setChecked(!checked);
}

void MainWindow::on_grpOperation2_clicked(bool checked)
{
    ui->grpOperation1->setChecked(!checked);
    ui->grpOperation3->setChecked(!checked);
    connectCopier(m_copier); // 连接信号-槽，复制的开始和取消指令是通过信号发送的
    m_copier2->disconnect(); // 因为使用相同的信号和槽，避免冲突，取消连接
}

void MainWindow::on_grpOperation3_clicked(bool checked)
{
    ui->grpOperation1->setChecked(!checked);
    ui->grpOperation2->setChecked(!checked);
    connectCopier(m_copier2); // 连接信号-槽，复制的开始和取消指令是通过信号发送的
    m_copier->disconnect(); // 因为使用相同的信号和槽，避免冲突，取消连接
}

void MainWindow::on_btnQuit_clicked()
{
    if (ui->grpOperation1->isChecked())
    {
        if (ui->btnCancel1->isEnabled())
        {
            on_btnCancel1_clicked();
        }
    }
    else if (ui->grpOperation2->isChecked())
    {
        if (ui->btnCancel2->isEnabled())
        {
            on_btnCancel2_clicked();
        }
    }
    else if (ui->grpOperation3->isChecked())
    {
        if (ui->btnCancel3->isEnabled())
        {
            on_btnCancel3_clicked();
        }
    }
    exit(0);
}

void MainWindow::closeEvent(QCloseEvent *)
{
    on_btnQuit_clicked();
}

void MainWindow::connectCopier(FileCopier *copier)
{
    connect(this, SIGNAL(startCopyRsquested()), // 使用信号-槽机制，发出开始指令
            copier, SLOT(startCopying()));
    connect(this, SIGNAL(cancelCopuRequested()), // 使用信号-槽机制，发出取消指令
            copier, SLOT(cancelCopying()));
    connect(copier, SIGNAL(errorOccurred()),
            this, SLOT(errorHandleSlot()));
    connect(copier, SIGNAL(percentCopied(double)),
            this, SLOT(updateCopyProgress(double)));
    connect(copier, SIGNAL(finished()),
            this, SLOT(copyFinishSlot()));
}

void MainWindow::on_btnCopy1_clicked()
{
    ui->btnCopy1->setEnabled(false);
    ui->btnCancel1->setEnabled(true);
    // 注意：这里用start()不用run()
    m_cpyThread->start(); // 复制文件的新线程开始（自动调用run()）
    ui->statusBar->showMessage("开始复制");
}

void MainWindow::on_btnCancel1_clicked()
{
    ui->btnCopy1->setEnabled(true);
    ui->btnCancel1->setEnabled(false);
    m_cpyThread->cancelCopy();
    ui->statusBar->showMessage("复制取消");
}

void MainWindow::on_btnCopy2_clicked()
{
    ui->btnCopy2->setEnabled(false);
    ui->btnCancel2->setEnabled(true);
    emit startCopyRsquested();
    ui->statusBar->showMessage("开始复制");
}

void MainWindow::on_btnCancel2_clicked()
{
    ui->btnCopy2->setEnabled(true);
    ui->btnCancel2->setEnabled(false);
    emit cancelCopuRequested();
    ui->statusBar->showMessage("复制取消");
}

void MainWindow::on_btnCopy3_clicked()
{
    ui->btnCopy3->setEnabled(false);
    ui->btnCancel3->setEnabled(true);
    emit startCopyRsquested();
    ui->statusBar->showMessage("开始复制");
}

void MainWindow::on_btnCancel3_clicked()
{
    ui->btnCopy3->setEnabled(true);
    ui->btnCancel3->setEnabled(false);
    emit cancelCopuRequested();
    ui->statusBar->showMessage("复制取消");
}

void MainWindow::errorHandleSlot()
{
    if (ui->grpOperation1->isChecked())
    {
        ui->btnCopy1->setEnabled(true);
        ui->btnCancel1->setEnabled(false);
    }
    else if (ui->grpOperation2->isChecked())
    {
        ui->btnCopy2->setEnabled(true);
        ui->btnCancel2->setEnabled(false);
    }
    ui->statusBar->showMessage("复制出错");
}

void MainWindow::updateCopyProgress(double percent)
{
    ui->progressBar->setValue(percent*100);
}

void MainWindow::copyFinishSlot()
{
    ui->progressBar->setValue(100);
    if (ui->grpOperation1->isChecked())
    {
        ui->btnCopy1->setEnabled(true);
        ui->btnCancel1->setEnabled(false);
    }
    else if (ui->grpOperation2->isChecked())
    {
        ui->btnCopy2->setEnabled(true);
        ui->btnCancel2->setEnabled(false);
    }
    ui->statusBar->showMessage("复制完成");
}
