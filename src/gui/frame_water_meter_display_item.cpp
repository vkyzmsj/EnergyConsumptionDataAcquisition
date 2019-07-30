#include "frame_water_meter_display_item.h"
#include "ui_frame_water_meter_display_item.h"

Frame_WaterMeterDisplayItem::Frame_WaterMeterDisplayItem(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Frame_water_meter_display_item),
    m_max_update_time_s(1)
{
    ui->setupUi(this);
}

Frame_WaterMeterDisplayItem::~Frame_WaterMeterDisplayItem()
{
    delete ui;
}

bool Frame_WaterMeterDisplayItem::IsEnable() const
{
    return m_water_meter_device_config.device_status == DeviceStatus::Activite;
}

QString Frame_WaterMeterDisplayItem::GetDeviceName() const
{
    return m_water_meter_device_config.device_name;
}

void Frame_WaterMeterDisplayItem::UpdateMeterVal(const QString &dev_name, const QDateTime &date_time, double val)
{
    if(dev_name != m_water_meter_device_config.device_name)
        return;
    ui->lcdNumber_measure_value->display(val);
    ui->lcdNumber_measure_value->setToolTip(QString("last update time: %1").arg(date_time.toString("hh:mm:ss")));
    ResetBackGroundColorValue();
    UpdateBackGroundColor();
}

WaterDeviceInfo Frame_WaterMeterDisplayItem::GetWaterMeterDeviceConfig() const
{
    return m_water_meter_device_config;
}

void Frame_WaterMeterDisplayItem::SetWaterMeterDeviceConfig(const WaterDeviceInfo &water_meter_device_config)
{
    m_water_meter_device_config = water_meter_device_config;
    Init();
}

void Frame_WaterMeterDisplayItem::StartUpdate()
{
    m_meter_update_max_timer.start(m_max_update_time_s * 1000);
    ResetBackGroundColorValue();
    UpdateBackGroundColor();
}

void Frame_WaterMeterDisplayItem::StopUpdate()
{
    m_meter_update_max_timer.stop();
}

void Frame_WaterMeterDisplayItem::SetMaxUpdateCheckTimeInval(int s)
{
    m_max_update_time_s = s;
}

void Frame_WaterMeterDisplayItem::Init()
{
    connect(&m_meter_update_max_timer, &QTimer::timeout, [&](){
        IncBackGroundColorBadLevel();
        UpdateBackGroundColor();
    });
    ui->label_device_name->setText(m_water_meter_device_config.device_name);
    setToolTip(QString("%1\n%2\n%3\n%4\n%5\n")
               .arg(tr("Device Name: %1").arg(m_water_meter_device_config.device_name))
               .arg(tr("IP: %1").arg(m_water_meter_device_config.server_ip))
               .arg(tr("Port: %1").arg(m_water_meter_device_config.server_port))
               .arg(tr("Value: %1 M^3").arg(ui->lcdNumber_measure_value->value()))
               .arg(tr("%1").arg(ui->lcdNumber_measure_value->toolTip())));
}

void Frame_WaterMeterDisplayItem::ResetBackGroundColorValue()
{
    m_back_ground_color.setGreen(255);
    m_back_ground_color.setRed(0);
    m_back_ground_color.setBlue(0);
}

void Frame_WaterMeterDisplayItem::IncBackGroundColorBadLevel()
{
    m_back_ground_color.setGreen(qMax(m_back_ground_color.green() - 1, 0));
    m_back_ground_color.setRed(qMin(m_back_ground_color.red() + 1, 255));

}

void Frame_WaterMeterDisplayItem::UpdateBackGroundColor()
{
    QString back_ground_color_style_sheet_str = QString("background: #%1%2%3")
                                                .arg(m_back_ground_color.red(), 2, 16, QChar('0'))
                                                .arg(m_back_ground_color.green(), 2, 16, QChar('0'))
                                                .arg(m_back_ground_color.blue(), 2, 16, QChar('0'));
//    qDebug() << __FUNCTION__ << back_ground_color_style_sheet_str;
    ui->lcdNumber_measure_value->setStyleSheet(back_ground_color_style_sheet_str);

}
