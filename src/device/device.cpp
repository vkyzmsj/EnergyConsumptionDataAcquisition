#include "device.h"
//QString m_dev_name; // 名称必须唯一
//MeterDeviceType m_dev_type;
//QString m_server_ip_addr;
//quint16 m_server_port;
//quint32 m_degree_query_cycle_s; // 读数查询周期
//quint32 m_degree_data_save_max_time_s; // 文件保存周期
//QString m_save_dir;
//bool m_enable;
//};
//MeterDeviceConfig::MeterDeviceConfig():
//    m_dev_name("undefined"),
//    m_dev_type(MeterDeviceType::WaterMeter),
//    m_server_ip_addr("192.168.0.7"),
//    m_server_port(11000),
//    m_degree_query_cycle_s(15),
//    m_degree_data_save_max_time_s(10000),
//    m_save_dir("."),
//    m_enable(false)
//{

//}

//QString MeterDeviceConfig::GetDevName() const
//{
//    return m_dev_name;
//}

//void MeterDeviceConfig::SetDevName(const QString &dev_name)
//{
//    m_dev_name = dev_name;
//}

//MeterDeviceType MeterDeviceConfig::GetDevType() const
//{
//    return m_dev_type;
//}

//void MeterDeviceConfig::SetDevType(const MeterDeviceType &dev_type)
//{
//    m_dev_type = dev_type;
//}

//QString MeterDeviceConfig::GetServerIpAddr() const
//{
//    return m_server_ip_addr;
//}

//void MeterDeviceConfig::SetServerIpAddr(const QString &server_ip_addr)
//{
//    m_server_ip_addr = server_ip_addr;
//}

//quint32 MeterDeviceConfig::GetDegreeQueryCycleS() const
//{
//    return m_degree_query_cycle_s;
//}

//void MeterDeviceConfig::SetDegreeQueryCycleS(const quint32 &degree_query_cycle_s)
//{
//    m_degree_query_cycle_s = degree_query_cycle_s;
//}

//quint32 MeterDeviceConfig::GetDegreeDataSaveMaxTimeS() const
//{
//    return m_degree_data_save_max_time_s;
//}

//void MeterDeviceConfig::SetDegreeDataSaveMaxTimeS(const quint32 &degree_data_save_max_time_s)
//{
//    m_degree_data_save_max_time_s = degree_data_save_max_time_s;
//}

//QString MeterDeviceConfig::GetSaveDir() const
//{
//    return m_save_dir;
//}

//void MeterDeviceConfig::SetSaveDir(const QString &save_dir)
//{
//    m_save_dir = save_dir;
//}

//quint16 MeterDeviceConfig::GetServerPort() const
//{
//    return m_server_port;
//}

//void MeterDeviceConfig::SetServerPort(const quint16 &server_port)
//{
//    m_server_port = server_port;
//}

//bool MeterDeviceConfig::IsEnable() const
//{
//    return m_enable;
//}

//void MeterDeviceConfig::setEnable(bool enable)
//{
//    m_enable = enable;
//}

//WaterMeterDeviceConfig::WaterMeterDeviceConfig():
//    m_dev_addr_code("00 00 00 00 00"),
//    m_manufacturer_code("00 00"),
//    m_meter_type_code("00")
//{

//}

//QString WaterMeterDeviceConfig::GetDevAddrCode() const
//{
//    return m_dev_addr_code;
//}

//void WaterMeterDeviceConfig::SetDevAddrCode(const QString &dev_addr_code)
//{
//    Q_ASSERT_X(dev_addr_code.size() != 5, __FUNCTION__, "invaild addr format");
//    m_dev_addr_code = dev_addr_code;
//}

//QString WaterMeterDeviceConfig::GetManufacturerCode() const
//{
//    return m_manufacturer_code;
//}

//void WaterMeterDeviceConfig::SetManufacturerCode(const QString &manufacturer_code)
//{
//    Q_ASSERT_X(manufacturer_code.size() != 2, __FUNCTION__, "invaild manufacturer code format");
//    m_manufacturer_code = manufacturer_code;
//}

//QString WaterMeterDeviceConfig::GetMeterTypeCode() const
//{
//    return m_meter_type_code;
//}

//void WaterMeterDeviceConfig::SetMeterTypeCode(const QString &meter_type_code)
//{
//    m_meter_type_code = meter_type_code;
//}

//void to_json(nlohmann::json &j, const MeterDeviceConfig &info)
//{
//    //    QString m_dev_name; // 名称必须唯一
//    //    MeterDeviceType m_dev_type;
//    //    QString m_server_ip_addr;
//    //    quint32 m_degree_query_cycle_s; // 读数查询周期
////    quint32 m_degree_data_save_max_time_s; // 文件保存周期
////    QString m_save_dir;
//    j["device name"] = info.m_dev_name.toStdString();
//    j["device type"] = info.m_dev_type;
//    j["server ip addr"] = info.m_server_ip_addr.toStdString();
//    j["server port"] = info.m_server_port;
//    j["degree query cycle(s)"] = info.m_degree_query_cycle_s;
//    j["degree data save max time(s)"] = info.m_degree_data_save_max_time_s;
//    j["save dir path"] = info.m_save_dir.toStdString();
//}

//void from_json(const nlohmann::json &j, MeterDeviceConfig &info)
//{
//    info.m_dev_name = QString::fromStdString(j.at("device name").get<std::string>());
//    info.m_dev_type = j.at("device type").get<MeterDeviceType>();
//    info.m_server_ip_addr = QString::fromStdString(j.at("server ip addr").get<std::string>());
//    info.m_server_port = (quint16)j.at("server port").get<int>();
//    info.m_degree_query_cycle_s = (quint32)j.at("degree query cycle(s)").get<int>();
//    info.m_degree_data_save_max_time_s = (quint32)j.at("degree data save max time(s)").get<int>();
//    info.m_save_dir = QString::fromStdString(j.at("save dir path").get<std::string>());
//}

//void to_json(nlohmann::json &j, const WaterMeterDeviceConfig &info)
//{
//    j["device addr code(6B)"] = info.m_dev_addr_code.toStdString();
//    j["manufacturer code(2B)"] = info.m_manufacturer_code.toStdString();
//    j["meter type code(1B)"] = info.m_meter_type_code.toStdString();
//}

//void from_json(const nlohmann::json &j, WaterMeterDeviceConfig &info)
//{
//    info.m_dev_addr_code = QString::fromStdString(j.at("device addr code(6B)").get<std::string>());
//    info.m_manufacturer_code = QString::fromStdString(j.at("manufacturer code(2B)").get<std::string>());
//    info.m_meter_type_code = QString::fromStdString(j.at("meter type code(1B)").get<std::string>());
//}
