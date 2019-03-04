#ifndef QZXINGNUFILTER_H
#define QZXINGNUFILTER_H

#include <QAbstractVideoFilter>
#include <QThreadPool>
#include <qzxingnu.h>
namespace QZXingNu {

class QZXingNuFilter : public QAbstractVideoFilter
{
    Q_OBJECT
    Q_PROPERTY(QZXingNu *qzxingNu READ qzxingNu WRITE setQzxingNu NOTIFY qzxingNuChanged)
    Q_PROPERTY(QZXingNuDecodeResult decodeResult READ decodeResult WRITE setDecodeResult NOTIFY
                   decodeResultChanged)
    QZXingNu *m_qzxingNu = nullptr;
    QThreadPool *m_threadPool = nullptr;
    friend class QZXingNuFilterRunnable;

public:
    QZXingNuFilter(QObject *parent = nullptr);

    // QAbstractVideoFilter interface
public:
    QVideoFilterRunnable *createFilterRunnable() override;
    QZXingNu *qzxingNu() const;
    QZXingNuDecodeResult decodeResult() const { return m_decodeResult; }

signals:
    void tagFound(QString tag);
public slots:
    void setQzxingNu(QZXingNu *qzxingNu);

    void setDecodeResult(QZXingNuDecodeResult decodeResult)
    {
        m_decodeResult = decodeResult;
        emit decodeResultChanged(m_decodeResult);
    }

private:
    QZXingNuDecodeResult m_decodeResult;

signals:
    void qzxingNuChanged(QZXingNu *qzxingNu);
    void decodeResultChanged(QZXingNuDecodeResult decodeResult);
};

} // namespace QZXingNu
#endif // QZXINGNUFILTER_H
