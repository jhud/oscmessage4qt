/****************************************************************************
**
**
****************************************************************************/

#ifndef OSCPACKETDSPCH_H
#define OSCPACKETDSPCH_H

#include "OSCListener.h"
#include "OSCBundle.h"
#include"OSCMessage.h"
//#include <QHash>

#include <QHostAddress>

class OSCPacketDispatcher
{
public:
    OSCPacketDispatcher();
    ~OSCPacketDispatcher();

    void addListener(QString& address, OSCListener *listener);
    void dispatchPacket(OSCPacket *packet, QDateTime* timestamp, QHostAddress &from);

private:
    void dispatchBundle(OSCBundle *bundle, QHostAddress &from);
    void dispatchMessage(OSCMessage* message, QHostAddress &from);
    void dispatchMessage(OSCMessage* message, QDateTime& time, QHostAddress &from);

    QHash<QString,OSCListener*>* iAddressToClassTable;

};

#endif
