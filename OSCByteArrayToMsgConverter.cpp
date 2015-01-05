/****************************************************************************
**
**
****************************************************************************/

#include "OSCByteArrayToMsgConverter.h"

OSCByteArrayToMsgConverter::OSCByteArrayToMsgConverter()
{
}

OSCByteArrayToMsgConverter::~OSCByteArrayToMsgConverter()
{
}

/**
 * Convert a byte array into an OSCPacket (either an OSCMessage or OSCBundle).
 * @return an OSCPacket
 */
OSCPacket * OSCByteArrayToMsgConverter::convert(QByteArray* byteArray, qint32 btLength)
{
    iBytes  = byteArray;
    iBytesLength = btLength;
    iStreamPosition = 0;
    if(isBundle())
            return convertBundle();
    else
            return convertMessage();
}

/**
 * Is my byte array a bundle?
 * @return true if it the byte array is a bundle, false o.w.
 */
bool OSCByteArrayToMsgConverter::isBundle()
{
    return iBytes->startsWith("#bundle");
}

/**
 * Convert the byte array a bundle. Assumes that the byte array is a bundle.
 * @return a bundle containing the data specified in the byte stream
 */
OSCBundle * OSCByteArrayToMsgConverter::convertBundle()
{
    iStreamPosition = 8;
    QDateTime timestamp = readTimeTag();
    OSCBundle* bundle = new OSCBundle(timestamp);
    OSCByteArrayToMsgConverter* myConverter = new OSCByteArrayToMsgConverter();
    while(iStreamPosition < iBytesLength) {
            // recursively read through the stream and convert packets you find
            qint32 packetLength = readInteger().toInt();
            QByteArray packetBytes(iBytes->constData()+iStreamPosition,packetLength);
            OSCPacket * packet = myConverter->convert(&packetBytes, packetLength);
            bundle->addPacket(packet);
    }
    delete myConverter;
    return bundle;
}

/**
 * Convert the byte array a simple message. Assumes that the byte array is a message.
 * @return a message containing the data specified in the byte stream
 */
OSCMessage * OSCByteArrayToMsgConverter::convertMessage()
{
    OSCMessage* message = new OSCMessage();
    QString addrStr = readString();
    message->setAddress(addrStr);
    QByteArray types = readTypes();
    if(types.isNull()) return message;

    moveToFourByteBoundry();
    for(int i = 0; i < types.length(); i++) {
        if('[' == types.at(i))
        {
            // we're looking at an array -- read it in
            message->addArgument(readArray(types, ++i));
            // then increment i to the end of the array
            while(']' != types.at(i)) i++;
        }else
            message->addArgument(readArgument(types.at(i)));
    }
    return message;
}

/**
 * Read a string from the byte stream.
 * @return the next string in the byte stream
 */
QString OSCByteArrayToMsgConverter::readString()
{
    qint32 strLen = lengthOfCurrentString();
    QByteArray stringChars;
    for(int i = 0; i < strLen; i++)
    {
        stringChars.append(iBytes->at(iStreamPosition++));
    }
    qDebug("readString:%s",QString(stringChars).toLatin1().constData());
    moveToFourByteBoundry();
    return QString(stringChars);
}

/**
 * Read the types of the arguments from the byte stream.
 * @return a char array with the types of the arguments
 */
QByteArray OSCByteArrayToMsgConverter::readTypes()
{
    QByteArray typesChars;
    if(iBytes->at(iStreamPosition)!= 0x2C) return typesChars;

    iStreamPosition++;
    // find out how long the list of types is
    qint32 typesLen = lengthOfCurrentString();
    if(!typesLen) return typesChars;

    // read in the types
    for(int i = 0; i < typesLen; i++)
    {
        typesChars.append(iBytes->at(iStreamPosition++));
    }
    qDebug("readTypes:%s",QString(typesChars).toLatin1().constData());
    return typesChars;
}

/**
 * Read an object of the type specified by the type char.
 * @param c type of argument to read
 * @return a Java representation of the argument
 */
QVariant OSCByteArrayToMsgConverter::readArgument(char ac)
{
    switch(ac)
    {
        case 'i' :
                return readInteger();
        case 'h' :
                return readBigInteger();
        case 'f' :
                return readFloat();
        case 'd' :
                return readDouble();
        case 's' :
                return readString();
        case 'c' :
                return readChar();
        case 'T' :
                return QVariant(true);
        case 'F' :
                return QVariant(false);
    }
    return QVariant();
}

/**
 * Read a char from the byte stream.
 * @return a Character
 */
QVariant OSCByteArrayToMsgConverter::readChar()
{
    return QVariant(iBytes->at(iStreamPosition++));
}

/**
 * Read a double &mdash; this just read a float.
 * @return a Double
 */
QVariant OSCByteArrayToMsgConverter::readDouble()
{
    return readFloat();
}

