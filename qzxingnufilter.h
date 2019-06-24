#ifndef QZXINGNUFILTER_H
#define QZXINGNUFILTER_H

#include <QAbstractVideoFilter>
#include <QThreadPool>
#include <QTime>
#include <qzxingnu.h>

class QZXingNuFilter : public QAbstractVideoFilter
{
    Q_OBJECT
    Q_PROPERTY(QZXingNu *qzxingNu READ qzxingNu WRITE setQzxingNu NOTIFY qzxingNuChanged)
    Q_PROPERTY(QZXingNu::DecodeResult decodeResult READ decodeResult WRITE setDecodeResult NOTIFY
                   decodeResultChanged)
    Q_PROPERTY(QRect captureRect READ captureRect WRITE setCaptureRect)
    Q_PROPERTY(qint32 intervalToCheckFrames READ intervalToCheckFrames WRITE setIntervalToCheckFrames)

    QRect m_captureRect;
    QZXingNu *m_qzxingNu = nullptr;
    QThreadPool *m_threadPool = nullptr;
    QZXingNu::DecodeResult m_decodeResult;
    QTime m_lastFrameCheckedTime;
    qint32 m_intervalToCheckFrames = 250;

    friend class QZXingNuFilterRunnable;

public:
    QZXingNuFilter(QObject *parent = nullptr);

    QRect captureRect() const;
    void setCaptureRect(QRect a_captureRect);

    qint32 intervalToCheckFrames() const;
    void setIntervalToCheckFrames(qint32 a_intervalToCheckFrames);

    // QAbstractVideoFilter interface
public:
    QVideoFilterRunnable *createFilterRunnable() override;
    QZXingNu *qzxingNu() const;
    QZXingNu::DecodeResult decodeResult() const { return m_decodeResult; }

signals:
    void tagFound(QString tag, QZXingNu::BarcodeFormat format);

public slots:
    void setQzxingNu(QZXingNu *qzxingNu);

    void setDecodeResult(QZXingNu::DecodeResult decodeResult)
    {
        m_decodeResult = decodeResult;
        emit decodeResultChanged(m_decodeResult);
    }

signals:
    void qzxingNuChanged(QZXingNu *qzxingNu);
    void decodeResultChanged(QZXingNu::DecodeResult decodeResult);
};

#endif // QZXINGNUFILTER_H
