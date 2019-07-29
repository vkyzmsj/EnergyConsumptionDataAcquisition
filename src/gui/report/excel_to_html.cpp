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

    QAxWidget *excel = new QAxWidget("Excel.Application") ;
    excel->setProperty("Visible", false);
    QAxObject * workbooks = excel->querySubObject("WorkBooks");
    workbooks->dynamicCall("Open (const QString&)", QString("%1").arg(excel_path));
    QAxObject * workBook = excel->querySubObject("ActiveWorkBook");

    QString out_html_path = QString("%1/report_%2.html").arg(QApplication::applicationDirPath()).arg(file_info.baseName());
    workBook->dynamicCall("SaveAs (const QString&, int)", QString("%1").arg(out_html_path), 44);

    workBook->dynamicCall("Close (Boolean)", false);
    excel->dynamicCall("Quit (void)");

    delete workBook;
    delete excel;

    emit ConvertFinish(out_html_path);
}
