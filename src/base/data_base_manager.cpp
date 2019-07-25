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
    QString query_cmd = QString("INSERT INTO %1 ("
                                "DeviceName,\
                                ServerIp,\
                                ServerPort,\
                                DeviceAddress,\
                                ManuCode,\
                                MeterType,\
                                StatusCode) VALUES (?,?,?,?,?,?,?)").arg(m_db_table_water_device_config);

                        QSqlQuery result(query_cmd, m_db);
    result.bindValue(0, info.device_name);
    result.bindValue(1, info.server_ip);
    result.bindValue(2, info.server_port);
    result.bindValue(3, info.device_address.toHex().left(12));
    result.bindValue(4, info.manu_code.toHex().left(4));
    result.bindValue(5, info.meter_type.toHex().left(2));
    result.bindValue(6, (int)info.device_status);

    if(result.exec())
    {
        qWarning() << __FUNCTION__ <<": insrt ok ";
        return true;
    }else{
        qWarning() << __FUNCTION__ <<": query error " << result.lastError().text();
        return false;
    }
}

bool DataBaseManager::UpdateWaterDeviceInfoByDeviceName(const WaterDeviceInfo &info)
{
    QString query_cmd = QString("UPDATE %1 set \
                                ServerIp = ?,\
                                ServerPort = ?,\
                                DeviceAddress = ?,\
                                ManuCode = ?,\
                                MeterType = ?,\
                                StatusCode = ? where DeviceName = \"%2\"").arg(m_db_table_water_device_config)
            .arg(info.device_name);

    QSqlQuery result(query_cmd, m_db);
    result.addBindValue(info.server_ip);
    result.addBindValue(info.server_port);
    result.addBindValue(info.device_address.toHex().left(12));
    result.addBindValue(info.manu_code.toHex().left(4));
    result.addBindValue(info.meter_type.toHex().left(2));
    result.addBindValue((int)info.device_status);

    if(result.exec())
    {
        qInfo() << __FUNCTION__ <<": update ok ";
        return true;
    }else{
        qWarning() << __FUNCTION__ <<": query error " << result.lastError().text();
        return false;
    }
}

