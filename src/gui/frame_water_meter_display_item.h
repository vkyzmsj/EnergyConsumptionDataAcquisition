#ifndef FRAME_WATER_METER_DISPLAY_ITEM_H
#define FRAME_WATER_METER_DISPLAY_ITEM_H

#include <QFrame>
#include <QTimer>
#include <QUdpSocket>

#include "../base/config.h"
#include "water_meter_query_thread.h"

namespace Ui {
class Frame_water_meter_display_item;
}

class Frame_WaterMeterDisplayItem : public QFrame
{
    Q_OBJECT

public:
    explicit Frame_WaterMeterDisplayItem(QWidget *parent = nullptr);
    ~Frame_WaterMeterDisplayItem();
    QString GetDeviceName() const;

    WaterDeviceInfo GetWaterMeterDeviceConfig() const;
    void SetWaterMeterDeviceConfig(const WaterDeviceInfo &water_meter_device_config);

    void StartUpdate();
    void StopUpdate();
    void SetMaxUpdateCheckTimeInval(int s);
private:
    void Init();
    void ResetBackGroundColorValue();
    void IncBackGroundColorBadLevel();
    void UpdateBackGroundColor();

signals:
    void MeterValReady(QDateTime date_time, double val);

public slots:
    void UpdateMeterVal(const QString &dev_name, const QDateTime &date_time, double val);

private:
    Ui::Frame_water_meter_display_item *ui;
    QTimer m_meter_update_max_timer;
    WaterDeviceInfo m_water_meter_device_config;
    int m_max_update_time_s;
    QColor m_back_ground_color;
};

#endif // FRAME_WATER_METER_DISPLAY_ITEM_H
