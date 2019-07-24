#pragma once

#include <QString>
#include <QByteArray>
#include <QDateTime>
#include "../../3rd_lib/nlohmann_json/json.hpp"

enum class DeviceStatus
{
    NoneActivite = 0x00,
    Activite = 0x01,
    Deleted = 0x02
};
NLOHMANN_JSON_SERIALIZE_ENUM(DeviceStatus,{
                                 {DeviceStatus::NoneActivite, "NoneActivite"},
                                 {DeviceStatus::Activite, "Activite"},
                             });

struct WaterDeviceInfo
{
    QString device_name, server_ip;
    QByteArray device_address;      // Hex 6B
    QByteArray manu_code;           // Hex 2B
    char meter_type;                // Hex 1B
    quint16 server_port;
    DeviceStatus device_status;
    WaterDeviceInfo(): device_name(""), server_ip(""),meter_type(0x00), server_port(0){

    }
    WaterDeviceInfo(const QString &device_name): device_name(device_name), server_ip(""),meter_type(0x00), server_port(0){

    }
    WaterDeviceInfo(const QString &device_name, const QString &ip, quint16 server_port): device_name(device_name), server_ip(ip),meter_type(0x00), server_port(server_port){

    }

    static bool DeviceAddressCheckPass(const QByteArray &device_address){
        return device_address.size() == 6;
    }
    static bool ManuCodeCheckPass(const QByteArray &manu_code){
        return manu_code.size() == 2;
    }
};
//void to_json(json &j, const MeterDeviceConfig &info);
//void from_json(const json &j, MeterDeviceConfig &info);

struct WaterDeviceRecord
{
    QDateTime date_time;
    double measure_value;
};
//void to_json(json &j, const MeterDeviceConfig &info);
//void from_json(const json &j, MeterDeviceConfig &info);
