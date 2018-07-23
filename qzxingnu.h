#ifndef QZXINGNU_H
#define QZXINGNU_H

#include <QImage>
#include <QObject>
#include <memory>
#include <zxing-cpp/core/src/Result.h>

using spZXingResult = std::shared_ptr<ZXing::Result>;

class QZXingNu : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector<int> formats READ formats WRITE setFormats NOTIFY formatsChanged)
    Q_PROPERTY(bool tryHarder READ tryHarder WRITE setTryHarder NOTIFY tryHarderChanged)
    Q_PROPERTY(bool tryRotate READ tryRotate WRITE setTryRotate NOTIFY tryRotateChanged)
    QVector<int> m_formats;

    bool m_tryHarder = false;
    bool m_tryRotate = false;

public:
    enum class BarcodeFormat {
        /** Aztec 2D barcode format. */
        AZTEC,

        /** CODABAR 1D format. */
        CODABAR,

        /** Code 39 1D format. */
        CODE_39,

        /** Code 93 1D format. */
        CODE_93,

        /** Code 128 1D format. */
        CODE_128,

        /** Data Matrix 2D barcode format. */
        DATA_MATRIX,

        /** EAN-8 1D format. */
        EAN_8,

        /** EAN-13 1D format. */
        EAN_13,

        /** ITF (Interleaved Two of Five) 1D format. */
        ITF,

        /** MaxiCode 2D barcode format. */
        MAXICODE,

        /** PDF417 format. */
        PDF_417,

        /** QR Code 2D barcode format. */
        QR_CODE,

        /** RSS 14 */
        RSS_14,

        /** RSS EXPANDED */
        RSS_EXPANDED,

        /** UPC-A 1D format. */
        UPC_A,

        /** UPC-E 1D format. */
        UPC_E,

        /** UPC/EAN extension format. Not a stand-alone format. */
        UPC_EAN_EXTENSION,

        // Not valid value, used to count the number of formats, thus should be always the last
        // listed here
        FORMAT_COUNT,
    };
    Q_ENUM(BarcodeFormat)
    explicit QZXingNu(QObject *parent = nullptr);
    QVector<int> formats() const;
    bool tryHarder() const;
    bool tryRotate() const;
signals:
    void imageDecoded(const QString data);
    void formatsChanged(QVector<int> formats);
    void tryHarderChanged(bool tryHarder);
    void tryRotateChanged(bool tryRotate);

public slots:
    spZXingResult decodeImage(const QImage &image);
    void setFormats(QVector<int> formats);
    void setTryHarder(bool tryHarder);
    void setTryRotate(bool tryRotate);
};

#endif // QZXINGNU_H
