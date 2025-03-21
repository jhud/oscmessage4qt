/****************************************************************************
**
**
****************************************************************************/

#include "OSCMsgToByteArrayConverter.h"

template<typename T>
static inline T BigEndian(const T& x)
{
    const int one = 1;
    const char sig = *(char*)&one;
    if (sig == 0) return x; // for big endian machine just return the input
    T ret;
    int size = sizeof(T);
    char* src = (char*)&x + sizeof(T) - 1;
    char* dst = (char*)&ret;
    while (size-- > 0){
        *dst++ = *src--;
    }
    return ret;
}

OSCMsgToByteArrayConverter::OSCMsgToByteArrayConverter(QByteArray& byteArray)
    : iByteArray(byteArray)
{
}

OSCMsgToByteArrayConverter::~OSCMsgToByteArrayConverter()
{
}

/**
 * Pad the stream to have a size divisible by 4.
 */
void OSCMsgToByteArrayConverter::appendNullCharToAlignStream()
{
    int mod = iByteArray.length() % 4;
    int pad = 4 - mod;
    for (int i = 0; i < pad; i++)
    {
        iByteArray.append('\0');
    }
}

/**
 * Write bytes to the stream, catching IOExceptions and converting them to RuntimeExceptions.
 * @param bytes byte[]
 */
void OSCMsgToByteArrayConverter::writeUnderHandler(QByteArray& bytes)
{
    iByteArray.append( alignBigEndToFourByteBoundry(bytes) );
}

/**
 * Write bytes into the byte stream.
 * @param bytes byte[]
 */
void OSCMsgToByteArrayConverter::write(QByteArray& bytes)
{
    writeUnderHandler(bytes);
}

/**
 * Write a 32 bit integer to the byte array without allocating memory.
 * @param value a 32 bit int.
 */
void OSCMsgToByteArrayConverter::writeInteger32ToByteArray(qint32 value)
{
    longintBytes[3] = (qint8)value; value>>=8;
    longintBytes[2] = (qint8)value; value>>=8;
    longintBytes[1] = (qint8)value; value>>=8;
    longintBytes[0] = (qint8)value;
    iByteArray.append((char*)longintBytes, 4);
}

/**
 * Write an int into the byte stream.
 * @param i int
 */
void OSCMsgToByteArrayConverter::write(qint32 ni)
{
    writeInteger32ToByteArray(ni);
}

void OSCMsgToByteArrayConverter::write(QChar achar)
{
    writeInteger32ToByteArray(achar.unicode());
}

void OSCMsgToByteArrayConverter::write(double& dba)
{
    qint64 dat1 = ((qint64)dba)>>29;
    qint32 dat0 = dat1 & 0x007FFFFF;
    dat0 |= (dat1>>3) & 0xFF800000;

    writeInteger32ToByteArray(dat0);
}

/**
 * Write a float into the byte stream.
 * @param f java.lang.Float
 */
void OSCMsgToByteArrayConverter::write(float& fa)
{
    qint32 * fv = (qint32*)(&fa);
    writeInteger32ToByteArray(*fv);
}

/**
 * @param i java.lang.BigInteger
 */
void OSCMsgToByteArrayConverter::write(qint64& la)
{
    writeInteger64ToByteArray(la);
}

/**
 * Write a 64 bit integer to the byte array without allocating memory.
 * @param value a 64 bit int.
 */
void OSCMsgToByteArrayConverter::writeInteger64ToByteArray(qint64& value)
{
    longintBytes[7] = (qint8)value; value>>=8;
    longintBytes[6] = (qint8)value; value>>=8;
    longintBytes[5] = (qint8)value; value>>=8;
    longintBytes[4] = (qint8)value; value>>=8;
    longintBytes[3] = (qint8)value; value>>=8;
    longintBytes[2] = (qint8)value; value>>=8;
    longintBytes[1] = (qint8)value; value>>=8;
    longintBytes[0] = (qint8)value;
    iByteArray.append((char*)longintBytes, 8);
}

/**
 * Line up the Big end of the bytes to a 4 byte boundry
 * @return byte[]
 * @param bytes byte[]
 */
QByteArray& OSCMsgToByteArrayConverter::alignBigEndToFourByteBoundry(QByteArray& bytes)
{
    int mod = bytes.length() % 4;
    if (mod==0) return bytes;

    int pad = 4 - mod;
    for(int i = 0; i < pad; i++)
    {
        bytes.insert(0,'\0');
    }
    return bytes;
}

/**
 * Write a string into the byte stream.
 * @param aString java.lang.String
 */
