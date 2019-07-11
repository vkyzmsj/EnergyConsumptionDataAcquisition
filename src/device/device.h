#ifndef DEVICE_H
#define DEVICE_H
#include <QString>

#include "../../3rd_lib/nlohmann_json/json.hpp"

using nlohmann::json;
enum class MeterDeviceType
{
    WaterMeter = 0x01
};
NLOHMANN_JSON_SERIALIZE_ENUM(MeterDeviceType,{
                                 {MeterDeviceType::WaterMeter, "water meter device"},
                             });

class MeterDeviceConfig
{
public:
    MeterDeviceConfig();


    QString GetDevName() const;
    void SetDevName(const QString &dev_name);

    MeterDeviceType GetDevType() const;
    void SetDevType(const MeterDeviceType &dev_type);

    QString GetServerIpAddr() const;
    void SetServerIpAddr(const QString &server_ip_addr);

    quint32 GetDegreeQueryCycleS() const;
    void SetDegreeQueryCycleS(const quint32 &degree_query_cycle_s);

    quint32 GetDegreeDataSaveMaxTimeS() const;
    void SetDegreeDataSaveMaxTimeS(const quint32 &degree_data_save_max_time_s);

    QString GetSaveDir() const;
    void SetSaveDir(const QString &save_dir);

    friend void to_json(json &j, const MeterDeviceConfig &info);
    friend void from_json(const json &j, MeterDeviceConfig &info);

    quint16 GetServerPort() const;
    void SetServerPort(const quint16 &server_port);

    bool IsEnable() const;
    void setEnable(bool enable);

private:
    QString m_dev_name; // 名称必须唯一
    MeterDeviceType m_dev_type;
    QString m_server_ip_addr;
    quint16 m_server_port;
    quint32 m_degree_query_cycle_s; // 读数查询周期
    quint32 m_degree_data_save_max_time_s; // 文件保存周期
    QString m_save_dir;
    bool m_enable;
};

void to_json(json &j, const MeterDeviceConfig &info);
void from_json(const json &j, MeterDeviceConfig &info);

class WaterMeterDeviceConfig : public MeterDeviceConfig
{
public:
    WaterMeterDeviceConfig();

    QString GetDevAddrCode() const;
    void SetDevAddrCode(const QString &dev_addr_code);

    QString GetManufacturerCode() const;
    void SetManufacturerCode(const QString &manufacturer_code);

    friend void to_json(json &j, const WaterMeterDeviceConfig &info);
    friend void from_json(const json &j, WaterMeterDeviceConfig &info);

    QString GetMeterTypeCode() const;
    void SetMeterTypeCode(const QString &meter_type_code);

private:
    QString m_dev_addr_code; // 5字节地址
    QString m_manufacturer_code; // 2字节
    QString m_meter_type_code; // 1字节

};
void to_json(json &j, const WaterMeterDeviceConfig &info);
void from_json(const json &j, WaterMeterDeviceConfig &info);

#endif // DEVICE_H
