/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include <QDebug>
#include <qaudioformat.h>


QT_BEGIN_NAMESPACE

namespace
{
    class QAudioFormatPrivateRegisterMetaTypes
    {
    public:
        QAudioFormatPrivateRegisterMetaTypes()
        {
            qRegisterMetaType<QAudioFormat>();
            qRegisterMetaType<QAudioFormat::SampleType>();
            qRegisterMetaType<QAudioFormat::Endian>();
        }
    } _registerMetaTypes;
}


class QAudioFormatPrivate : public QSharedData
{
public:
    QAudioFormatPrivate()
    {
        frequency = -1;
        channels = -1;
        sampleSize = -1;
        byteOrder = QAudioFormat::Endian(QSysInfo::ByteOrder);
        sampleType = QAudioFormat::Unknown;
    }

    QAudioFormatPrivate(const QAudioFormatPrivate &other):
        QSharedData(other),
        codec(other.codec),
        byteOrder(other.byteOrder),
        sampleType(other.sampleType),
        frequency(other.frequency),
        channels(other.channels),
        sampleSize(other.sampleSize)
    {
    }

    QAudioFormatPrivate& operator=(const QAudioFormatPrivate &other)
    {
        codec = other.codec;
        byteOrder = other.byteOrder;
        sampleType = other.sampleType;
        frequency = other.frequency;
        channels = other.channels;
        sampleSize = other.sampleSize;

        return *this;
    }

    QString codec;
    QAudioFormat::Endian byteOrder;
    QAudioFormat::SampleType sampleType;
    int frequency;
    int channels;
    int sampleSize;
};

/*!
    \class QAudioFormat
    \brief The QAudioFormat class stores audio stream parameter information.

    \inmodule QtMultimedia
    \ingroup  multimedia
    \since 1.0

    An audio format specifies how data in an audio stream is arranged,
    i.e, how the stream is to be interpreted. The encoding itself is
    specified by the codec() used for the stream.

    In addition to the encoding, QAudioFormat contains other
    parameters that further specify how the audio sample data is arranged.
    These are the frequency, the number of channels, the sample size,
    the sample type, and the byte order. The following table describes
    these in more detail.

    \table
        \header
            \o Parameter
            \o Description
        \row
            \o Sample Rate
            \o Samples per second of audio data in Hertz.
        \row
            \o Number of channels
            \o The number of audio channels (typically one for mono
               or two for stereo)
        \row
            \o Sample size
            \o How much data is stored in each sample (typically 8
               or 16 bits)
        \row
            \o Sample type
            \o Numerical representation of sample (typically signed integer,
               unsigned integer or float)
        \row
            \o Byte order
            \o Byte ordering of sample (typically little endian, big endian)
    \endtable

    This class is typically used in conjunction with QAudioInput or
    QAudioOutput to allow you to specify the parameters of the audio
    stream being read or written.

    You can obtain audio formats compatible with the audio device used
    through functions in QAudioDeviceInfo. This class also lets you
    query available parameter values for a device, so that you can set
    the parameters yourself. See the \l QAudioDeviceInfo class
    description for details. You need to know the format of the audio
    streams you wish to play or record.
*/

/*!
    Construct a new audio format.

    Values are initialized as follows:
    \list
    \o sampleRate()  = -1
    \o channelCount() = -1
    \o sampleSize() = -1
    \o byteOrder()  = QAudioFormat::Endian(QSysInfo::ByteOrder)
    \o sampleType() = QAudioFormat::Unknown
    \c codec()      = ""
    \endlist
*/
QAudioFormat::QAudioFormat():
    d(new QAudioFormatPrivate)
{
}

/*!
    Construct a new audio format using \a other.
    \since 1.0
*/
QAudioFormat::QAudioFormat(const QAudioFormat &other):
    d(other.d)
{
}

/*!
    Destroy this audio format.
*/
QAudioFormat::~QAudioFormat()
{
}

/*!
    Assigns \a other to this QAudioFormat implementation.
    \since 1.0
*/
QAudioFormat& QAudioFormat::operator=(const QAudioFormat &other)
{
    d = other.d;
    return *this;
}

/*!
  Returns true if this QAudioFormat is equal to the \a other
  QAudioFormat; otherwise returns false.

  All elements of QAudioFormat are used for the comparison.
  \since 1.0
*/
bool QAudioFormat::operator==(const QAudioFormat &other) const
{
    return d->frequency == other.d->frequency &&
            d->channels == other.d->channels &&
            d->sampleSize == other.d->sampleSize &&
            d->byteOrder == other.d->byteOrder &&
            d->codec == other.d->codec &&
            d->sampleType == other.d->sampleType;
}

/*!
  Returns true if this QAudioFormat is not equal to the \a other
  QAudioFormat; otherwise returns false.

  All elements of QAudioFormat are used for the comparison.
  \since 1.0
*/
bool QAudioFormat::operator!=(const QAudioFormat& other) const
{
    return !(*this == other);
}

