QT += widgets network

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# para poder plotear rapido
LIBS += -L"C:\QcMuparser" -l:MuParserCreationDLL_2024.dll
INCLUDEPATH += "C:\QcMuparser"
LIBS += -L"C:\Qc_io" -l:qcustomplotd2.dll
INCLUDEPATH += "C:\Qc_io"
