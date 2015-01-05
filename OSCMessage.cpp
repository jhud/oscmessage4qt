/****************************************************************************
**
**
****************************************************************************/

#include "OSCMessage.h"

/**
 * Create an empty OSC Message.
 * In order to send this osc message, you need to set the address
 * and, perhaps, some arguments.
 */
OSCMessage::OSCMessage()
 : OSCPacket()
{
    iArguments = new QVector<QVariant>(0);
    init(false);
}

/**
 * Create an OSCMessage with an address already initialized.
 * @param newAddress the recepient of this OSC message
 */
OSCMessage::OSCMessage(const QString &newAddress)
{
    QList<QVariant> marguments;
    construct(newAddress, marguments);
}

/**
 * Create an OSCMessage with an address and arguments already initialized.
 * @param newAddress    the recepient of this OSC message
 * @param newArguments  the data sent to the receiver
 */
OSCMessage::OSCMessage(const QString &newAddress, QList<QVariant>& newArguments)
{
    construct(newAddress, newArguments);
}

void OSCMessage::construct(const QString& newAddress, QList<QVariant>& newArguments)
{
    iAddress = newAddress;
    if(newArguments.length())
    {
        iArguments = new QVector<QVariant>(0);
        for(int i = 0; i < newArguments.length(); i++)
        {
            iArguments->append(newArguments.at(i));
        }
    }else
        iArguments = new QVector<QVariant>(0);

    init(false);
}

OSCMessage::~OSCMessage()
{
    iArguments->clear();
    delete iArguments;
}

/**
 * The receiver of this message.
 * @return the receiver of this OSC Message
 */
QString OSCMessage::getAddress()
{
    return iAddress;
}

/**
 * Set the address of this messsage.
 * @param anAddress the receiver of the message
 */
void OSCMessage::setAddress(QString& anAddress)
{
    iAddress = anAddress;
}

/**
 * Add an argument to the list of arguments.
 * @param argument a Float, String, Integer, BigInteger, Boolean or array of these
 */
void OSCMessage::addArgument(QVariant argument)
{
    if(!argument.isNull())
    {
        iArguments->append(argument);
    }
}

/**
 * The arguments of this message.
 * @return the arguments to this message
 */
QList<QVariant> OSCMessage::getArguments()
{
    return iArguments->toList();
}

/**
 * Convert the address into a byte array. Used internally.
 * @param stream OscPacketByteArrayConverter
 */
void OSCMessage::computeAddressByteArray(OSCMsgToByteArrayConverter& stream)
{
    stream.write(iAddress);
}

/**
 * Convert the arguments into a byte array. Used internally.
 * @param stream OscPacketByteArrayConverter
 */
void OSCMessage::computeArgumentsByteArray(OSCMsgToByteArrayConverter& stream)
{
    stream.write(',');
    if(iArguments->isEmpty()) return;

    stream.writeTypes(*iArguments);
    for(int i = 0; i < iArguments->count(); i++)
    {
        stream.write((QVariant&)iArguments->at(i));
    }
}

/**
 * Convert the message into a byte array. Used internally.
 * @param stream OscPacketByteArrayConverter
 */
void OSCMessage::computeByteArray(OSCMsgToByteArrayConverter& stream)
{
    computeAddressByteArray(stream);
    computeArgumentsByteArray(stream);
    //byteArray = stream.toByteArray();
}
