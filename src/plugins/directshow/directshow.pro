TARGET = dsengine
QT += multimedia-private

win32:!qtHaveModule(opengl)|qtConfig(dynamicgl) {
    LIBS_PRIVATE += -lgdi32 -luser32
}

HEADERS += dsserviceplugin.h
SOURCES += dsserviceplugin.cpp

# Remove WINVER/_WIN32_WINNT definitions added to qt_build_config.prf
# by qtbase/d57a7c41712f8627a462d893329dc3f0dbb52d32 since the multimedia
# headers of MinGW 5.3/7.1 are too broken to compile with 0x0601.
mingw {
    DEFINES -= WINVER=0x0601 _WIN32_WINNT=0x0601
    DEFINES += NO_DSHOW_STRSAFE
}

# Link against amstrmid for IID_IMFVideoDeviceID, IID_IMFVideoPresenter, IID_IMFTopologyServiceLookupClient
# and IID_IMFTopologyServiceLookupClient; add -Wl,--allow-multiple-definition to workaround conflicts with
# strmiids which contains symbols also required but not provided by amstrmid
mingw {
    LIBS_PRIVATE += -Wl,--allow-multiple-definition -lamstrmid
}

include(common/common.pri)
include(player/player.pri)
include(camera/camera.pri)

OTHER_FILES += \
    directshow.json

PLUGIN_TYPE = mediaservice
PLUGIN_CLASS_NAME = DSServicePlugin
load(qt_plugin)
