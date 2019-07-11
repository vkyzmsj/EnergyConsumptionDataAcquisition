#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "src/gui/frame_tools.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitWaterMeterDevice()
{
    QList<WaterMeterDeviceConfig> list = Config::Instance()->GetWaterDeviceConfigList();
    for(auto c : list)
    {
        if(!m_water_meter_device_display_items.contains(c.GetDevName()))
        {
            Frame_WaterMeterDisplayItem *item = new Frame_WaterMeterDisplayItem;
            item->SetWaterMeterDeviceConfig(c);
            ui->verticalLayout_water_meter_display->addWidget(item);
        }
    }
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
    WaterMeterDeviceConfig water_meter_config;
    edit.SetWaterMeterDeviceConfig(water_meter_config);
    edit.show();

    QEventLoop loop;
    connect(&edit, &Frame_WaterMeterDeviceConfigEdit::Finished, &loop, &QEventLoop::quit);
    loop.exec();

    if(edit.ExitWithYes())
    {
        qDebug() <<":::";
        Config::Instance()->AddWaterDeviceConfig(edit.GetWaterMeterDeviceConfig());
        Config::Instance()->Save();
    }

}

void MainWindow::on_actionUpdateDeviceStatus_triggered()
{
    InitWaterMeterDevice();
}