bool DataBaseManager::InsertWaterDeviceRecord(const QString &dev_name, const WaterDeviceRecord &record)
{
    // 删除数据记录表中的数据
    int device_id = QueryWaterDeviceId(dev_name);
    if(device_id == -1)
    {
        qWarning() << __FUNCTION__ <<": no result";
        return false;
    }

    QString query_cmd = QString("INSERT INTO %1 (                                  DeviceId,\
                                RecordTime,\
                                MeasureValue\
                                ) VALUES (?,?,?)").arg(m_db_table_water_device_record);

                        QSqlQuery result(query_cmd, m_db);
    result.addBindValue(device_id);
    result.addBindValue(record.date_time.toString("yyyyMMdd hh:mm:ss"));
    result.addBindValue(record.measure_value);

    if(result.exec())
    {
        qInfo() << __FUNCTION__ <<": insrt ok ";
        return true;
    }else{
        qWarning() << __FUNCTION__ <<": query error " << result.lastError().text();
        return false;
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
                                StatusCode FROM %1").arg(m_db_table_water_device_config);

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
            info.device_address = QByteArray::fromHex(result.value(3).toString().toStdString().c_str());
            info.manu_code = QByteArray::fromHex(result.value(4).toString().toStdString().c_str());
            info.meter_type = (QByteArray::fromHex(result.value(5).toString().toStdString().c_str()));
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
    QString query_cmd = QString("select RecordTime, MeasureValue from %1 where DeviceId = %2 and  RecordTime between \"%3\" and \"%4\"")
                        .arg(m_db_table_water_device_record)
                        .arg(device_id).arg(start_time.toString("yyyyMMdd hh:mm:ss")).arg(end_time.toString("yyyyMMdd hh:mm:ss"));
    QSqlQuery result = m_db.exec(query_cmd);
    QList<WaterDeviceRecord> list;
    if(result.exec())
    {
        while(result.next())
        {
            WaterDeviceRecord info;
            info.date_time = QDateTime::fromString(result.value(0).toString(), "yyyyMMdd hh:mm:ss");
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
    m_db_file_path("ecda2.db")
{
    if(!InitDb())
    {
        QMessageBox::warning(nullptr, tr("Data Base Manager"),
                             tr("Init db file failed, error message is %1, stop the process").arg(m_db.lastError().text()),
                             QMessageBox::Yes);
        abort();
    }
    if(!CreateTableIfNotExist())
    {
        QMessageBox::warning(nullptr, tr("Data Base Manager"),
                             tr("check table of db failed, error message is %1, stop the process").arg(m_db.lastError().text()),
                             QMessageBox::Yes);
        abort();
    }
    Test();
}

bool DataBaseManager::InitDb()
{

    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(m_db_file_path);
    return m_db.open();
}

bool DataBaseManager::CreateTableIfNotExist()
{
    static QString create_water_device_config_table_cmd = "CREATE TABLE water_device_info (\
                                                          DeviceId      INTEGER   PRIMARY KEY AUTOINCREMENT\
                                                          UNIQUE,\
            DeviceName    TEXT      NOT NULL\
            UNIQUE,\
            ServerIp      TEXT,\
            ServerPort    INTEGER,\
            DeviceAddress CHAR (12),\
            ManuCode      CHAR (4),\
            MeterType     CHAR (2),\
            StatusCode    INTEGER   NOT NULL\
            )";
            static QString create_water_record_table_cmd = "CREATE TABLE water_meter_record (\
                                                           DeviceId     INTEGER,\
            RecordTime   DATE,\
            MeasureValue REAL    NOT NULL\
            )";

            QString query_cmd = "select name from sqlite_master where type='table' order by name";
    QSqlQuery query(m_db);
    if(query.exec(query_cmd))
    {
        QStringList list;
        while(query.next())
        {
            list.append(query.value(0).toString());
        }
        QStringList table_list;
        table_list << m_db_table_water_device_config << m_db_table_water_device_record;
        QSqlQuery create_query(m_db);
        for(auto it : table_list)
        {
            if(!list.contains(it))
            {
                if(it == m_db_table_water_device_config)
                {
                    if(!create_query.exec(create_water_device_config_table_cmd))
                    {
                        qWarning() << __FUNCTION__<<": " << create_query.lastError().text();
                        return false;
                    }
                }else if(it == m_db_table_water_device_record){
                    if(!create_query.exec(create_water_record_table_cmd))
                    {
                        qWarning() << __FUNCTION__<<": " << create_query.lastError().text();
                        return false;
                    }
                }
            }
        }
        return true;
    }
    return false;
}

void DataBaseManager::Test()
{
//    InsertTest();
//    UpdateTest();
//    QueryTest();
    DeleteTest();

}

void DataBaseManager::InsertTest()
{
    WaterDeviceInfo base_info;
    base_info.device_name = "k";
    base_info.device_address = QByteArray::fromHex("010203040506");
    base_info.device_status = DeviceStatus::Activite;
    base_info.manu_code = QByteArray::fromHex("02040608");
    base_info.meter_type = QByteArray::fromHex("01");
    base_info.server_ip = "127.0.0.5";
    base_info.server_port = 22;
    QDateTime base_date_time = QDateTime::currentDateTime();

    for(int i = 0; i < 10; i++)
    {
        WaterDeviceInfo info = base_info;
        info.device_name = info.device_name + QString::number(i);
        InsertWaterDeviceInfo(info);

        WaterDeviceRecord record;
        record.date_time = QDateTime::currentDateTime();

        record.measure_value = i;
        InsertWaterDeviceRecord(info.device_name, record);

        for(int m = 0; m < 10; m++)
        {
            record.date_time = base_date_time.addDays(m);
            for(int n = 0; n < 5; n++)
            {
                record.date_time = record.date_time.addSecs(n + 10);
                record.measure_value = QString("%1.%2").arg(m).arg(n).toDouble();
                InsertWaterDeviceRecord(info.device_name, record);
            }
        }
    }

}

void DataBaseManager::UpdateTest()
{
    WaterDeviceInfo base_info;
    base_info.device_name = "k0";
    base_info.device_address = QByteArray::fromHex("010203040506");
    base_info.device_status = DeviceStatus::Activite;
    base_info.manu_code = QByteArray::fromHex("02040608");
    base_info.meter_type = QByteArray::fromHex("06");
    base_info.server_ip = "127.0.0.5";
    base_info.server_port = 22;

    UpdateWaterDeviceInfoByDeviceName(base_info);
}

void DataBaseManager::QueryTest()
{
    QList<WaterDeviceInfo> info_list;
    QMap<QString , QList<WaterDeviceRecord>> rec_map;
    info_list = QueryWaterDeviceInfoList();
    for(auto it : info_list)
    {
        qDebug() << it;
        QList<WaterDeviceRecord> rec_list = QueryWaterDeviceRecordList(it.device_name, QDateTime::fromString("20190728 11:55:36", "yyyyMMdd hh:mm:ss"),
                                                                       QDateTime::fromString("20190731 11:55:43", "yyyyMMdd hh:mm:ss"));
        for(auto i : rec_list)
        {
            qDebug() << i;
        }

        qDebug() << "+++++++++++++++++++++++++++++++++";
    }
}

void DataBaseManager::DeleteTest()
{
    DeleteWaterDevice("k8");
}
