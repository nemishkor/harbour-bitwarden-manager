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
    src/apijsondumper.cpp \
    src/app.cpp \
    src/auth.cpp \
    src/cipherstring.cpp \
    src/enums.cpp \
    src/factories/cipherfactory.cpp \
    src/factories/folderfactory.cpp \
    src/models/authentication.cpp \
    src/models/cipher.cpp \
    src/models/ciphercard.cpp \
    src/models/cipherfield.cpp \
    src/models/cipherfilters.cpp \
    src/models/cipheridentity.cpp \
    src/models/cipherlogin.cpp \
    src/models/cipherpasswordhistoryitem.cpp \
    src/models/folder.cpp \
    src/models/folderlistitem.cpp \
    src/models/folderslistmodel.cpp \
    src/models/responses/identitycaptcharesponse.cpp \
    src/models/tasklistitem.cpp \
    src/models/taskslistmodel.cpp \
    src/services/appidservice.cpp \
    src/services/cipherservice.cpp \
    src/services/cryptoservice.cpp \
    src/services/entitiesservice.cpp \
    src/services/environmentservice.cpp \
    src/services/folderservice.cpp \
    src/services/stateservice.cpp \
    src/services/syncservice.cpp \
    src/services/tokenservice.cpp \
    src/symmetriccryptokey.cpp \
    src/tasks/apiauthtask.cpp \
    src/tasks/apitask.cpp \
    src/tasks/removeciphertask.cpp \
    src/tasks/synctask.cpp \
    src/user.cpp \
    src/vaultmanager.cpp \
    src/viewmodels/cipherfieldlistitem.cpp \
    src/viewmodels/cipherfieldslistmodel.cpp \
    src/viewmodels/cipherlistitem.cpp \
    src/viewmodels/cipherpasswordhistorylistitem.cpp \
    src/viewmodels/cipherpasswordhistorylistmodel.cpp \
    src/viewmodels/cipherslistmodel.cpp \
    src/viewmodels/cipherview.cpp \
    src/viewmodels/vault.cpp

DISTFILES += qml/harbour-bitwarden-manager.qml \
    README.md \
    qml/components/BackgroundItemButton.qml \
    qml/components/CipherLabel.qml \
    qml/components/LockBlock.qml \
    qml/components/SyncBlock.qml \
    qml/components/Tasks.qml \
    qml/cover/CoverPage.qml \
    qml/pages/Cipher.qml \
    qml/pages/CipherFields.qml \
    qml/pages/CipherPasswordHistory.qml \
    qml/pages/Ciphers.qml \
    qml/pages/Folders.qml \
    qml/pages/Login.qml \
    qml/pages/Home.qml \
    qml/pages/Profile.qml \
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
    src/apijsondumper.h \
    src/app.h \
    src/auth.h \
    src/cipherstring.h \
    src/devicetype.h \
    src/enums.h \
    src/factories/cipherfactory.h \
    src/factories/folderfactory.h \
    src/kdftype.h \
    src/models/authentication.h \
    src/models/cipher.h \
    src/models/ciphercard.h \
    src/models/cipherfield.h \
    src/models/cipherfilters.h \
    src/models/cipheridentity.h \
    src/models/cipherlogin.h \
    src/models/cipherpasswordhistoryitem.h \
    src/models/folder.h \
    src/models/folderlistitem.h \
    src/models/folderslistmodel.h \
    src/models/responses/identitycaptcharesponse.h \
    src/models/tasklistitem.h \
    src/models/taskslistmodel.h \
    src/services/appidservice.h \
    src/services/cipherservice.h \
    src/services/cryptoservice.h \
    src/services/entitiesservice.h \
    src/services/environmentservice.h \
    src/services/folderservice.h \
    src/services/stateservice.h \
    src/services/syncservice.h \
    src/services/tokenservice.h \
    src/symmetriccryptokey.h \
    src/tasks/apiauthtask.h \
    src/tasks/apitask.h \
    src/tasks/removeciphertask.h \
    src/tasks/synctask.h \
    src/user.h \
    src/vaultmanager.h \
    src/viewmodels/cipherfieldlistitem.h \
    src/viewmodels/cipherfieldslistmodel.h \
    src/viewmodels/cipherlistitem.h \
    src/viewmodels/cipherpasswordhistorylistitem.h \
    src/viewmodels/cipherpasswordhistorylistmodel.h \
    src/viewmodels/cipherslistmodel.h \
    src/viewmodels/cipherview.h \
    src/viewmodels/vault.h

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

