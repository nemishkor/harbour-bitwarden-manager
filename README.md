## Download openssl libs before build

Requirements: wget rpm2cpio cpio

```
cd ..
mkdir -p libs/openssl-1.1.1-aarch64 libs/openssl-1.1.1-armv7hl libs/openssl-1.1.1-i486
cd libs
wget https://openrepos.net/sites/default/files/packages/8272/openssl-devel-1.1.1lgit1-1.10.4.jolla_.aarch64.rpm
wget https://openrepos.net/sites/default/files/packages/8272/openssl-libs-1.1.1lgit1-1.10.4.jolla_.aarch64.rpm
wget https://openrepos.net/sites/default/files/packages/8272/openssl-devel-1.1.1lgit1-1.10.4.jolla_.armv7hl.rpm
wget https://openrepos.net/sites/default/files/packages/8272/openssl-libs-1.1.1lgit1-1.10.4.jolla_.armv7hl.rpm
wget https://openrepos.net/sites/default/files/packages/8272/openssl-devel-1.1.1lgit1-1.10.4.jolla_.i486.rpm
wget https://openrepos.net/sites/default/files/packages/8272/openssl-libs-1.1.1lgit1-1.10.4.jolla_.i486.rpm
rpm2cpio openssl-devel-1.1.1lgit1-1.10.4.jolla_.aarch64.rpm | cpio -idmD openssl-1.1.1-aarch64
rpm2cpio openssl-libs-1.1.1lgit1-1.10.4.jolla_.aarch64.rpm | cpio -idmD openssl-1.1.1-aarch64
rpm2cpio openssl-devel-1.1.1lgit1-1.10.4.jolla_.armv7hl.rpm | cpio -idmD openssl-1.1.1-armv7hl
rpm2cpio openssl-libs-1.1.1lgit1-1.10.4.jolla_.armv7hl.rpm | cpio -idmD openssl-1.1.1-armv7hl
rpm2cpio openssl-devel-1.1.1lgit1-1.10.4.jolla_.i486.rpm | cpio -idmD openssl-1.1.1-i486
rpm2cpio openssl-libs-1.1.1lgit1-1.10.4.jolla_.i486.rpm | cpio -idmD openssl-1.1.1-i486
rm openssl-*-1.1.1lgit1-1.10.4.jolla_.*.rpm
```
