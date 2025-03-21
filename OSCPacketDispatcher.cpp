/****************************************************************************
**
**
****************************************************************************/

#include "OSCPacketDispatcher.h"

OSCPacketDispatcher::OSCPacketDispatcher()
{
    iAddressToClassTable = new QHash<QString,OSCListener *>();
}

OSCPacketDispatcher::~OSCPacketDispatcher()
{
    iAddressToClassTable->clear();
    delete iAddressToClassTable;
}

void OSCPacketDispatcher::addListener(QString& address, OSCListener* listener)
{
    iAddressToClassTable->insert(address, listener);
}

void OSCPacketDispatcher::dispatchPacket(OSCPacket * packet, QDateTime* timestamp, QHostAddress & from)
{
    if(packet->isBundle)
    {
        dispatchBundle(dynamic_cast<OSCBundle*>(packet), from);
    }else
    {
        if(timestamp)
        {
            dispatchMessage(dynamic_cast<OSCMessage*>(packet), *timestamp, from);
        }else
        {
            dispatchMessage(dynamic_cast<OSCMessage*>(packet), from);
        }
    }
}

void OSCPacketDispatcher::dispatchBundle(OSCBundle* bundle, QHostAddress & from)
{
    QDateTime& timestamp = bundle->getTimestamp();
    QList<OSCPacket*>& packets = bundle->getPackets();
    for (int i = 0; i < packets.length(); i++)
    {
            dispatchPacket((OSCPacket*)packets.at(i), &timestamp, from);
    }
}

void OSCPacketDispatcher::dispatchMessage(OSCMessage* message, QHostAddress & from)
{
    QDateTime dtimeNull = QDateTime();
    dispatchMessage(message,dtimeNull, from);
}

void OSCPacketDispatcher::dispatchMessage(OSCMessage* message, QDateTime& time, QHostAddress &from)
{
    QList<QString> mkeys = iAddressToClassTable->keys();
    for(int i=0; i < mkeys.length(); i++)
    {
        const QString& addresskey = mkeys.at(i);
        if(message->getAddress().startsWith(addresskey))
        {
            OSCListener * olistener = iAddressToClassTable->value(addresskey);
            olistener->acceptMessage(time,message, from);
        }
    }
}
