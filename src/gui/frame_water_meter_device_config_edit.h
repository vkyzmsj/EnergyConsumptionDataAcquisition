#ifndef FRAME_WATER_METER_DEVICE_CONFIG_EDIT_H
#define FRAME_WATER_METER_DEVICE_CONFIG_EDIT_H

#include <QFrame>
#include "../base/config.h"

namespace Ui {
class Frame_water_meter_device_config_edit;
}

class Frame_WaterMeterDeviceConfigEdit : public QFrame
{
    Q_OBJECT

public:
    explicit Frame_WaterMeterDeviceConfigEdit(QWidget *parent = nullptr);
    ~Frame_WaterMeterDeviceConfigEdit();


    WaterMeterDeviceConfig GetWaterMeterDeviceConfig() const;
    void SetWaterMeterDeviceConfig(const WaterMeterDeviceConfig &water_meter_device_config);

    bool ExitWithYes();

private:
    void Init();
    void UpdateDeviceConfig();
signals:
    void Finished();

private slots:
    void on_pushButton_yes_clicked();

    void on_pushButton_cancle_clicked();

    void on_toolButton_clicked();

private:
    Ui::Frame_water_meter_device_config_edit *ui;
    WaterMeterDeviceConfig m_water_meter_device_config;
    bool m_exit_with_yes;
};

#endif // FRAME_WATER_METER_DEVICE_CONFIG_EDIT_H
