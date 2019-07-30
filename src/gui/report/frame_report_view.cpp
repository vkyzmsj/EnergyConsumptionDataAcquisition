#include "frame_report_view.h"
#include "ui_frame_report_view.h"
#include <QMetaMethod>
#include <QMetaObject>
#include <QAction>

FrameReportView::FrameReportView(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Frame_report_view)
{
    ui->setupUi(this);
    Init();
}

FrameReportView::~FrameReportView()
{
    delete ui;
}

void FrameReportView::LoadNewFile(const QString &file_path)
{
    ui->pushButton_gen_report->setText(tr("Gen"));
    m_web_page->load(QUrl(QString("file:/%1").arg(file_path)));
}

void FrameReportView::Init()
{
    m_web_view = new QWebEngineView();
//    m_js_context = new JsContext(this);
//    m_web_channel = new QWebChannel(this);
    m_web_page = new QWebEnginePage();
    m_web_view->setPage(m_web_page);
    m_web_view->setContextMenuPolicy(Qt::ActionsContextMenu);
//    QAction *print_action = new QAction(tr("Print"));
//    m_print = new QPrinter();
//    connect(print_action, &QAction::toggle, this, [&](){
//        m_web_view->page()->print(m_print,  [=](bool){});
//    });
//    m_web_view->addAction(print_action);
//    m_web_channel->registerObject("context", m_js_context);
//    m_web_page->setWebChannel(m_web_channel);
//    connect(m_js_context, &JsContext::RecvdMsg, this, [&](const QString& msg) {
////        ui->statusBar->showMessage(QString("Received message：%1").arg(msg), 3000);
//        qDebug() << msg;
//    });

//    m_web_page->load(QUrl("file:/report.html"));
    ui->verticalLayout->addWidget(m_web_view);

    m_report_generator = new ReportGenerator();
    QThread *thread = new QThread();
    m_report_generator->moveToThread(thread);
    thread->start();

    m_excel_to_html = new ExcelToHtml();
    QThread *thread2 = new QThread();
    m_excel_to_html->moveToThread(thread2);
    thread2->start();

    connect(ui->pushButton_gen_report, &QPushButton::clicked, [&](){
        ui->pushButton_gen_report->setText(tr("gen..."));
        m_report_generator->SetTimeRange(ui->dateTimeEdit_start_time->dateTime(), ui->dateTimeEdit_end_time->dateTime());
        QMetaObject::invokeMethod(m_report_generator,  &ReportGenerator::StartGenReport);
    });
    connect(m_report_generator, &ReportGenerator::ReportReady, m_excel_to_html, &ExcelToHtml::ConvertExcel);
    connect(m_excel_to_html, &ExcelToHtml::ConvertFinish, this, &FrameReportView::LoadNewFile);
}

void FrameReportView::on_pushButton_open_report_dir_clicked()
{
    QString cmd = QString("start %1").arg(Config::Instance()->GetReportOutputPath());
    system(cmd.toStdString().c_str());
}
