#include "report_generator.h"

ReportGenerator::ReportGenerator(QObject *parent) : QObject(parent)
{
    m_report_colome_labels << tr("Device Type") << tr("Device Name") << tr("Start Time") << tr("End Time") << tr("Start Value") << tr("End Value") << tr("Used Value")
          << tr("Wight(Kg)") << tr("Product Num") << tr("Vaild Produce Rate");
}

void ReportGenerator::SetTimeRange(const QDateTime &start_time, const QDateTime &end_time)
{
    m_time_range.first = start_time;
    m_time_range.second = end_time;
}

void ReportGenerator::GenReportTitle(Document &xlsx, CellReference &start_cell)
{
    Format format;
    format.setHorizontalAlignment(Format::AlignHCenter);
    format.setVerticalAlignment(Format::AlignVCenter);
    format.setBottomBorderColor(Qt::gray);
    format.setBottomBorderStyle(Format::BorderStyle::BorderDashed);

    // default row size 15
    QFont font = format.font();
    font.setBold(true);
    format.setFont(font);
    format.setFontSize(24);
    xlsx.setRowHeight(1, 45);
    xlsx.write(start_cell, tr("Product Energy Consumption Report"), format);
    xlsx.mergeCells(CellRange(start_cell, CellReference(start_cell.row(), start_cell.column() + m_report_colome_labels.size() - 1)));
    start_cell.setRow(start_cell.row() + 1);
}

void ReportGenerator::GenSummaryInfo(Document &xlsx, QXlsx::CellReference &start_cell)
{
    Format format;
    format.setHorizontalAlignment(Format::AlignHCenter);
    format.setVerticalAlignment(Format::AlignVCenter);

    QFont font = format.font();
    font.setBold(false);
    font.setItalic(true);
    format.setFont(font);
    format.setFontSize(11);
    xlsx.setRowHeight(1, 15);
    xlsx.write(start_cell, tr("Time Range: %1 - %2  %3").arg(m_time_range.first.toString("yyyy/HH/dd hh:mm:ss")).arg(m_time_range.second.toString("yyyy/HH/dd hh:mm:ss"))
               .arg(tr("Water/Gas Unit: m^3\tPower Unit: KWh")), format);
    xlsx.mergeCells(CellRange(start_cell, CellReference(start_cell.row(), start_cell.column() + m_report_colome_labels.size() - 1)));
    start_cell.setRow(start_cell.row() + 1);
}

void ReportGenerator::GenResultTitle(Document &xlsx, QXlsx::CellReference &start_cell)
{
    QList<QString > title = m_report_colome_labels;

    Format format;
    QFont font = format.font();
    font.setBold(true);
    format.setFont(font);
    format.setFontSize(15);
    format.setHorizontalAlignment(Format::AlignHCenter);
    format.setVerticalAlignment(Format::AlignVCenter);
    format.setBottomBorderStyle(Format::BorderStyle::BorderThick);
    format.setTopBorderStyle(Format::BorderStyle::BorderThick);
    format.setTopBorderColor(Qt::black);
    format.setBottomBorderColor(Qt::black);
    for(auto it : title)
    {
        xlsx.write(start_cell, it, format);
        start_cell.setColumn(start_cell.column() + 1);
    }
    start_cell.setRow(start_cell.row() + 1);
    start_cell.setColumn(1);
}

