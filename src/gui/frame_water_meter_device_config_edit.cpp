#include <QFileDialog>

#include "frame_water_meter_device_config_edit.h"
#include "ui_frame_water_meter_device_config_edit.h"
#include <QMessageBox>


Frame_WaterMeterDeviceConfigEdit::Frame_WaterMeterDeviceConfigEdit(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Frame_water_meter_device_config_edit),
    m_exit_with_yes(false)
{
    ui->setupUi(this);
    QList<DeviceStatus> device_status_list;
    device_status_list << DeviceStatus::Deleted << DeviceStatus::Activite << DeviceStatus::NoneActivite;
    for(auto it : device_status_list)
    {
        ui->comboBox_device_status->addItem(ToString(it), static_cast<int>(it));
    }
}

Frame_WaterMeterDeviceConfigEdit::~Frame_WaterMeterDeviceConfigEdit()
{
    delete ui;
}

void Frame_WaterMeterDeviceConfigEdit::on_pushButton_yes_clicked()
{
    UpdateDeviceConfig();
    DataBaseManager *db_manager = DataBaseManager::Instance();
    if(!db_manager->InsertWaterDeviceInfo(m_device_info))
    {
        QMessageBox::warning(this, tr("Add Water Device Config"), tr("device name has exist"), QMessageBox::Yes);
        return;
    }
    m_exit_with_yes = true;
    hide();
    emit Finished();
}

WaterDeviceInfo Frame_WaterMeterDeviceConfigEdit::GetDeviceInfo() const
{
    return m_device_info;
}

void Frame_WaterMeterDeviceConfigEdit::SetDeviceInfo(const WaterDeviceInfo &water_meter_device_config)
{
    m_device_info = water_meter_device_config;
    Init();
}

bool Frame_WaterMeterDeviceConfigEdit::ExitWithYes()
{
    return m_exit_with_yes;
}

void Frame_WaterMeterDeviceConfigEdit::Init()
{
    ui->lineEdit_dev_name->setText(m_device_info.device_name);
    ui->lineEdit_ip_addr->setText(m_device_info.server_ip);
    ui->lineEdit_server_port->setText(QString::number(m_device_info.server_port));
    ui->lineEdit_meter_type_code->setText(m_device_info.meter_type.toHex().toUpper());
    ui->lineEdit_manufacture_code->setText(m_device_info.manu_code.toHex().toUpper());
    ui->lineEdit_device_addr->setText(m_device_info.device_address.toHex().toUpper());
    ui->comboBox_device_status->setCurrentIndex(ui->comboBox_device_status->findData(static_cast<int>(m_device_info.device_status)));
}

void Frame_WaterMeterDeviceConfigEdit::UpdateDeviceConfig()
{
    m_device_info.device_name = (ui->lineEdit_dev_name->text().trimmed());
    m_device_info.server_ip = (ui->lineEdit_ip_addr->text().trimmed());
    m_device_info.server_port = static_cast<quint16>(ui->lineEdit_server_port->text().trimmed().toInt());
    m_device_info.meter_type = QByteArray::fromHex(ui->lineEdit_meter_type_code->text().trimmed().toStdString().c_str());
    m_device_info.manu_code = QByteArray::fromHex(ui->lineEdit_manufacture_code->text().trimmed().toStdString().c_str());
    m_device_info.device_address = QByteArray::fromHex(ui->lineEdit_device_addr->text().trimmed().toStdString().c_str());
    m_device_info.device_status = static_cast<DeviceStatus>(ui->comboBox_device_status->currentData().toInt());
}

void Frame_WaterMeterDeviceConfigEdit::on_pushButton_cancle_clicked()
{
    hide();
    emit Finished();
}
