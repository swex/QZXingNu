#include "qzxingnu.h"
#include <QDebug>
#include <zxing-cpp/core/src/BarcodeFormat.h>
#include <zxing-cpp/core/src/DecodeHints.h>
#include <zxing-cpp/core/src/GenericLuminanceSource.h>
#include <zxing-cpp/core/src/HybridBinarizer.h>
#include <zxing-cpp/core/src/MultiFormatReader.h>
#include <zxing-cpp/core/src/Result.h>

using ZXing::BarcodeFormat;
using ZXing::DecodeHints;
using ZXing::GenericLuminanceSource;
using ZXing::HybridBinarizer;
using ZXing::MultiFormatReader;
using ZXing::Result;

QZXingNu::QZXingNu(QObject *parent)
    : QObject(parent)
{
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

static std::vector<BarcodeFormat> zxingFormats(const QVector<int> &from)
{
    std::vector<BarcodeFormat> result;
    result.reserve(from.size());
    std::transform(from.begin(), from.end(), std::back_inserter(result),
                   [](int a) { return static_cast<ZXing::BarcodeFormat>(a); });
    return result;
}

spZXingResult QZXingNu::decodeImage(const QImage &image)
{
    auto generic = std::make_shared<GenericLuminanceSource>(
        image.width(), image.height(), image.bits(), image.width() * 4, 4, 0, 1, 2);
    DecodeHints hints;
    hints.setPossibleFormats(zxingFormats(formats()));
    hints.setShouldTryHarder(m_tryHarder);
    hints.setShouldTryRotate(m_tryRotate);
    MultiFormatReader reader(hints);
    auto result = reader.read(HybridBinarizer(generic));
    if (result.isValid()) {
        emit imageDecoded(QString::fromStdWString(result.text()));
    }
    return std::make_shared<ZXing::Result>(std::move(result));
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
