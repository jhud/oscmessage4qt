/****************************************************************************
**
**
****************************************************************************/

#include "OSCBundle.h"

OSCBundle::OSCBundle()
{
    QList<OSCPacket*> mpackets;
    construct(mpackets,(QDateTime&)TIMESTAMP_IMMEDIATE);
}

OSCBundle::OSCBundle(QDateTime& timestamp)
{
    QList<OSCPacket*> mpackets;
    construct(mpackets,timestamp);
}

OSCBundle::OSCBundle(QList<OSCPacket*>& packets)
{
    construct(packets,(QDateTime&)TIMESTAMP_IMMEDIATE);
}

OSCBundle::OSCBundle(QList<OSCPacket *> &packets, QDateTime& timestamp)
{
    construct(packets, timestamp);
}

void OSCBundle::construct(QList<OSCPacket *> &packets, QDateTime& timestamp)
{
    if(packets.isEmpty())
    {
        iPackets = new QVector<OSCPacket*>();
    }else
    {
        iPackets = new QVector<OSCPacket*>(packets.length());
        for(int i = 0; i < packets.length(); i++)
        {
            iPackets->append(packets.at(i));
        }
    }
    iTimestamp = timestamp;
    init(true);
}

OSCBundle::~OSCBundle()
{
    iPackets->clear();
    delete iPackets;
}

/**
 * Get the packets contained in this bundle.
 * @return an array of packets
 */
QList<OSCPacket *> &OSCBundle::getPackets()
{
    iPacketList = iPackets->toList();
    return iPacketList;
}

/**
 * Convert the timetag (a Java Date) into the OSC byte stream. Used Internally.
 */
void OSCBundle::computeTimeTagByteArray(OSCMsgToByteArrayConverter& stream)
{
    if(iTimestamp.isNull()||iTimestamp==TIMESTAMP_IMMEDIATE)
    {
        stream.write((qint32)0);
        stream.write((qint32)1);
        return;
    }
    long secsSince1970 = iTimestamp.toTime_t();
    long secs = secsSince1970 + SECONDS_FROM_1900_to_1970;
    long fraction = 0;

    stream.write((qint32)secs);
    stream.write((qint32)fraction);
}

/**
 * Compute the OSC byte stream representation of the bundle. Used Internally.
 * @param stream OscPacketByteArrayConverter
 */
void OSCBundle::computeByteArray(OSCMsgToByteArrayConverter& stream)
{
    QString bdstr("#bundle");
    stream.write(bdstr);
    computeTimeTagByteArray(stream);
    for(int i = 0; i < iPackets->count(); i++)
    {
        OSCPacket& mpackets = (OSCPacket&)iPackets->at(i);
        QByteArray& objBytearray = mpackets.getByteArray();
        stream.write(objBytearray.length());
        stream.write(objBytearray);
    }
    //byteArray = stream.toByteArray();
}
