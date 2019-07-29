#ifndef DATA_BASE_MANAGER_H
#define DATA_BASE_MANAGER_H
#include <QString>
#include <QMap>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QObject>
#include "../device/device.h"

class DataBaseManager : public QObject
{
    Q_OBJECT
public:
    static DataBaseManager *Instance();
    void AddWaterMeterRecord();
    ~DataBaseManager();


    // 删除
    bool DeleteWaterDevice(const QString &dev_name);

    // 修改
    bool InsertWaterDeviceInfo(const WaterDeviceInfo &info);
    bool UpdateWaterDeviceInfoByDeviceName(const WaterDeviceInfo &info);

    bool InsertWaterDeviceRecord(const QString &dev_name, const WaterDeviceRecord &record);
    // 查询
    QList<WaterDeviceInfo> QueryWaterDeviceInfoList();
    QList<WaterDeviceRecord> QueryWaterDeviceRecordList(const QString &device_name, const QDateTime &start_time, const QDateTime &end_time);
    int QueryWaterDeviceId(const QString &name);


private:
    explicit DataBaseManager(QObject *parent = nullptr);
    bool InitDb();
    bool CreateTableIfNotExist();

    void Test();
    void InsertTest();
    void UpdateTest();
    void QueryTest();
    void DeleteTest();

signals:

public slots:

private:
    const QString m_db_file_path;
    QSqlDatabase m_db;
    static const QString m_db_table_water_device_config;
    static const QString m_db_table_water_device_record;
};

#endif // DATA_BASE_MANAGER_H
