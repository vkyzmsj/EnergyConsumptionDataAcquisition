#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "src/gui/frame_tools.h"
#include "src/gui/report/frame_report_view.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_db_manager = DataBaseManager::Instance();

    ui->actionStop->setEnabled(false);
    InitWaterMeterDevice();

    ui->verticalLayout_report->addWidget(new FrameReportView);
    setWindowTitle(tr("EnergyConsumptionDataAcquisition"));
    resize(1400 , 800);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitWaterMeterDevice()
{
    frame_water_meter_display = new Frame_WaterMeterDisplay();
    frame_water_meter_display->SyncView();
//    frame_water_meter_display->show();
    ui->verticalLayout_water_meter->addWidget(frame_water_meter_display);

    frame_gas_meter_display = new Frame_GasMeterDisplay();
    frame_gas_meter_display->SyncView();

    ui->verticalLayout_gas_meter->addWidget(frame_gas_meter_display);
//    frame_gas_meter_display->show();

    frame_power_meter_display = new Frame_PowerMeterDisplay();
    frame_power_meter_display->SyncView();

    ui->horizontalLayout_device_monitor->addWidget(frame_power_meter_display);


}

void MainWindow::on_actionHelpTools_triggered()
{
    Frame_Tools *frame = new Frame_Tools;
    frame->setAttribute(Qt::WA_DeleteOnClose);
    frame->show();
}

void MainWindow::on_actionAddWaterDevice_triggered()
{
    Frame_WaterMeterDeviceConfigEdit edit;
    WaterDeviceInfo water_meter_info;
    edit.SetDeviceInfo(water_meter_info);
    edit.show();

    QEventLoop loop;
    connect(&edit, &Frame_WaterMeterDeviceConfigEdit::Finished, &loop, &QEventLoop::quit);
    loop.exec();
}

void MainWindow::on_actionUpdateDeviceStatus_triggered()
{
    InitWaterMeterDevice();
}

void MainWindow::on_actionSyncView_triggered()
{
    frame_water_meter_display->SyncView();
}

void MainWindow::on_actionStart_triggered()
{
    ui->actionStart->setEnabled(false);
    ui->actionStop->setEnabled(true);
}

void MainWindow::on_actionStop_triggered()
{
    ui->actionStart->setEnabled(true);
    ui->actionStop->setEnabled(false);
}
