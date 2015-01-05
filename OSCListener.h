/****************************************************************************
**
**
****************************************************************************/

#ifndef OSCLISTENER_H
#define OSCLISTENER_H

#include <QDateTime.h>
class OSCMessage;

class OSCListener
{
public:

    virtual void acceptMessage(QDateTime& time, OSCMessage* message) = 0;

};

#endif
