#ifndef COPYFILETHREAD_H
#define COPYFILETHREAD_H

#include <QThread>

class CopyFileThread : public QThread
{
    Q_OBJECT
public:
    CopyFileThread(QObject * parent = 0);

protected:
    void run(); // 新线程入口

signals:
    void percentCopied(double percent);
    void finished();
    void errorOccurred();

public slots:
    void cancelCopy();

private:
    bool m_canceled;
};

#endif // COPYFILETHREAD_H
