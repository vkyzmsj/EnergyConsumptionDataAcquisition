#ifndef EXCEL_TO_HTML_H
#define EXCEL_TO_HTML_H

#include <QObject>
#include <QAxWidget>
#include <QAxObject>
#include <QFile>
#include <QFileInfo>
#include <QDialog>
#include <QMessageBox>
#include <QApplication>
#include <QDateTime>

class ExcelToHtml : public QObject
{
    Q_OBJECT
public:
    explicit ExcelToHtml(QObject *parent = nullptr);


signals:
    const QString ConvertFinish(const QString &file_path);



public slots:
    void ConvertExcel(const QString &excel_path);
};

#endif // EXCEL_TO_HTML_H
