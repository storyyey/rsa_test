QT       += core gui serialport network sql xlsx

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
RC_ICONS = favicon.ico
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    atcmd.cpp \
    commonsensor.cpp \
    database.cpp \
    debugwidget.cpp \
    electricalfiresensor.cpp \
    eventrecord.cpp \
    helpwidget.cpp \
    humituresensor.cpp \
    main.cpp \
    mainwindow.cpp \
    onlyapp.cpp \
    operatexlsx.cpp \
    operationpanel.cpp \
    outdoorwlhs.cpp \
    promptdialog.cpp \
    residualelectricitysensor.cpp \
    saveasdialog.cpp \
    sensorbaseconfig.cpp \
    sensorconfigwidget.cpp \
    sensordata.cpp \
    steptips.cpp \
    switchvalsensor.cpp \
    tableswidget.cpp \
    temperaturesensor.cpp \
    threephaseelectricity.cpp \
    threephaseswitchsensor.cpp \
    threephasevoltagesensor.cpp \
    uartwidget.cpp \
    waterlevelsensor.cpp \
    waterpresssensor.cpp \
    waterpumpsensor.cpp

HEADERS += \
    atcmd.h \
    commonsensor.h \
    database.h \
    debugwidget.h \
    electricalfiresensor.h \
    eventrecord.h \
    helpwidget.h \
    humituresensor.h \
    mainwindow.h \
    onlyapp.h \
    operatexlsx.h \
    operationpanel.h \
    outdoorwlhs.h \
    promptdialog.h \
    residualelectricitysensor.h \
    saveasdialog.h \
    sensorbaseconfig.h \
    sensorconfigwidget.h \
    sensordata.h \
    sensoroperation.h \
    settingOption.h \
    steptips.h \
    switchvalsensor.h \
    tableswidget.h \
    temperaturesensor.h \
    threephaseelectricity.h \
    threephaseswitchsensor.h \
    threephasevoltagesensor.h \
    uartwidget.h \
    waterlevelsensor.h \
    waterpresssensor.h \
    waterpumpsensor.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icon.qrc
