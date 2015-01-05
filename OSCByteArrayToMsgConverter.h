/****************************************************************************
**
**
****************************************************************************/

#ifndef OSCBATOJC_H
#define OSCBATOJC_H

#include "OSCBundle.h"
#include "OSCMessage.h"

class OSCByteArrayToMsgConverter
{
public:
    OSCByteArrayToMsgConverter();
    ~OSCByteArrayToMsgConverter();

    OSCPacket * convert(QByteArray* byteArray, qint32 bytesLength);

protected:

private:
    bool isBundle();
    unsigned char nextByte();

    OSCBundle *convertBundle();
    OSCMessage *convertMessage();
    QString readString();
    QByteArray readTypes();
    QVariant readArgument(char ac);
    QVariant readChar();
    QVariant readDouble();
    QVariant readFloat();
    QVariant readBigInteger();
    QVariant readInteger();
    QDateTime readTimeTag();
    QList<QVariant> readArray(QByteArray& types, int i);
    qint32 lengthOfCurrentString();
    void moveToFourByteBoundry();

    QByteArray* iBytes;
    qint32 iBytesLength;
    qint32 iStreamPosition;

};

#endif
