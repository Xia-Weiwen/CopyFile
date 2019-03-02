#ifndef FILECOPIER_H
#define FILECOPIER_H

#include <QObject>

class FileCopier : public QObject
{
    Q_OBJECT
public:
    explicit FileCopier(QObject *parent = 0);

signals:
    void percentCopied(double percent);
    void finished();
    void errorOccurred();

public slots:
    void startCopying();
    void cancelCopying();

private:
    bool m_canceled;
};

#endif // FILECOPIER_H