void OSCMsgToByteArrayConverter::write(QString& aString)
{
    QByteArray strArray(aString.toUtf8());
    int mod = strArray.length() % 4;
    int pad = 4 - mod;
    for(int i = 0; i < pad; i++)
    {
        strArray.append('\0');
    }
    iByteArray.append(strArray);
}

/**
 * Write a char into the byte stream.
 * @param c char
 */
void OSCMsgToByteArrayConverter::write(char ac)
{
    iByteArray.append(ac);
}

/**
 * Write an object into the byte stream.
 * @param anObject one of Float, String, Integer, BigInteger, or array of these.
 */
void OSCMsgToByteArrayConverter::write(QVariant& anObject)
{
    if(anObject.isNull()) return;

    QVariant::Type objType = anObject.type();
    if(objType==QVariant::List)
    {
        QList<QVariant> objList(anObject.toList());
        for(int i = 0; i < objList.length(); i++)
        {
            write((QVariant&)objList.at(i));
        }
        return;
    }
    if(objType==QVariant::Double)
    {
        double tdat(anObject.toDouble());
        write(tdat);
        return;
    }
    if(objType==QVariant::String)
    {
        QString tdat(anObject.toString());
        write(tdat);
        return;
    }
    if(objType==QVariant::Int)
    {
        qint32 tdat(anObject.toInt());
        write(tdat);
        return;
    }
    if(objType==QVariant::Char)
    {
        QChar tdat(anObject.toChar());
        write(tdat);
        return;
    }
    if(objType==QVariant::LongLong)
    {
        qint64 tdat(anObject.toLongLong());
        write(tdat);
        return;
    }
    if(objType==QVariant::ByteArray)
    {
        QByteArray tdat(anObject.toByteArray());
        write(qint32(tdat.length()));
        writeBlob(tdat);
        return;
    }
    if((QMetaType::Type)objType==QMetaType::Float)
    {
        float tdat(anObject.toFloat());
        write(tdat);
        return;
    }
}


/**
 * Write a string into the byte stream.
 * @param aString java.lang.String
 */
void OSCMsgToByteArrayConverter::writeBlob(QByteArray& strArray)
{
    int mod = strArray.length() % 4;
    int pad = 4 - mod;
    for(int i = 0; i < pad; i++)
    {
        strArray.append('\0');
    }
    iByteArray.append(strArray);
}

/**
 * Write the type tag for the type represented by the class
 * @param c Class of a Java object in the arguments
 */
void OSCMsgToByteArrayConverter::writeType(QVariant& anObject)
{
    if(anObject.isNull()) return;

    QVariant::Type objType = anObject.type();
    if(objType==QVariant::List)
    {
        QList<QVariant> objList(anObject.toList());
        iByteArray.append('[');
        for(int i = 0; i < objList.length(); i++)
        {
            writeType((QVariant&)objList.at(i));
        }
        iByteArray.append(']');
        return;
    }
    if(objType==QVariant::Int)
    {
        iByteArray.append('i');
        return;
    }
    if(objType==QVariant::LongLong)
    {
        iByteArray.append('h');
        return;
    }
    if((QMetaType::Type)objType==QMetaType::Float)
    {
        iByteArray.append('f');
        return;
    }
    if(objType==QVariant::Double)
    {
        iByteArray.append('d');
        return;
    }
    if(objType==QVariant::String)
    {
        iByteArray.append('s');
        return;
    }
    if(objType==QVariant::Char)
    {
        iByteArray.append('c');
        return;
    }
    if(objType==QVariant::ByteArray) {
        iByteArray.append('b');
        return;
    }
}

/**
 * Write the types for an array element in the arguments.
 * @param array java.lang.Object[]
 */
void OSCMsgToByteArrayConverter::writeTypesArray(QList<QVariant>& array)
{
    for(int i = 0; i < array.length(); i++)
    {
        const QVariant& anobj(array.at(i));
        if(anobj.isNull()) continue;

        if(anobj.type()==QVariant::Bool)
        {
            if(anobj.toBool())
            {
                iByteArray.append('T');
            }
            else
            {
                iByteArray.append('F');
            }
            continue;
        }
        writeType((QVariant&)anobj);
    }
}

/**
 * Write types for the arguments (use a vector for jdk1.1 compatibility, rather than an ArrayList).
 * @param vector  the arguments to an OSCMessage
 */
void OSCMsgToByteArrayConverter::writeTypes(QVector<QVariant>& vector)
{
    QList<QVariant> objList(vector.toList());
    writeTypesArray(objList);
    appendNullCharToAlignStream();
}
