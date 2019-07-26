#pragma once
#include "frame_water_meter_display_item.h"
#include "../base/data_base_manager.h"
#include <QFrame>
#include <QMap>

namespace Ui {
class Frame_water_meter_display;
}

class Frame_WaterMeterDisplay : public QFrame
{
    Q_OBJECT

public:
    explicit Frame_WaterMeterDisplay(QWidget *parent = nullptr);
    ~Frame_WaterMeterDisplay();

    void SyncView();

private:
    Ui::Frame_water_meter_display *ui;
    Config *m_config;
    DataBaseManager *m_db_manager;
    bool m_start_monitor;
    QMap<QString, Frame_WaterMeterDisplayItem *> m_view_map;
    QMap<>
};

