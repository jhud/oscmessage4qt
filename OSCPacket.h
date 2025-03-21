/****************************************************************************
**
**
****************************************************************************/

#ifndef OSCPACKET_H
#define OSCPACKET_H

#include "OSCMsgToByteArrayConverter.h"
#include <QVariant>
#include <QVector>


class OSCPacket
{
public:
    OSCPacket();
    virtual ~OSCPacket();

    QByteArray& getByteArray();

    bool isBundle;

protected:
    void computeByteArray();
    virtual void computeByteArray(OSCMsgToByteArrayConverter& /*stream*/) = 0;
    void init(bool bundflag) { isBundle = bundflag; };

    bool isByteArrayComputed;
    QByteArray iByteArray;
};

#endif
