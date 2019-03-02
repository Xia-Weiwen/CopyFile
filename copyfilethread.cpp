#include "copyfilethread.h"
#include <QFile>
#include <QFileInfo>
#include "common.h"
#include <QDebug>

CopyFileThread::CopyFileThread(QObject *parent)
    : QThread(parent)
{
    // 新线程的初始化不要放在这里
}

void CopyFileThread::run()
{
    // 新线程入口
    // 初始化和操作放在这里
    m_canceled = false;
    QFile src(SrcFile);
    QFile cpy(CpyFile);
    qint64 srcSize = QFileInfo(src).size();
    int bytesWritten = 0;
    if (srcSize==0 || !src.open(QFile::ReadOnly))
    {
        emit errorOccurred();
        return;
    }
    if (cpy.exists())
    {
        cpy.resize(0);
    }
    if (!cpy.open(QFile::WriteOnly))
    {
        src.close();
        emit errorOccurred();
        return;
    }
    while (!src.atEnd())
    {
        if (m_canceled)
        {
            src.close();
            cpy.close();
            cpy.remove();
            return;
        }
        msleep(8);
        qint64 wrt = cpy.write(src.readLine());
        if (wrt < 0)
        {
            emit errorOccurred();
            src.close();
            cpy.close();
            cpy.remove();
            return;
        }
        bytesWritten += wrt;
        emit percentCopied(bytesWritten*1.0/srcSize);
    }
    src.close();
    cpy.close();
    emit finished();
}

void CopyFileThread::cancelCopy()
{
    m_canceled = true;
}
