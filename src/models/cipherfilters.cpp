#include "cipherfilters.h"

CipherFilters::CipherFilters()
{

}

CipherFilters::CipherFilters(bool deletedOnly, QString folderId)
    : deletedOnly(deletedOnly),
      folderId(folderId)
{

}

bool CipherFilters::getDeletedOnly() const
{
    return deletedOnly;
}

const QString &CipherFilters::getFolderId() const
{
    return folderId;
}

bool CipherFilters::filter(Cipher *cipher)
{
    if(deletedOnly && cipher->getDeletedDate().isNull()){
        return false;
    }
    if(!folderId.isNull() && cipher->getFolderId() != folderId){
        return false;
    }
    return true;
}
