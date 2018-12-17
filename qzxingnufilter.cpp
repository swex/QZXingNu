#include "qzxingnufilter.h"
#include <QFutureWatcher>
#include <QVideoFilterRunnable>
#include <QtConcurrent>
#include <functional>
#include <qzxingnu.h>
#include <zxing-cpp/core/src/BarcodeFormat.h>
#include <zxing-cpp/core/src/DecodeHints.h>
#include <zxing-cpp/core/src/GenericLuminanceSource.h>
#include <zxing-cpp/core/src/HybridBinarizer.h>
#include <zxing-cpp/core/src/MultiFormatReader.h>
#include <zxing-cpp/core/src/Result.h>

extern QImage qt_imageFromVideoFrame(const QVideoFrame &frame);

class QZxingNuFilterRunnable : public QObject, public QVideoFilterRunnable
{
    QZxingNuFilter *m_filter = nullptr;

public:
    QZxingNuFilterRunnable(QZxingNuFilter *filter, QObject *parent = nullptr)
        : QObject(parent)
        , m_filter(filter)
    {
    }

    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat & /*surfaceFormat*/,
                    RunFlags  /*flags*/) override
    {
        if (!m_filter) {
            qWarning() << "filter null";
            return *input;
        }
        if (!input || !input->isValid()) {
            return *input;
        }
        if (m_filter->m_busy) {
            return *input;
        }
        m_filter->m_busy = 1;
        const auto frame = qt_imageFromVideoFrame(*input);
        auto bound = std::bind(&QZXingNu::decodeImage, m_filter->m_qzxingNu, std::placeholders::_1);
        auto watcher = new QFutureWatcher<spZXingResult>();
        QObject::connect(watcher, &QFutureWatcher<spZXingResult>::finished, m_filter,
                         [watcher, this]() {
                             auto result = watcher->future().result();
                             if (result && result->isValid()) {
                                 emit m_filter->tagFound(QString::fromStdWString(result->text()));
                             }
                             m_filter->m_busy = 0;
                         });
        QObject::connect(watcher, &QFutureWatcher<spZXingResult>::finished, m_filter,
                         [this, watcher]() { watcher->deleteLater(); });
        watcher->setFuture(QtConcurrent::run(m_filter->m_threadPool, bound, frame));
        return *input;
    }
};

QZxingNuFilter::QZxingNuFilter(QObject *parent)
    : QAbstractVideoFilter(parent)
    , m_threadPool(new QThreadPool(this))
{
    m_threadPool->setMaxThreadCount(1);
}

QVideoFilterRunnable *QZxingNuFilter::createFilterRunnable()
{
    return new QZxingNuFilterRunnable(this);
}

QZXingNu *QZxingNuFilter::qzxingNu() const
{
    return m_qzxingNu;
}

bool QZxingNuFilter::busy() const
{
    return m_busy;
}

void QZxingNuFilter::setQzxingNu(QZXingNu *qzxingNu)
{
    if (m_qzxingNu == qzxingNu)
        return;

    m_qzxingNu = qzxingNu;
    emit qzxingNuChanged(m_qzxingNu);
}

void QZxingNuFilter::setBusy(bool busy)
{
    if (m_busy == busy)
        return;

    m_busy = busy;
    emit busyChanged(m_busy);
}
