/****************************************************************************
**
**
****************************************************************************/

#include "OSCPort.h"

/**
 * Create an OSCPort that sends to newAddress, newPort
 * @param newAddress InetAddress
 * @param newPort int
 */
OSCPort::OSCPort(QHostAddress& LocalAddress, QHostAddress& remoteAddress, qint16 newPort, bool asListener, QObject * parent)
    :ibListening(false)
    , iDispatcher(0)
    , iConverter(0)
    , iSocket(0)
    , iPort(0)
{
    construct(LocalAddress, remoteAddress, newPort, asListener);
}

void OSCPort::construct(QHostAddress& LoAddr, QHostAddress& newAddress, qint16 newPort, bool asListener)
{
    qDebug() << "Setting up OSCPort";

    ibListening = false;
    iSocket  = new QUdpSocket(this);
    iDispatcher = new OSCPacketDispatcher();
    iConverter  = new OSCByteArrayToMsgConverter();

    iAddress = newAddress;
    iPort  = newPort;
    if (asListener && iSocket->bind(LoAddr, iPort, QAbstractSocket::ShareAddress | QAbstractSocket::ReuseAddressHint) == false) {
        qDebug() << "Error binding to OSC port.";
    }
}

OSCPort::~OSCPort()
{
    iSocket->close();
    delete iSocket;
    delete iDispatcher;
    delete iConverter;
}

void OSCPort::run()
{
    QByteArray datagram(1536,0);
   while(ibListening)
    {
        iSocket->waitForConnected();

        if(!iSocket->waitForReadyRead()) {
            qDebug() << "Error with waitForReadyRead.";
            QThread::sleep(1);
            continue;
        }

        if(!iSocket->hasPendingDatagrams()) continue;

        qint32 mbytesLength;
        mbytesLength = iSocket->readDatagram(datagram.data(),1536);
        if(mbytesLength != -1)
        {
            OSCPacket * oscPacket = iConverter->convert(&datagram,mbytesLength);
            iDispatcher->dispatchPacket(oscPacket);
            delete oscPacket;
        }
    }
}

/**
 * Start listening for incoming OSCPackets
 */
void OSCPort::startListening()
{
    if(!ibListening)
    {
        connect(iSocket,SIGNAL(readyRead()),this,SLOT(onReadyRead()) );
        //start();
        //ibListening = true;
    }
}

void OSCPort::stopListening()
{
    disconnect(iSocket,SIGNAL(readyRead()),this,SLOT(onReadyRead()) );
    ibListening = false;
}

/**
 * Register the listener for incoming OSCPackets addressed to an Address
 * @param anAddress  the address to listen for
 * @param listener   the object to invoke when a message comes in
 */
void OSCPort::addListener(QString& anAddress, OSCListener* listener)
{
    iDispatcher->addListener(anAddress, listener);
}

/**
 * Send an osc packet (message or bundle) to the receiver I am bound to.
 * @param aPacket OSCPacket
 */
void OSCPort::send(OSCPacket& aPacket)
{
    QByteArray& byteArray = aPacket.getByteArray();
    qDebug() << "Writing OSC packet:" << byteArray.toPercentEncoding() << " to " << iAddress.toString() << " port" << iPort;
    iSocket->writeDatagram(byteArray, iAddress, iPort);
}

void OSCPort::onReadyRead()
{
    QByteArray datagram(1536,0);

        if(!iSocket->hasPendingDatagrams()) return;

        qint32 mbytesLength;
        mbytesLength = iSocket->readDatagram(datagram.data(),1536);
        if(mbytesLength != -1)
        {
            OSCPacket * oscPacket = iConverter->convert(&datagram,mbytesLength);
            iDispatcher->dispatchPacket(oscPacket);
            delete oscPacket;
        }
}
