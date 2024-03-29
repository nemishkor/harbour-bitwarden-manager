#ifndef ENUMS_H
#define ENUMS_H


class Enums
{
public:
    Enums();
    enum EncryptionType {
        AesCbc256_B64 = 0,
        AesCbc128_HmacSha256_B64 = 1,
        AesCbc256_HmacSha256_B64 = 2,
        Rsa2048_OaepSha256_B64 = 3,
        Rsa2048_OaepSha1_B64 = 4,
        Rsa2048_OaepSha256_HmacSha256_B64 = 5,
        Rsa2048_OaepSha1_HmacSha256_B64 = 6,
    };
    enum TaskStatus {
        Fail = -1,
        Waiting = 0,
        InProgress = 1,
        Success = 2,
    };
};

#endif // ENUMS_H
