#ifndef FRAME_REPORT_VIEW_H
#define FRAME_REPORT_VIEW_H

#include <QFrame>
#include <QWebEnginePage>
#include <QWebEngineView>
#include <QWebChannel>
#include <QDebug>
#include <QThread>
#include <QPrinter>
#include "../../logic/report_generator.h"

#include "js_context.h"
#include "excel_to_html.h"

namespace Ui {
class Frame_report_view;
}

class FrameReportView : public QFrame
{
    Q_OBJECT

public:
    explicit FrameReportView(QWidget *parent = nullptr);
    ~FrameReportView();

public slots:
    void LoadNewFile(const QString &file_path);

private slots:
    void on_pushButton_open_report_dir_clicked();

private:
    void Init();

private:
    Ui::Frame_report_view *ui;
//    QWebEnginePage *m_web_page;
    QWebEngineView *m_web_view;
    QWebChannel *m_web_channel;
    QWebEnginePage *m_web_page;
    JsContext *m_js_context;
    ReportGenerator *m_report_generator;
    ExcelToHtml *m_excel_to_html;
    QList<QThread *> m_thread_list;
    QPrinter *m_print;
};

#endif // FRAME_REPORT_VIEW_H
