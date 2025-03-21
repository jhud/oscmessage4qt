/****************************************************************************
**
**
****************************************************************************/

#ifndef OSCJTOBAC_H
#define OSCJTOBAC_H

#include <QVariant>
#include <QVector>

class OSCMsgToByteArrayConverter
{
public:
    OSCMsgToByteArrayConverter(QByteArray& byteArray);
    ~OSCMsgToByteArrayConverter();

    void appendNullCharToAlignStream();
    void write(QByteArray& bytes);
    void write(qint32 ni);
    void write(QChar achar);
    void write(double& dba);
    void write(float& fa);
    void write(qint64& la);
    void write(QString& aString);
    void writeBlob(QByteArray& strArray);
    void write(char c);
    void write(QVariant& anObject);
    void writeType(QVariant& anObject);
    void writeTypesArray(QList<QVariant>& array);
    void writeTypes(QVector<QVariant>& vector);

protected:
    QByteArray& iByteArray;

private:
    QByteArray& alignBigEndToFourByteBoundry(QByteArray& bytes);
    void writeUnderHandler(QByteArray& bytes);
    void writeInteger32ToByteArray(qint32 value);
    void writeInteger64ToByteArray(qint64& value);

    qint8 longintBytes[8];
};

#endif
