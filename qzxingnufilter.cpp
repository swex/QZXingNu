#include "qzxingnufilter.h"
#include <QVariant>
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
#include <QPointer>

extern QImage qt_imageFromVideoFrame(const QVideoFrame &frame);
namespace QZXingNu {

class QZXingNuFilterRunnable : public QObject, public QVideoFilterRunnable
{

    QZXingNuFilter *m_filter = nullptr;

public:
    explicit QZXingNuFilterRunnable(QZXingNuFilter *filter, QObject *parent = nullptr)
        : QObject(parent)
        , m_filter(filter)
    {
    }

    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat & /*surfaceFormat*/,
                    RunFlags /*flags*/) override
    {
        if (m_filter == nullptr) {
            qWarning() << "filter null";
            return *input;
        }
        if (!input->isValid()) {
            qWarning() << "input invalid";
            return *input;
        }

        if (m_filter->m_threadPool->activeThreadCount()
            >= m_filter->m_threadPool->maxThreadCount()) {
            qDebug() << QString("FAIL: threads: %1, max_threads: %2")
                            .arg(m_filter->m_threadPool->activeThreadCount())
                            .arg(m_filter->m_threadPool->maxThreadCount());
            return *input;
        }
        const auto frame = qt_imageFromVideoFrame(*input);
        auto bound = std::bind(&QZXingNu::decodeImage, m_filter->m_qzxingNu, std::placeholders::_1);
        auto watcher = new QFutureWatcher<QZXingNuDecodeResult>(this);
        QObject::connect(watcher, &QFutureWatcher<QZXingNuDecodeResult>::finished, this,
                         [watcher, this]() {
                             auto result = watcher->future().result();
                             delete watcher;
                         });
        auto future = QtConcurrent::run(m_filter->m_threadPool, bound, frame);
        watcher->setFuture(future);
        return *input;
    }
};

QZXingNuFilter::QZXingNuFilter(QObject *parent)
    : QAbstractVideoFilter(parent)
    , m_threadPool(new QThreadPool(this))
{
    m_threadPool->setMaxThreadCount(QThread::idealThreadCount() > 1
                                        ? QThread::idealThreadCount() - 1
                                        : QThread::idealThreadCount());
    connect(this, &QZXingNuFilter::qzxingNuChanged, this, [this]() {
        connect(m_qzxingNu, &QZXingNu::decodeResultChanged, this, &QZXingNuFilter::setDecodeResult);
    });
    connect(this, &QZXingNuFilter::decodeResultChanged, this,
            [this]() { emit tagFound(m_decodeResult.text); });
}

QVideoFilterRunnable *QZXingNuFilter::createFilterRunnable()
{
    return new QZXingNuFilterRunnable(this);
}

QZXingNu *QZXingNuFilter::qzxingNu() const
{
    return m_qzxingNu;
}

void QZXingNuFilter::setQzxingNu(QZXingNu *qzxingNu)
{
    if (m_qzxingNu == qzxingNu)
        return;

    m_qzxingNu = qzxingNu;
    emit qzxingNuChanged(m_qzxingNu);
}
} // namespace QZXingNu
