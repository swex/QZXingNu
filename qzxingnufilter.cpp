#include "qzxingnufilter.h"
#include <QFutureWatcher>
#include <QPointer>
#include <QVariant>
#include <QtConcurrent>
#include <functional>
#include <memory>
#include <qzxingnu.h>
#include <zxing-cpp/core/src/BarcodeFormat.h>
#include <zxing-cpp/core/src/DecodeHints.h>
#include <zxing-cpp/core/src/GenericLuminanceSource.h>
#include <zxing-cpp/core/src/HybridBinarizer.h>
#include <zxing-cpp/core/src/MultiFormatReader.h>
#include <zxing-cpp/core/src/Result.h>
#include <QTimer>

namespace QZXingNu {

QZXingNuFilter::QZXingNuFilter(QObject *parent)
    : QObject(parent)
    , m_threadPool(new QThreadPool(this))
    , m_timer(new QTimer(this))
{
    m_threadPool->setMaxThreadCount(QThread::idealThreadCount() > 1 ? QThread::idealThreadCount() - 1 : QThread::idealThreadCount());
    m_timer->setInterval(200);
    m_timer->setSingleShot(false);
    m_timer->start();
    connect(m_timer, &QTimer::timeout, this, [this]() {
        if (m_decodersRunning >= m_threadPool->maxThreadCount()) {
            return;
        }

        if (!m_videoSink) {
            return;
        }
        if (!m_qzxingNu) {
            return;
        }
        QPointer<QZXingNuFilter> filterPointer(this);
        auto watcher = new QFutureWatcher<QZXingNuDecodeResult>();
        QObject::connect(watcher, &QFutureWatcher<QZXingNuDecodeResult>::finished, this, [watcher, filterPointer]() {
            if (filterPointer) {
                filterPointer->m_decodersRunning--;
            }
            auto result = watcher->future().result();
            delete watcher;
            return result;
        });
        m_decodersRunning++;
        auto future =
            QtConcurrent::run(m_threadPool, [this, frame = m_videoSink->videoFrame()]() { return m_qzxingNu->decodeImage(frame); });
        watcher->setFuture(future);
    });
    connect(this, &QZXingNuFilter::qzxingNuChanged, this,
            [this]() { connect(m_qzxingNu, &QZXingNu::decodeResultChanged, this, &QZXingNuFilter::setDecodeResult); });
    connect(this, &QZXingNuFilter::decodeResultChanged, this, [this]() { emit tagFound(m_decodeResult.text); });
}

QZXingNu *QZXingNuFilter::qzxingNu() const
{
    return m_qzxingNu;
}

QZXingNuDecodeResult QZXingNuFilter::decodeResult() const
{
    return m_decodeResult;
}

void QZXingNuFilter::setQzxingNu(QZXingNu *qzxingNu)
{
    if (m_qzxingNu == qzxingNu)
        return;

    m_qzxingNu = qzxingNu;
    emit qzxingNuChanged(m_qzxingNu);
}

void QZXingNuFilter::setDecodeResult(QZXingNuDecodeResult decodeResult)
{
    m_decodeResult = decodeResult;
    emit decodeResultChanged(m_decodeResult);
}

QVideoSink *QZXingNuFilter::videoSink() const
{
    return m_videoSink;
}

void QZXingNuFilter::setVideoSink(QVideoSink *newVideoSink)
{
    if (m_videoSink == newVideoSink)
        return;
    m_videoSink = newVideoSink;
    emit videoSinkChanged();
}

} // namespace QZXingNu
