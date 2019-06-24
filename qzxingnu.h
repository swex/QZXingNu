#ifndef QZXINGNU_H
#define QZXINGNU_H

#include <QImage>
#include <QObject>
#include <memory>

class QZXingNu : public QObject
{
    Q_OBJECT

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

    enum class DecodeStatus {
        NoError = 0,
        NotFound,
        FormatError,
        ChecksumError,
    };
    Q_ENUM(DecodeStatus)

    struct DecodeResult {
        QZXingNu::DecodeStatus status;
        QZXingNu::BarcodeFormat format;
        QString text;
        QByteArray rawBytes;
        QVector<QPointF> points;
        bool valid;
    };

    Q_PROPERTY(QVector<int> formats READ formats WRITE setFormats NOTIFY formatsChanged)
    Q_PROPERTY(bool tryHarder READ tryHarder WRITE setTryHarder NOTIFY tryHarderChanged)
    Q_PROPERTY(bool tryRotate READ tryRotate WRITE setTryRotate NOTIFY tryRotateChanged)
    Q_PROPERTY(QZXingNu::DecodeResult decodeResult READ decodeResult NOTIFY decodeResultChanged)
    QVector<int> m_formats;
    bool m_tryHarder = false;
    bool m_tryRotate = false;
    QZXingNu::DecodeResult m_decodeResult;

public:
    explicit QZXingNu(QObject *parent = nullptr);
    QVector<int> formats() const;
    bool tryHarder() const;
    bool tryRotate() const;
    QZXingNu::DecodeResult decodeResult() const;

    static void registerQMLTypes();

signals:
    void imageDecoded(QString data);
    void formatsChanged(QVector<int> formats);
    void tryHarderChanged(bool tryHarder);
    void tryRotateChanged(bool tryRotate);
    void decodeResultChanged(QZXingNu::DecodeResult decodeResult);
    void queueDecodeResult(QZXingNu::DecodeResult result);

public slots:
    QZXingNu::DecodeResult decodeImage(const QImage &image);
    void setFormats(QVector<int> formats);
    void setTryHarder(bool tryHarder);
    void setTryRotate(bool tryRotate);

protected:
    void setDecodeResult(QZXingNu::DecodeResult decodeResult);
};
Q_DECLARE_METATYPE(QZXingNu::BarcodeFormat)
Q_DECLARE_METATYPE(QZXingNu::DecodeStatus)
Q_DECLARE_METATYPE(QZXingNu::DecodeResult)

#endif // QZXINGNU_H