/*!
    Returns true if all of the parameters are valid.
    \since 1.0
*/
bool QAudioFormat::isValid() const
{
    return d->frequency != -1 && d->channels != -1 && d->sampleSize != -1 &&
            d->sampleType != QAudioFormat::Unknown && !d->codec.isEmpty();
}

/*!
   Sets the sample rate to \a samplerate Hertz.

   \since 1.0
*/
void QAudioFormat::setSampleRate(int samplerate)
{
    d->frequency = samplerate;
}

/*!
   \obsolete

   Use setSampleRate() instead.
*/
void QAudioFormat::setFrequency(int frequency)
{
    d->frequency = frequency;
}

/*!
    Returns the current sample rate in Hertz.

    \since 1.0
*/
int QAudioFormat::sampleRate() const
{
    return d->frequency;
}

/*!
   \obsolete

   Use sampleRate() instead.
*/
int QAudioFormat::frequency() const
{
    return d->frequency;
}

/*!
   Sets the channel count to \a channels.

   \since 1.0
*/
void QAudioFormat::setChannelCount(int channels)
{
    d->channels = channels;
}

/*!
   \obsolete

   Use setChannelCount() instead.
*/
void QAudioFormat::setChannels(int channels)
{
    d->channels = channels;
}

/*!
    Returns the current channel count value.

    \since 1.0
*/
int QAudioFormat::channelCount() const
{
    return d->channels;
}

/*!
    \obsolete

    Use channelCount() instead.
*/
int QAudioFormat::channels() const
{
    return d->channels;
}

/*!
   Sets the sample size to the \a sampleSize specified, in bits.

   This is typically 8 or 16, but some systems may support higher sample sizes.
   \since 1.0
*/
void QAudioFormat::setSampleSize(int sampleSize)
{
    d->sampleSize = sampleSize;
}

/*!
    Returns the current sample size value, in bits.
    \since 1.0
*/
int QAudioFormat::sampleSize() const
{
    return d->sampleSize;
}

/*!
   Sets the codec to \a codec.

   The parameter to this function should be one of the types
   reported by the QAudioDeviceInfo::supportedCodecs() function
   for the audio device you are working with.

   \since 1.0
   \sa QAudioDeviceInfo::supportedCodecs()
*/
void QAudioFormat::setCodec(const QString &codec)
{
    d->codec = codec;
}

/*!
    Returns the current codec identifier.

   \since 1.0
   \sa QAudioDeviceInfo::supportedCodecs()
*/
QString QAudioFormat::codec() const
{
    return d->codec;
}

/*!
   Sets the byteOrder to \a byteOrder.
   \since 1.0
*/
void QAudioFormat::setByteOrder(QAudioFormat::Endian byteOrder)
{
    d->byteOrder = byteOrder;
}

/*!
    Returns the current byteOrder value.
    \since 1.0
*/
QAudioFormat::Endian QAudioFormat::byteOrder() const
{
    return d->byteOrder;
}

/*!
   Sets the sampleType to \a sampleType.
   \since 1.0
*/
void QAudioFormat::setSampleType(QAudioFormat::SampleType sampleType)
{
    d->sampleType = sampleType;
}

/*!
    Returns the current SampleType value.
    \since 1.0
*/
QAudioFormat::SampleType QAudioFormat::sampleType() const
{
    return d->sampleType;
}

/*!
    \enum QAudioFormat::SampleType

    \value Unknown       Not Set
    \value SignedInt     Samples are signed integers
    \value UnSignedInt   Samples are unsigned intergers
    \value Float         Samples are floats
*/

/*!
    \enum QAudioFormat::Endian

    \value BigEndian     Samples are big endian byte order
    \value LittleEndian  Samples are little endian byte order
*/

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, QAudioFormat::Endian endian)
{
    QDebug nospace = dbg.nospace();
    switch (endian) {
        case QAudioFormat::BigEndian:
            nospace << "BigEndian";
            break;
        case QAudioFormat::LittleEndian:
            nospace << "LittleEndian";
            break;
    }
    return nospace;
}

QDebug operator<<(QDebug dbg, QAudioFormat::SampleType type)
{
    QDebug nospace = dbg.nospace();
    switch (type) {
        case QAudioFormat::SignedInt:
            nospace << "SignedInt";
            break;
        case QAudioFormat::UnSignedInt:
            nospace << "UnSignedInt";
            break;
        case QAudioFormat::Float:
            nospace << "Float";
            break;
       default:
            nospace << "Unknown";
            break;
    }
    return nospace;
}

QDebug operator<<(QDebug dbg, const QAudioFormat &f)
{
    dbg.nospace() << "QAudioFormat(" << f.sampleRate();
    dbg.nospace() << "Hz, " << f.sampleSize();
    dbg.nospace() << "bit, channelCount=" << f.channelCount();
    dbg.nospace() << ", sampleType=" << f.sampleType();
    dbg.nospace() << ", byteOrder=" << f.byteOrder();
    dbg.nospace() << ", codec=" << f.codec();
    dbg.nospace() << ")";

    return dbg.space();
}
#endif



QT_END_NAMESPACE