/**
 * Read a float from the byte stream.
 * @return a Float
 */
QVariant OSCByteArrayToMsgConverter::readFloat()
{
    quint32 floatBytes = nextByte();
    floatBytes = (floatBytes<<8)| nextByte();
    floatBytes = (floatBytes<<8)| nextByte();
    floatBytes = (floatBytes<<8)| nextByte();

    float * floatCast = (float*)&floatBytes;

    qDebug("readFloat:%f",*floatCast);
    return QVariant(*floatCast);
}

unsigned char OSCByteArrayToMsgConverter::nextByte() {
    return iBytes->at(iStreamPosition++);
}
/**
 * Read a Big Integer (64 bit int) from the byte stream.
 * @return a BigInteger
 */
QVariant OSCByteArrayToMsgConverter::readBigInteger()
{
    qint64 longintBytes = iBytes->at(iStreamPosition++);
    longintBytes = (longintBytes<<8)| iBytes->at(iStreamPosition++);
    longintBytes = (longintBytes<<8)| iBytes->at(iStreamPosition++);
    longintBytes = (longintBytes<<8)| iBytes->at(iStreamPosition++);
    longintBytes = (longintBytes<<8)| iBytes->at(iStreamPosition++);
    longintBytes = (longintBytes<<8)| iBytes->at(iStreamPosition++);
    longintBytes = (longintBytes<<8)| iBytes->at(iStreamPosition++);
    longintBytes = (longintBytes<<8)| iBytes->at(iStreamPosition++);
    return QVariant(longintBytes);
}

/**
 * Read an Integer (32 bit int) from the byte stream.
 * @return an Integer
 */
QVariant OSCByteArrayToMsgConverter::readInteger()
{
    qint32 intBytes = iBytes->at(iStreamPosition++);
    intBytes = (intBytes<<8)| iBytes->at(iStreamPosition++);
    intBytes = (intBytes<<8)| iBytes->at(iStreamPosition++);
    intBytes = (intBytes<<8)| iBytes->at(iStreamPosition++);
    qDebug("readInteger:%d",intBytes);
    return QVariant(intBytes);
}

/**
 * Read the time tag and convert it to a Java Date object. A timestamp is a 64 bit number
 * representing the time in NTP format. The first 32 bits are seconds since 1900, the
 * second 32 bits are fractions of a second.
 * @return a Date
 */
QDateTime OSCByteArrayToMsgConverter::readTimeTag()
{
    qint64 secondBytes;
    qint64 fractionBytes;
    qint32 tdata;

    bool isImmediate = true;
    tdata = iBytes->at(iStreamPosition++);
    tdata = (tdata<<8)| iBytes->at(iStreamPosition++);
    tdata = (tdata<<8)| iBytes->at(iStreamPosition++);
    tdata = (tdata<<8)| iBytes->at(iStreamPosition++);
    if(tdata > 0) isImmediate = false;
    secondBytes = (unsigned)tdata;

    tdata = iBytes->at(iStreamPosition++);
    tdata = (tdata<<8)| iBytes->at(iStreamPosition++);
    tdata = (tdata<<8)| iBytes->at(iStreamPosition++);
    tdata = (tdata<<8)| iBytes->at(iStreamPosition++);
    if(tdata > 0) isImmediate = false;
    fractionBytes = (unsigned)tdata;

    if(isImmediate) return TIMESTAMP_IMMEDIATE;

    qint64 secsSince1900 = secondBytes;
    long secsSince1970 =  secsSince1900 - OSCBundle::SECONDS_FROM_1900_to_1970;
    if(secsSince1970 < 0) secsSince1970 = 0;

    //long fraction = (fractionBytes*1000)>>32;
    QDateTime mTimeTag;
    mTimeTag.setTime_t(secsSince1970);
    return mTimeTag;
}

/**
 * Read an array from the byte stream.
 * @param types
 * @param i
 * @return an Array
 */
QList<QVariant> OSCByteArrayToMsgConverter::readArray(QByteArray& types, int i)
{
    int arrayLen = 0;
    while(types.at(i+arrayLen)!= ']') arrayLen++;

    QList<QVariant> array;
    for(int j = 0; j < arrayLen; j++)
    {
        QVariant targm = readArgument(types.at(i+j));
        if(!targm.isNull())
        {
            array.append(targm);
        }
    }
    return array;
}

/**
 * Get the length of the string currently in the byte stream.
 */
qint32 OSCByteArrayToMsgConverter::lengthOfCurrentString()
{
    int i = 0;
    while(iBytes->at(iStreamPosition+i)!= 0) i++;
    return i;
}

/**
 * Move to the next byte with an index in the byte array divisable by four.
 */
void OSCByteArrayToMsgConverter::moveToFourByteBoundry()
{
    // If i'm already at a 4 byte boundry, I need to move to the next one
    int mod = iStreamPosition % 4;
    iStreamPosition += (4 - mod);
}
