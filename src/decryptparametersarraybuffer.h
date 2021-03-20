#ifndef DECRYPTPARAMETERSARRAYBUFFER_H
#define DECRYPTPARAMETERSARRAYBUFFER_H

#include <QByteArray>

class DecryptParametersArrayBuffer
{
public:
    DecryptParametersArrayBuffer();
    QByteArray encKey;
    QByteArray data;
    QByteArray iv;
    QByteArray macKey;
    QByteArray mac;
    QByteArray macData;
};

#endif // DECRYPTPARAMETERSARRAYBUFFER_H
