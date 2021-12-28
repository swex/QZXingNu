#ifndef QZXINGNUFILTER_H
#define QZXINGNUFILTER_H

#include <qzxingnu.h>
#include <QMediaCaptureSession>
#include <QImageCapture>
#include <QVideoSink>
#include <QThreadPool>

namespace QZXingNu {

class QZXingNuFilter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QZXingNu *qzxingNu READ qzxingNu WRITE setQzxingNu NOTIFY qzxingNuChanged)
    Q_PROPERTY(QZXingNuDecodeResult decodeResult READ decodeResult WRITE setDecodeResult NOTIFY decodeResultChanged)
    Q_PROPERTY(QVideoSink *videoSink READ videoSink WRITE setVideoSink NOTIFY videoSinkChanged)
    QThreadPool *m_threadPool;

    QZXingNu *m_qzxingNu = nullptr;
    QZXingNuDecodeResult m_decodeResult;
    QVideoSink *m_videoSink = nullptr;
    QTimer *m_timer;
    int m_decodersRunning = 0;

public:
    QZXingNuFilter(QObject *parent = nullptr);

public:
    QZXingNu *qzxingNu() const;
    QZXingNuDecodeResult decodeResult() const;

    QVideoSink *videoSink() const;
    void setVideoSink(QVideoSink *newVideoSink);

signals:
    void tagFound(QString tag);
public slots:
    void setQzxingNu(QZXingNu *qzxingNu);
    void setDecodeResult(QZXingNuDecodeResult decodeResult);

signals:
    void qzxingNuChanged(QZXingNu *qzxingNu);
    void decodeResultChanged(QZXingNuDecodeResult decodeResult);
    void videoSinkChanged();
};

} // namespace QZXingNu
#endif // QZXINGNUFILTER_H
