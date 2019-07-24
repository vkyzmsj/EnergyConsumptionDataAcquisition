#include "data_base_manager.h"
#include <QDebug>
#include <QMessageBox>

const QString DataBaseManager::m_db_table_water_device_config = "water_device_info";
const QString DataBaseManager::m_db_table_water_device_record = "water_meter_record";


DataBaseManager *DataBaseManager::Instance()
{
    static DataBaseManager manager;
    return &manager;
}

DataBaseManager::~DataBaseManager()
{
    if(m_db.isOpen())
        m_db.close();
}

bool DataBaseManager::DeleteWaterDevice(const QString &dev_name)
{
    // 删除数据记录表中的数据
    int device_id = QueryWaterDeviceId(dev_name);
    if(device_id == -1)
    {
        qWarning() << __FUNCTION__ <<": no result";
        return true;
    }

    QString query_cmd = QString("delete from %1 where DeviceId=%2").arg(m_db_table_water_device_record).arg(device_id);
    QSqlQuery result = m_db.exec(query_cmd);
    if(!result.lastError().isValid())
    {
        qWarning() << __FUNCTION__ <<": delete ok " << result.lastError().text();
    }else{
        qWarning() << __FUNCTION__ <<": query error " << result.lastError().text();
    }
    // 删除信息表中的记录

    query_cmd = QString("DELETE FROM %1 where DeviceName = \"%2\"").arg(m_db_table_water_device_config).arg(dev_name);
    result = m_db.exec(query_cmd);
    if(!result.lastError().isValid())
    {
        qWarning() << __FUNCTION__ <<": delete ok " << result.lastError().text();
    }else{
        qWarning() << __FUNCTION__ <<": query error " << result.lastError().text();
    }
    return true;
}

bool DataBaseManager::InsertWaterDeviceInfo(const WaterDeviceInfo &info)
{
    QString query_cmd = QString("INSERT INTO %1 (                                  DeviceName,\
                                ServerIp,\
                                ServerPort,\
                                DeviceAddress,\
                                ManuCode,\
                                MeterType,\
                                StatusCode) VALUES (?,?,?,?,?,?,?)").arg(m_db_table_water_device_record);

                        QSqlQuery result(query_cmd, m_db);
    result.bindValue(info.device_name);
    result.bindValue(info.server_ip);

    result.bindValue(info.server_port);

    result.bindValue(info.device_address.toHex().left(12));

    result.bindValue(info.manu_code.toHex().left(4));

    result.bindValue(QString("%1").arg((uchar)info.meter_type, 2, 16, QChar(0)));

    result.bindValue((int)info.device_status);

    if(result.exec())
    {
        qWarning() << __FUNCTION__ <<": insrt ok ";
    }else{
        qWarning() << __FUNCTION__ <<": query error " << result.lastError().text();
    }
}

bool DataBaseManager::UpdateWaterDeviceInfoByDeviceName(const WaterDeviceInfo &info)
{
    QString query_cmd = QString("UPDATE INTO %1 ( set \
                                ServerIp = ?,\
                                ServerPort = ?,\
                                DeviceAddress = ?,\
                                ManuCode = ?,\
                                MeterType = ?,\
                                StatusCode = ?) where DeviceName = \"%2\"").arg(m_db_table_water_device_config)
            .arg(info.device_name);

    QSqlQuery result(query_cmd, m_db);
    result.bindValue(info.server_ip);

    result.bindValue(info.server_port);

    result.bindValue(info.device_address.toHex().left(12));

    result.bindValue(info.manu_code.toHex().left(4));

    result.bindValue(QString("%1").arg((uchar)info.meter_type, 2, 16, QChar(0)));

    result.bindValue((int)info.device_status);

    if(result.exec())
    {
        qWarning() << __FUNCTION__ <<": update ok ";
    }else{
        qWarning() << __FUNCTION__ <<": query error " << result.lastError().text();
    }
}

