#ifndef CONFIG_H
#define CONFIG_H
#include "../device/device.h"
#include <QMap>


class Config
{
public:
    static Config *Instance();
    void Save();

    QString GetDbFilePath() const;

    QString GetReportOutputPath() const;
    void SetReportOutputPath(const QString &report_path);

private:
    Config();
    void Load();

private:
    const QString m_config_file_path;
    const QString m_db_file_path;
    QString m_report_output_path;
};

#endif // CONFIG_H
