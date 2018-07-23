#ifndef QZXINGNUFILTER_H
#define QZXINGNUFILTER_H

#include <QAbstractVideoFilter>
#include <QThreadPool>
#include <QAtomicInt>

class QZXingNu;

class QZxingNuFilter : public QAbstractVideoFilter
{
    Q_OBJECT
    Q_PROPERTY(QZXingNu *qzxingNu READ qzxingNu WRITE setQzxingNu NOTIFY qzxingNuChanged)
    Q_PROPERTY(bool busy READ busy WRITE setBusy NOTIFY busyChanged)
    QZXingNu *m_qzxingNu = nullptr;
    QThreadPool *m_threadPool = nullptr;
    QAtomicInt m_busy = 0;
    friend class QZxingNuFilterRunnable;

public:
    QZxingNuFilter(QObject *parent = nullptr);

    // QAbstractVideoFilter interface
public:
    QVideoFilterRunnable *createFilterRunnable() override;
    QZXingNu *qzxingNu() const;
    bool busy() const;
signals:
    void tagFound(QString tag);
public slots:
    void setQzxingNu(QZXingNu *qzxingNu);
    void setBusy(bool busy);

signals:
    void qzxingNuChanged(QZXingNu *qzxingNu);
    void busyChanged(bool busy);
};

#endif // QZXINGNUFILTER_H
