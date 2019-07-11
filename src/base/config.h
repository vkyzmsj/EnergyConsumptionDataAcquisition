#ifndef CONFIG_H
#define CONFIG_H
#include "../device/device.h"
#include <QMap>

class Config
{
public:
    static Config *Instance();
    bool WaterDeviceExists(const QString &name) const;
    void AddWaterDeviceConfig(const WaterMeterDeviceConfig &config);
    QList<WaterMeterDeviceConfig> GetWaterDeviceConfigList() const;
    void Save();

    QString GetDbFilePath() const;

private:
    Config();
    void Load();

private:
    QMap<QString, WaterMeterDeviceConfig> m_water_meter_device_config_map;
    const QString m_config_file_path;
    const QString m_db_file_path;
};

#endif // CONFIG_H
