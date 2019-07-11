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
}

Frame_WaterMeterDeviceConfigEdit::~Frame_WaterMeterDeviceConfigEdit()
{
    delete ui;
}

void Frame_WaterMeterDeviceConfigEdit::on_pushButton_yes_clicked()
{
    UpdateDeviceConfig();
    Config *cof = Config::Instance();
    if(cof->WaterDeviceExists(m_water_meter_device_config.GetDevName()))
    {
        QMessageBox::warning(this, tr("Add Water Device Config"), tr("device name has exist"), QMessageBox::Yes);
        return;
    }
    m_exit_with_yes = true;
    hide();
    emit Finished();
}

WaterMeterDeviceConfig Frame_WaterMeterDeviceConfigEdit::GetWaterMeterDeviceConfig() const
{
    return m_water_meter_device_config;
}

void Frame_WaterMeterDeviceConfigEdit::SetWaterMeterDeviceConfig(const WaterMeterDeviceConfig &water_meter_device_config)
{
    m_water_meter_device_config = water_meter_device_config;
    Init();
}

bool Frame_WaterMeterDeviceConfigEdit::ExitWithYes()
{
    return m_exit_with_yes;
}

void Frame_WaterMeterDeviceConfigEdit::Init()
{
    ui->lineEdit_dev_name->setText(m_water_meter_device_config.GetDevName());
    ui->lineEdit_ip_addr->setText(m_water_meter_device_config.GetServerIpAddr());
    ui->lineEdit_server_port->setText(QString::number(m_water_meter_device_config.GetServerPort()));
    ui->lineEdit_query_cycle_s->setText(QString::number(m_water_meter_device_config.GetDegreeQueryCycleS()));
    ui->lineEdit_save_max_time_s->setText(QString::number(m_water_meter_device_config.GetDegreeDataSaveMaxTimeS()));
    ui->lineEdit_data_save_path->setText(m_water_meter_device_config.GetSaveDir());
    ui->lineEdit_meter_type_code->setText(m_water_meter_device_config.GetMeterTypeCode());
    ui->lineEdit_manufacture_code->setText(m_water_meter_device_config.GetManufacturerCode());
    ui->lineEdit_device_addr->setText(m_water_meter_device_config.GetDevAddrCode());
    ui->checkBox_device_is_enable->setChecked(m_water_meter_device_config.IsEnable());
}

void Frame_WaterMeterDeviceConfigEdit::UpdateDeviceConfig()
{
    m_water_meter_device_config.SetDevName(ui->lineEdit_dev_name->text().trimmed());
    m_water_meter_device_config.SetDevType(MeterDeviceType::WaterMeter);
    m_water_meter_device_config.SetServerIpAddr(ui->lineEdit_ip_addr->text().trimmed());
    m_water_meter_device_config.SetServerPort(ui->lineEdit_server_port->text().trimmed().toInt());
    m_water_meter_device_config.SetDegreeQueryCycleS(ui->lineEdit_query_cycle_s->text().toInt());
    m_water_meter_device_config.SetDegreeDataSaveMaxTimeS(ui->lineEdit_save_max_time_s->text().toInt());
    m_water_meter_device_config.SetSaveDir(ui->lineEdit_data_save_path->text().trimmed());
    m_water_meter_device_config.SetMeterTypeCode(ui->lineEdit_meter_type_code->text().trimmed());
    m_water_meter_device_config.SetManufacturerCode(ui->lineEdit_manufacture_code->text().trimmed());
    m_water_meter_device_config.SetDevAddrCode(ui->lineEdit_device_addr->text().trimmed());
    m_water_meter_device_config.setEnable(ui->checkBox_device_is_enable->isChecked());
}

void Frame_WaterMeterDeviceConfigEdit::on_pushButton_cancle_clicked()
{
    hide();
    emit Finished();
}

void Frame_WaterMeterDeviceConfigEdit::on_toolButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("chose data save directory path"),
                                                    m_water_meter_device_config.GetSaveDir(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if(!dir.isEmpty())
    {
        ui->lineEdit_data_save_path->setText(dir);
    }
}
