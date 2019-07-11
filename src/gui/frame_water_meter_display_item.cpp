#include "frame_water_meter_display_item.h"
#include "ui_frame_water_meter_display_item.h"

Frame_WaterMeterDisplayItem::Frame_WaterMeterDisplayItem(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Frame_water_meter_display_item)
{
    ui->setupUi(this);
    m_query_thread = new WaterMeterQueryThread;
    connect(m_query_thread, &WaterMeterQueryThread::NetConnectStatusChanged, this, [&](QAbstractSocket::SocketState state){
        switch (state) {
            case QAbstractSocket::SocketState::UnconnectedState:
            {
                ui->label_connect_status->setText("Unconnected");
                break;
            }
            case QAbstractSocket::SocketState::ConnectingState:
            {
                ui->label_connect_status->setText("Connecting");
                break;
            }
            case QAbstractSocket::SocketState::ConnectedState:
            {
                ui->label_connect_status->setText("Connected");
                break;
            }
            default:
                break;
        }
    });
    connect(m_query_thread, &WaterMeterQueryThread::started, this, [&](){
        ui->pushButton_start_req->setText("Stop");
        ui->pushButton_start_req->setToolTip("Query Is Running");
    });
    connect(m_query_thread, &WaterMeterQueryThread::finished, this, [&](){
        ui->pushButton_start_req->setText("Start");
        ui->pushButton_start_req->setToolTip("");
    });
}

Frame_WaterMeterDisplayItem::~Frame_WaterMeterDisplayItem()
{
    delete ui;
}

bool Frame_WaterMeterDisplayItem::IsEnable() const
{
    return m_water_meter_device_config.IsEnable();
}

bool Frame_WaterMeterDisplayItem::IsRunning() const
{
    return m_query_thread->isRunning();
}

void Frame_WaterMeterDisplayItem::StartReq()
{
    m_query_thread->start();
}

void Frame_WaterMeterDisplayItem::StopReq()
{
    m_query_thread->StopReq();
}

QString Frame_WaterMeterDisplayItem::GetDeviceName() const
{
    return m_water_meter_device_config.GetDevName();
}

void Frame_WaterMeterDisplayItem::on_pushButton_start_req_clicked()
{
    if(m_water_meter_device_config.IsEnable())
        m_query_thread->start();
}

void Frame_WaterMeterDisplayItem::UpdateMeterVal(QDateTime date_time, double val)
{
    ui->lcdNumber_meter_number->display(val);
    ui->lcdNumber_meter_number->setToolTip(QString("update: %1").arg(date_time.toString("hh:mm:ss")));
}

WaterMeterDeviceConfig Frame_WaterMeterDisplayItem::GetWaterMeterDeviceConfig() const
{
    return m_water_meter_device_config;
}

void Frame_WaterMeterDisplayItem::SetWaterMeterDeviceConfig(const WaterMeterDeviceConfig &water_meter_device_config)
{
    m_water_meter_device_config = water_meter_device_config;
    Init();
}

void Frame_WaterMeterDisplayItem::Init()
{
    ui->groupBox_device->setTitle(m_water_meter_device_config.GetDevName());
    ui->label_device_addr->setText(m_water_meter_device_config.GetDevAddrCode());
    ui->label_connect_status->setText("Unconnected");
}
