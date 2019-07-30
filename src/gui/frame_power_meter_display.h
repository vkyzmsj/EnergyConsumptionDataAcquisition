#ifndef FRAME_POWER_METER_DISPLAY_H
#define FRAME_POWER_METER_DISPLAY_H

#include "frame_water_meter_display_item.h"
#include "../base/data_base_manager.h"
#include <QFrame>
#include <QMap>
#include <QTreeWidgetItem>

namespace Ui {
class Frame_power_meter_display;
}

class Frame_PowerMeterDisplay : public QFrame
{
    Q_OBJECT

public:
    explicit Frame_PowerMeterDisplay(QWidget *parent = nullptr);
    ~Frame_PowerMeterDisplay();
    void SyncView();

private:
    QString GetStyleSheet() const;

private:
    Ui::Frame_power_meter_display *ui;
    Config *m_config;
    DataBaseManager *m_db_manager;
    bool m_start_monitor;
    QMap<QString, Frame_WaterMeterDisplayItem *> m_view_map;
    QMap<QString, QTreeWidgetItem *> m_tree_node_map;
    QTreeWidgetItem *m_tree_top_item;
};

#endif // FRAME_POWER_METER_DISPLAY_H
