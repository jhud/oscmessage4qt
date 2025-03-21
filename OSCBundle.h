/****************************************************************************
**
**
****************************************************************************/

#ifndef OSCBUNDLE_H
#define OSCBUNDLE_H

#include <QDateTime>
#include "OSCPacket.h"
#include "OSCms_global.h"


const QDateTime TIMESTAMP_IMMEDIATE(QDate(1970,1,1));

class OSCMSSHARED_EXPORT OSCBundle : public OSCPacket
{
public:
    static const unsigned long SECONDS_FROM_1900_to_1970 = 2208988800;

    OSCBundle();
    OSCBundle(QDateTime& timestamp);
    OSCBundle(QList<OSCPacket *> &packets);
    OSCBundle(QList<OSCPacket*>& packets, QDateTime& timestamp);
    void construct(QList<OSCPacket*>& packets, QDateTime& timestamp);

    ~OSCBundle();

    QDateTime& getTimestamp() { return iTimestamp; }
    void setTimestamp(QDateTime& timestamp) { iTimestamp = timestamp; }
    void addPacket(OSCPacket * packet) { iPackets->append(packet); }

    QList<OSCPacket*>& getPackets();

protected:
    void computeTimeTagByteArray(OSCMsgToByteArrayConverter& stream);
    void computeByteArray(OSCMsgToByteArrayConverter& stream);

    QVector<OSCPacket*>* iPackets;
    QList<OSCPacket*> iPacketList;
    QDateTime iTimestamp;

};

#endif
