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

void OSCPacketDispatcher::dispatchPacket(OSCPacket * packet, QDateTime* timestamp)
{
    if(packet->isBundle)
    {
        dispatchBundle(dynamic_cast<OSCBundle*>(packet));
    }else
    {
        if(timestamp)
        {
            dispatchMessage(dynamic_cast<OSCMessage*>(packet), *timestamp);
        }else
        {
            dispatchMessage(dynamic_cast<OSCMessage*>(packet));
        }
    }
}

void OSCPacketDispatcher::dispatchBundle(OSCBundle* bundle)
{
    QDateTime& timestamp = bundle->getTimestamp();
    QList<OSCPacket*>& packets = bundle->getPackets();
    for (int i = 0; i < packets.length(); i++)
    {
            dispatchPacket((OSCPacket*)packets.at(i), &timestamp);
    }
}

void OSCPacketDispatcher::dispatchMessage(OSCMessage* message)
{
    QDateTime dtimeNull = QDateTime();
    dispatchMessage(message,dtimeNull);
}

void OSCPacketDispatcher::dispatchMessage(OSCMessage* message, QDateTime& time)
{
    QList<QString> mkeys = iAddressToClassTable->keys();
    for(int i=0; i < mkeys.length(); i++)
    {
        const QString& addresskey = mkeys.at(i);
        if(addresskey==message->getAddress())
        {
            OSCListener * olistener = iAddressToClassTable->value(addresskey);
            olistener->acceptMessage(time,message);
        }
    }
}
