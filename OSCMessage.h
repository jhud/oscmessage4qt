/****************************************************************************
**
**
****************************************************************************/

#ifndef OSCMESSAGE_H
#define OSCMESSAGE_H

#include "OSCPacket.h"
#include "OSCms_global.h"


class OSCMSSHARED_EXPORT OSCMessage : public OSCPacket
{
public:
    OSCMessage();
    OSCMessage(const QString& newAddress);
    OSCMessage(const QString& newAddress, QList<QVariant>& newArguments);
    void construct(const QString& newAddress, QList<QVariant>& newArguments);

    ~OSCMessage();

    QString getAddress();
    void setAddress(QString& anAddress);
    void addArgument(QVariant argument);
    QList<QVariant> getArguments();

protected:
    void computeAddressByteArray(OSCMsgToByteArrayConverter& stream);
    void computeArgumentsByteArray(OSCMsgToByteArrayConverter& stream);
    void computeByteArray(OSCMsgToByteArrayConverter& stream);

    QVector<QVariant>* iArguments;
    QString iAddress;
};

#endif
