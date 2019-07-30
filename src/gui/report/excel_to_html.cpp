#include "excel_to_html.h"

ExcelToHtml::ExcelToHtml(QObject *parent) : QObject(parent)
{

}

void ExcelToHtml::ConvertExcel(const QString &excel_path)
{
    QFileInfo file_info(excel_path);
    if(!file_info.exists())
    {
        QMessageBox::warning(nullptr, tr("Convert"), tr("not find excel path, the port will not show!!!"), QMessageBox::Yes);
        return;
    }

    ////
    /// 使用COM接口时，需要注意使用路径时，需要使用绝对路径，而且需要符合windows的标准，必须使用“\\”而不是“\”
    /// 当使用相对路径，默认路径是系统的“文档”文件夹
    /// 同时注意路径不能太长
    ///
    QAxObject excel( "Excel.Application", nullptr);
    excel.setProperty("Visible", false);
    QAxObject * workbooks = excel.querySubObject("WorkBooks");

    QString full_report_path = QString("%1").arg(file_info.absoluteFilePath());
    workbooks->dynamicCall("Open (const QString&)", QString("%1").arg(QString(full_report_path).replace('/', '\\')));
    QAxObject * workBook = excel.querySubObject("ActiveWorkBook");

    QString out_html_path = QString("%1/%2.html").arg(QApplication::applicationDirPath()).arg(file_info.baseName());
    workBook->dynamicCall("SaveAs (const QString&, int)", QString("%1").arg(out_html_path).replace('/', '\\'), 44);
//    workBook->dynamicCall("SaveAs (const QString&, int)", QString("QApplication::applicationDirPath()/%1").arg("sss.html"), 44);

    workBook->dynamicCall("Close (Boolean)", false);
    excel.dynamicCall("Quit (void)");

    emit ConvertFinish(out_html_path);
}
