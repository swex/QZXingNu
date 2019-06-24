#include "qzxingnu.h"
#include <QCoreApplication>
#include <QtQml/QQmlEngine>
#include "qzxingnufilter.h"
#include <zxing-cpp/core/src/BarcodeFormat.h>
#include <zxing-cpp/core/src/DecodeHints.h>
#include <zxing-cpp/core/src/GenericLuminanceSource.h>
#include <zxing-cpp/core/src/HybridBinarizer.h>
#include <zxing-cpp/core/src/MultiFormatReader.h>
#include <zxing-cpp/core/src/Result.h>

using ZXingFormats = std::vector<ZXing::BarcodeFormat>;
using ZXing::DecodeHints;
using ZXing::GenericLuminanceSource;
using ZXing::HybridBinarizer;
using ZXing::MultiFormatReader;
using ZXing::Result;

static QVector<QPointF> toQVectorOfQPoints(const std::vector<ZXing::ResultPoint> &points)
{
    QVector<QPointF> result;
    for (const auto &point : points) {
        result.append(QPointF(point.x(), point.y()));
    }
    return result;
}

static QZXingNu::DecodeResult toQZXingNuDecodeResult(const ZXing::Result &result)
{
    return { static_cast<QZXingNu::DecodeStatus>(result.status()),
             static_cast<QZXingNu::BarcodeFormat>(result.format()),
             QString::fromStdWString(result.text()),
             QByteArray(result.rawBytes().charPtr(), result.rawBytes().length()),
             toQVectorOfQPoints(result.resultPoints()),
             result.isValid() };
}

static ZXingFormats zxingFormats(const QVector<int> &from)
{
    ZXingFormats result;
    result.reserve(static_cast<ZXingFormats::size_type>(from.size()));
    std::transform(from.begin(), from.end(), std::back_inserter(result),
                   [](int a) { return static_cast<ZXing::BarcodeFormat>(a); });
    return result;
}

QZXingNu::QZXingNu(QObject *parent)
    : QObject(parent)
{
    connect(this, &QZXingNu::queueDecodeResult, this, &QZXingNu::setDecodeResult);
}

QVector<int> QZXingNu::formats() const
{
    return m_formats;
}

bool QZXingNu::tryHarder() const
{
    return m_tryHarder;
}

bool QZXingNu::tryRotate() const
{
    return m_tryRotate;
}

QZXingNu::DecodeResult QZXingNu::decodeResult() const
{
    return m_decodeResult;
}

void QZXingNu::registerQMLTypes()
{
    qRegisterMetaType<QZXingNu::DecodeResult>("DecodeResult");
    qRegisterMetaType<QZXingNu::DecodeStatus>("DecodeStatus");
    qRegisterMetaType<QZXingNu::BarcodeFormat>("BarcodeFormat");

    qmlRegisterUncreatableMetaObject(QZXingNu::staticMetaObject, "com.github.swex.QZXingNu", 1, 0,
                                     "QZXingNu", "Error: only enums allowed");
    qmlRegisterType<QZXingNuFilter>("com.github.swex.QZXingNu", 1, 0, "QZXingNuFilter");
    qmlRegisterType<QZXingNu>("com.github.swex.QZXingNu", 1, 0, "QZXingNu");
}

QZXingNu::DecodeResult QZXingNu::decodeImage(const QImage &image)
{
    // reentrant
    auto generic = std::make_shared<GenericLuminanceSource>(
        image.width(), image.height(), image.bits(), image.width() * 4, 4, 0, 1, 2);
    DecodeHints hints;
    auto convertFormats = [this]() { return zxingFormats(m_formats); };
    hints.setPossibleFormats(convertFormats());
    hints.setShouldTryHarder(m_tryHarder);
    hints.setShouldTryRotate(m_tryRotate);
    MultiFormatReader reader(hints);
    auto result = reader.read(HybridBinarizer(generic));
    if (result.isValid()) {
        auto qzxingResult = toQZXingNuDecodeResult(result);
        emit queueDecodeResult(qzxingResult);
        return qzxingResult;
    }
    return {};
}

void QZXingNu::setFormats(QVector<int> formats)
{
    if (m_formats == formats)
        return;

    m_formats = formats;
    emit formatsChanged(m_formats);
}

void QZXingNu::setTryHarder(bool tryHarder)
{
    if (m_tryHarder == tryHarder)
        return;

    m_tryHarder = tryHarder;
    emit tryHarderChanged(m_tryHarder);
}

void QZXingNu::setTryRotate(bool tryRotate)
{
    if (m_tryRotate == tryRotate)
        return;

    m_tryRotate = tryRotate;
    emit tryRotateChanged(m_tryRotate);
}

void QZXingNu::setDecodeResult(QZXingNu::DecodeResult decodeResult)
{
    m_decodeResult = decodeResult;
    emit decodeResultChanged(m_decodeResult);
}
