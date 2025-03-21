/****************************************************************************
**
**
****************************************************************************/

#ifndef OSCPORT_H
#define OSCPORT_H

#include <QThread>
#include <QUdpSocket>
#include "OSCPacketDispatcher.h"
#include "OSCByteArrayToMsgConverter.h"
#include "OSCms_global.h"

class OSCMSSHARED_EXPORT OSCPort : public QObject
{
    Q_OBJECT

public:
    explicit OSCPort(QHostAddress& LocalAddress, QHostAddress& remoteAddress, qint16 newPort, bool asListener, QObject * parent = 0);
    virtual ~OSCPort();

    /**
     * The port that the SuperCollider <b>synth</b> engine ususally listens to &mdash; 57110.
     */
    inline static qint16 defaultSCOSCPort()     { return 57110; }
    /**
     * The port that the SuperCollider <b>language</b> engine ususally listens to &mdash; 57120.
     */
    inline static qint16 defaultSCLangOSCPort() { return 57120; }
    /**
     * Close the socket and free-up resources. It's recommended that clients call
     * this when they are done with the port.
     */
    inline void close() { iSocket->close(); }

    void startListening();
    void stopListening();
    bool isListening() { return ibListening; }
    void addListener(QString& anAddress, OSCListener *listener);
    void send(OSCPacket& aPacket);
    void construct(QHostAddress& LocalAddress, QHostAddress& newAddress, qint16 newPort, bool asListener);

private slots:
    void onReadyRead();

protected:
    void run();

    QHostAddress iAddress;
    bool ibListening;
    OSCPacketDispatcher* iDispatcher;
    OSCByteArrayToMsgConverter* iConverter;

    QUdpSocket* iSocket;
    qint16 iPort;
};

#endif
