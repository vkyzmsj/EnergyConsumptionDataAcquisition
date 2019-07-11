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
    bool IsEnable() const;
    bool IsRunning() const;
    void StartReq();
    void StopReq();
    QString GetDeviceName() const;

    WaterMeterDeviceConfig GetWaterMeterDeviceConfig() const;
    void SetWaterMeterDeviceConfig(const WaterMeterDeviceConfig &water_meter_device_config);

private:
    void Init();

signals:
    void MeterValReady(QDateTime date_time, double val);

private slots:
    void on_pushButton_start_req_clicked();
    void UpdateMeterVal(QDateTime date_time, double val);

private:
    Ui::Frame_water_meter_display_item *ui;
    QTimer m_meter_update_max_timer;
    int m_meter_not_update_continue_cnt;

    WaterMeterDeviceConfig m_water_meter_device_config;
    WaterMeterQueryThread *m_query_thread;
};

#endif // FRAME_WATER_METER_DISPLAY_ITEM_H
