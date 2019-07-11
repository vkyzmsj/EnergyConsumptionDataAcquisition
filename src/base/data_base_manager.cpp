#include "data_base_manager.h"
#include <QDebug>
#include <QMessageBox>

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
