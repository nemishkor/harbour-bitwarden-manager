# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = harbour-bitwarden-manager

CONFIG += sailfishapp

SOURCES += src/harbour-bitwarden-manager.cpp \
    src/api.cpp \
    src/appidservice.cpp \
    src/auth.cpp \
    src/authentication.cpp \
    src/cipher.cpp \
    src/ciphercard.cpp \
    src/cipherfield.cpp \
    src/cipherfieldlistitem.cpp \
    src/cipherfieldslistmodel.cpp \
    src/cipheridentity.cpp \
    src/cipherlistitem.cpp \
    src/cipherlogin.cpp \
    src/cipherpasswordhistoryitem.cpp \
    src/cipherpasswordhistorylistitem.cpp \
    src/cipherpasswordhistorylistmodel.cpp \
    src/cipherservice.cpp \
    src/cipherslistmodel.cpp \
    src/cipherstring.cpp \
    src/cipherview.cpp \
    src/cryptoservice.cpp \
    src/decryptparametersarraybuffer.cpp \
    src/folder.cpp \
    src/foldersmodel.cpp \
    src/symmetriccryptokey.cpp \
    src/syncservice.cpp \
    src/tokenservice.cpp \
    src/user.cpp \
    src/vaultmanager.cpp

DISTFILES += qml/harbour-bitwarden-manager.qml \
    README.md \
    qml/components/BackgroundItemButton.qml \
    qml/components/CipherLabel.qml \
    qml/components/LockBlock.qml \
    qml/components/SyncBlock.qml \
    qml/cover/CoverPage.qml \
    qml/pages/Cipher.qml \
    qml/pages/CipherFields.qml \
    qml/pages/CipherPasswordHistory.qml \
    qml/pages/Ciphers.qml \
    qml/pages/Folders.qml \
    qml/pages/Login.qml \
    qml/pages/Home.qml \
    qml/pages/Profile.qml \
    qml/pages/Sync.qml \
    qml/pages/Sync.qml \
    qml/pages/Unlock.qml \
    rpm/harbour-bitwarden-manager.changes \
    rpm/harbour-bitwarden-manager.changes.run.in \
    rpm/harbour-bitwarden-manager.spec \
    rpm/harbour-bitwarden-manager.yaml \
    translations/*.ts \
    harbour-bitwarden-manager.desktop

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n

# German translation is enabled as an example. If you aren't
# planning to localize your app, remember to comment out the
# following TRANSLATIONS line. And also do not forget to
# modify the localized app name in the the .desktop file.
TRANSLATIONS += translations/harbour-bitwarden-manager-de.ts \
    translations/harbour-bitwarden-manager-sv.ts

HEADERS += \
    src/api.h \
    src/appidservice.h \
    src/auth.h \
    src/authentication.h \
    src/cipher.h \
    src/ciphercard.h \
    src/cipherfield.h \
    src/cipherfieldlistitem.h \
    src/cipherfieldslistmodel.h \
    src/cipheridentity.h \
    src/cipherlistitem.h \
    src/cipherlogin.h \
    src/cipherpasswordhistoryitem.h \
    src/cipherpasswordhistorylistitem.h \
    src/cipherpasswordhistorylistmodel.h \
    src/cipherservice.h \
    src/cipherslistmodel.h \
    src/cipherstring.h \
    src/cipherview.h \
    src/cryptoservice.h \
    src/decryptparametersarraybuffer.h \
    src/devicetype.h \
    src/folder.h \
    src/foldersmodel.h \
    src/kdftype.h \
    src/symmetriccryptokey.h \
    src/syncservice.h \
    src/tokenservice.h \
    src/user.h \
    src/vaultmanager.h

QT += network
QT += quick

contains(QT_ARCH, arm64) {
    ARCH_LIBS=lib64
    OPENSSL_ARCH=aarch64
    message("Compiling for 64bit ARM system")
} else {
    contains(QT_ARCH, arm) {
        ARCH_LIBS=lib
        OPENSSL_ARCH=armv7hl
        message("Compiling for 32bit ARM system")
    } else {
        contains(QT_ARCH, i386) {
            ARCH_LIBS=lib
            OPENSSL_ARCH=i486
            message("Compiling for 32bit i386 system")
        } else {
            message("Unknown arch type. Unable to select libraries")
        }
    }
}

INCLUDEPATH += ../libs/openssl-1.1.1-$$OPENSSL_ARCH/usr/include/
LIBS += -L$$PWD/../libs/openssl-1.1.1-$$OPENSSL_ARCH/usr/$$ARCH_LIBS/ -lssl -lcrypto

