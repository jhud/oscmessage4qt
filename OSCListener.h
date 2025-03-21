/****************************************************************************
**
**
****************************************************************************/

#ifndef OSCLISTENER_H
#define OSCLISTENER_H

#include <QHostAddress>

//#include <QDateTime.h>
class OSCMessage;

class OSCListener
{
public:

    virtual void acceptMessage(class QDateTime& time, OSCMessage* message, QHostAddress & from) = 0;

};

#endif