void DataBaseManager::InsertWaterDeviceRecord(const QString &dev_name, const WaterDeviceRecord &record)
{
    // 删除数据记录表中的数据
    int device_id = QueryWaterDeviceId(dev_name);
    if(device_id == -1)
    {
        qWarning() << __FUNCTION__ <<": no result";
        return true;
    }

    QString query_cmd = QString("INSERT INTO %1 (                                  DeviceId,\
                                RecordTime,\
                                MeasureValue\
                                ) VALUES (?,?,?)").arg(m_db_table_water_device_record);

                        QSqlQuery result(query_cmd, m_db);
    result.bindValue(device_id);
    result.bindValue(record.date_time.toString());
    result.bindValue(record.measure_value);

    if(result.exec())
    {
        qWarning() << __FUNCTION__ <<": insrt ok ";
    }else{
        qWarning() << __FUNCTION__ <<": query error " << result.lastError().text();
    }
}

QList<WaterDeviceInfo> DataBaseManager::QueryWaterDeviceInfoList()
{
    QString query_cmd = QString("SELECT DeviceName,\
                                ServerIp,\
                                ServerPort,\
                                DeviceAddress,\
                                ManuCode,\
                                MeterType,\
                                StatusCode FROM %1").arg(m_db_table_water_device_record);

                                QList<WaterDeviceInfo> list;
                        QSqlQuery result = m_db.exec(query_cmd);
    if(result.exec())
    {
        while(result.next())
        {
            WaterDeviceInfo info;
            info.device_name = result.value(0).toString();
            info.server_ip = result.value(1).toString();
            info.server_port = result.value(2).toUInt();
            info.device_address = QByteArray::fromHex(result.value(3).toString());
            info.manu_code = QByteArray::fromHex(result.value(4).toString());
            info.meter_type = (QByteArray::fromHex(result.value(5).toString()).at(0));
            info.device_status = (DeviceStatus)result.value(6).toInt();

            list << info;
        }
    }else{
        qWarning() << __FUNCTION__ <<": " << result.lastError().text();
    }
    return list;
}

QList<WaterDeviceRecord> DataBaseManager::QueryWaterDeviceRecordList(const QString &device_name, const QDateTime &start_time, const QDateTime &end_time)
{
    int device_id = QueryWaterDeviceId(device_name);
    QString query_cmd = QString("select RecordTime, MeasureValue from %1 where RecordTime between ? and ?");
    QSqlQuery result = m_db.exec(query_cmd);
    QList<WaterDeviceRecord> list;
    if(result.exec())
    {
        while(result.next())
        {
            WaterDeviceRecord info;
            info.date_time = QDateTime::fromString(result.value(0).toString());
            info.measure_value = result.value(1).toDouble();
            list << info;
        }
    }else{
        qWarning() << __FUNCTION__ <<": " << result.lastError().text();
    }
    return list;
}

int DataBaseManager::QueryWaterDeviceId(const QString &name)
{
    QString query_cmd = QString("select DeviceId from %1 where DeviceName = \"%2\"").arg(m_db_table_water_device_config)
                        .arg(name);
    QSqlQuery result = m_db.exec(query_cmd);
    if(!result.lastError().isValid())
    {
        while(result.next())
        {
            return result.value(0).toInt();
        }
        qWarning() << __FUNCTION__ <<": no record " << result.lastError().text();
        return -1;
    }else{
        qWarning() << __FUNCTION__ <<": query error " << result.lastError().text();
        return -1;
    }
}

DataBaseManager::DataBaseManager(QObject *parent) : QObject(parent),
    m_db_file_path("ecda.db")
{
    if(!InitDb())
    {
        QMessageBox::warning(nullptr, tr("Data Base Manager"),
                             tr("Init db file failed, error message is %1, stop the process").arg(m_db.lastError().text()),
                             QMessageBox::Yes);
        abort();
    }
}

bool DataBaseManager::InitDb()
{

    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("MyDataBase.db");
    return m_db.open();
}
