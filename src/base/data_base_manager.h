#ifndef DATA_BASE_MANAGER_H
#define DATA_BASE_MANAGER_H
#include <QString>
#include <QMap>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QObject>

class DataBaseManager : public QObject
{
    Q_OBJECT
public:
    static DataBaseManager *Instance();
    void AddWaterMeterRecord();
    ~DataBaseManager();


private:
    explicit DataBaseManager(QObject *parent = nullptr);
    bool InitDb();

signals:

public slots:

private:
    const QString m_db_file_path;
    QSqlDatabase m_db;
};

#endif // DATA_BASE_MANAGER_H
