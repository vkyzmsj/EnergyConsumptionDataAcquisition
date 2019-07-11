#include "config.h"
#include "../general/helper.h"
#include <QFile>
#include <QMessageBox>

Config *Config::Instance()
{
    static Config c;
    return &c;
}

bool Config::WaterDeviceExists(const QString &name) const
{
    return  m_water_meter_device_config_map.contains(name);
}

void Config::AddWaterDeviceConfig(const WaterMeterDeviceConfig &config)
{
    m_water_meter_device_config_map.insert(config.GetDevName(), config);
}

QList<WaterMeterDeviceConfig> Config::GetWaterDeviceConfigList() const
{
    return m_water_meter_device_config_map.values();
}

void Config::Save()
{
    json j;
    std::map<std::string, WaterMeterDeviceConfig> water_std_map;
    for(auto it = m_water_meter_device_config_map.begin(); it != m_water_meter_device_config_map.end(); ++it)
    {
        water_std_map.insert(std::pair<std::string, WaterMeterDeviceConfig>(it.key().toStdString(), *it));
    }
    j["water device config"] = water_std_map;


    QFile file(m_config_file_path);
    file.open(QIODevice::WriteOnly | QIODevice::Unbuffered);
    if(!Helper::WriteFile(file, j.dump(4)))
    {
        QMessageBox::warning(nullptr, QObject::tr("save config file"), QObject::tr("save config file failed."),
                             QMessageBox::Yes);
    }
}

Config::Config():
    m_config_file_path("config.json"),
    m_db_file_path("ecda.db")
{

}

void Config::Load()
{
    QFile file(m_config_file_path);

    QString context;
    Helper::ReadFile(file, context, file.size());
    file.close();

    if(context.size() == 0)
    {
        QMessageBox::warning(nullptr, QObject::tr("load config file"), QObject::tr("read config file failed or file context is empty"),
                             QMessageBox::Yes);
        abort();
    }
    try {
        json j = json::parse(context.toStdString().c_str());
        QMap<std::string, WaterMeterDeviceConfig> water_std_map =
                QMap<std::string, WaterMeterDeviceConfig>(j.at("water device config").get<std::map<std::string, WaterMeterDeviceConfig>>());
        for(auto it = water_std_map.begin(); it != water_std_map.end(); ++it)
        {
            m_water_meter_device_config_map.insert(QString::fromStdString(it.key()), *it);
        }


    } catch (json::exception e) {
        int ret = QMessageBox::warning(nullptr, QObject::tr("load config file"),
                             QObject::tr("config file broken\n"
                                         "detail error message:\n"
                                         "\t%1\n"
                                         "Click OK To Reset ConfigFile, You Need Restart This Program").arg(e.what()),
                             QMessageBox::Ok | QMessageBox::Cancel);
        if(ret == QMessageBox::Ok)
        {
            file.remove();
        }
        abort();
    }
}

QString Config::GetDbFilePath() const
{
    return m_db_file_path;
}
