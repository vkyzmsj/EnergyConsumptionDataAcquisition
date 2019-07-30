#-------------------------------------------------
#
# Project created by QtCreator 2019-06-29T02:56:14
#
#-------------------------------------------------

QT       += core gui network sql  webchannel webenginewidgets axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EnergyConsumptionDataAcquisition
TEMPLATE = app
DESTDIR  = _bin

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        src/base/config.cpp \
        src/base/data_base_manager.cpp \
        src/device/device.cpp \
        src/general/helper.cpp \
        src/gui/frame_tools.cpp \
        src/gui/frame_water_meter_device_config_edit.cpp \
        src/gui/frame_water_meter_display.cpp \
        src/gui/frame_water_meter_display_item.cpp \
        src/gui/report/excel_to_html.cpp \
        src/gui/report/frame_report_view.cpp \
        src/gui/report/js_context.cpp \
        src/gui/water_meter_query_thread.cpp \
        src/logic/device_meter_query.cpp \
        src/logic/report_generator.cpp

HEADERS += \
        mainwindow.h \
        src/base/config.h \
        src/base/data_base_manager.h \
        src/device/device.h \
        src/general/helper.h \
        src/gui/frame_tools.h \
        src/gui/frame_water_meter_device_config_edit.h \
        src/gui/frame_water_meter_display.h \
        src/gui/frame_water_meter_display_item.h \
        src/gui/report/excel_to_html.h \
        src/gui/report/frame_report_view.h \
        src/gui/report/js_context.h \
        src/gui/water_meter_query_thread.h \
        src/logic/device_meter_query.h \
        src/logic/report_generator.h

FORMS += \
        mainwindow.ui \
        src/gui/frame_tools.ui \
        src/gui/frame_water_meter_device_config_edit.ui \
        src/gui/frame_water_meter_display.ui \
        src/gui/frame_water_meter_display_item.ui \
        src/gui/report/frame_report_view.ui

INCLUDEPATH += $$PWD/3rd_lib/nlohmann_json


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    src/db/table_of_db_design \
    src/gui/report/msgutils.js \
    src/gui/report/report.html

include($$PWD/3rd_lib/QtXlsxWriter/src/xlsx/qtxlsx.pri)

TRANSLATIONS = ecd_zh_CN.ts

RESOURCES += \
    source.qrc
