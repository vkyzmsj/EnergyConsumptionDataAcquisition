#ifndef REPORT_GENERATOR_H
#define REPORT_GENERATOR_H

#include <QObject>
#include <QColor>
#include <QPair>
#include <QDateTime>
#include "../device/device.h"
#include "../base/data_base_manager.h"
#include "../base/config.h"
#include "xlsxdocument.h"
#include "xlsxformat.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"
#include "xlsxformat.h"
#include "xlsxabstractsheet.h"

QTXLSX_USE_NAMESPACE
class ReportGenerator : public QObject
{
    Q_OBJECT
    struct MeasureItem
    {
        QString name;
        QDateTime start_time, end_time;
        double start_val, end_val;
        MeasureItem() : name("---"), start_time(QDateTime::fromString("19790102 12:00:00", "yyyyMMdd hh:mm:ss")),
            end_time(start_time), start_val(0), end_val(start_val){

        }
    };

public:
    explicit ReportGenerator(QObject *parent = nullptr);
    void SetTimeRange(const QDateTime &start_time, const QDateTime &end_time);
private:
    void GenReportTitle(QXlsx::Document &m_xlsx, QXlsx::CellReference &start_cell);
    void GenSummaryInfo(QXlsx::Document &m_xlsx, QXlsx::CellReference &start_cell);
    void GenResultTitle(QXlsx::Document &xlsx, QXlsx::CellReference &start_cell);
    void GenMeasureItemListContext(Document &m_xlsx, QXlsx::CellReference &start_cell, const QString &device_type_name, const QList<MeasureItem> &list);
    MeasureItem ToMeasureItem(const QString &device_name, const QList<WaterDeviceRecord> &list);
    void GenReport();

    QList<MeasureItem> GetWaterDeviceMeasureList();

signals:
    void ReportReady(const QString &report_path);

public slots:
    void StartGenReport();

private:
    QPair<QDateTime, QDateTime> m_time_range;
};

#endif // REPORT_GENERATOR_H