void ReportGenerator::GenMeasureItemListContext(Document &xlsx,  QXlsx::CellReference &start_cell, const QString &device_type_name, const QList<MeasureItem> &list)
{
    auto curr_cell = start_cell;

    auto GenDeviceTypeColum = [](Document &xlsx, const QString &device_type_name, int item_cnt, const CellReference &cell){
        Format format;
        format.setHorizontalAlignment(Format::AlignHCenter);
        format.setVerticalAlignment(Format::AlignVCenter);
        format.setFontSize(14);
        xlsx.write(cell, device_type_name, format);
        xlsx.mergeCells(CellRange(cell, CellReference(cell.row() + item_cnt - 1, cell.column())));
    };

    auto GenItem = [](Document &xlsx, const CellReference &cell_ref, const MeasureItem &item, const Format &format){
        auto colum = cell_ref.column();
        xlsx.write(cell_ref, item.name, format);
        colum++;
        xlsx.write(CellReference(cell_ref.row(), colum++), item.start_time.toString("yyyyMMdd hh:mm:ss"), format);
        xlsx.write(CellReference(cell_ref.row(), colum++), item.end_time.toString("yyyyMMdd hh:mm:ss"), format);
        xlsx.write(CellReference(cell_ref.row(), colum++), item.start_val, format);
        xlsx.write(CellReference(cell_ref.row(), colum++), item.end_val, format);
        xlsx.write(CellReference(cell_ref.row(), colum++), item.end_val - item.start_val, format);
        xlsx.write(CellReference(cell_ref.row(), colum++), " ", format);
        xlsx.write(CellReference(cell_ref.row(), colum++), " ", format);
    };

    GenDeviceTypeColum(xlsx, device_type_name, list.size(), curr_cell);
    curr_cell.setColumn(2);

    int index = 0;
    Format format_blank, format_rich;
    format_blank.setFontSize(14);
    format_blank.setHorizontalAlignment(Format::AlignHCenter);
    format_blank.setVerticalAlignment(Format::AlignVCenter);
    format_rich = format_blank;
    format_rich.setPatternBackgroundColor(QColor(Qt::gray));

    for(auto it: list)
    {
        GenItem(xlsx, curr_cell, it, ((index% 2) == 0)? format_blank : format_rich) ;
        curr_cell.setRow(curr_cell.row() + 1);
        index = (index + 1) & 0x01;
    }
    start_cell.setColumn(1);
    start_cell.setRow(start_cell.row() + list.size());
}

ReportGenerator::MeasureItem ReportGenerator::ToMeasureItem(const QString &device_name,  const QList<WaterDeviceRecord> &list)
{
    ReportGenerator::MeasureItem out_item;
    QMap<QDateTime, WaterDeviceRecord> in_map;
    for(auto it : list)
    {
        in_map[it.date_time] = it;
    }
    if(in_map.size() > 0)
    {
        out_item.name = device_name;
        out_item.start_time = in_map.firstKey();
        out_item.start_val = in_map.first().measure_value;
        out_item.end_time = in_map.lastKey();
        out_item.end_val = in_map.last().measure_value;
    }
    return  out_item;
}

void ReportGenerator::GenReport()
{
    QXlsx::Document xlsx;
    QXlsx::CellReference start_cell(1,1);
    GenReportTitle(xlsx, start_cell);
    GenSummaryInfo(xlsx, start_cell);
    GenResultTitle(xlsx, start_cell);

    xlsx.setColumnWidth(1, 10, 22);
    xlsx.setColumnWidth(1, 20);
    xlsx.setColumnWidth(3, 4, 25);



    // 水表
    QList<MeasureItem> water_measure_list = GetWaterDeviceMeasureList();
    GenMeasureItemListContext(xlsx, start_cell, tr("Water Meter Device"), water_measure_list);

    xlsx.workbook()->renameSheet(0, "Report");
    QString file_path = QString("%1/report_%2.xlsx").arg(Config::Instance()->GetReportOutputPath()).arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
    xlsx.saveAs(file_path);
    emit ReportReady(file_path);
}

QList<ReportGenerator::MeasureItem> ReportGenerator::GetWaterDeviceMeasureList()
{
    QList<ReportGenerator::MeasureItem> list;
    auto device_list = DataBaseManager::Instance()->QueryWaterDeviceInfoList();
    for(auto it: device_list)
    {
        auto measure_val_list = DataBaseManager::Instance()->QueryWaterDeviceRecordList(it.device_name, m_time_range.first, m_time_range.second);
        auto item = ToMeasureItem(it.device_name, measure_val_list);
        list.append(item);
    }
    return list;
}

void ReportGenerator::StartGenReport()
{
    GenReport();
}
