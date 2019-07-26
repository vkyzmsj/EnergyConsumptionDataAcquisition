#include "frame_water_meter_display.h"
#include "ui_frame_water_meter_display.h"

Frame_WaterMeterDisplay::Frame_WaterMeterDisplay(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Frame_water_meter_display)
{
    ui->setupUi(this);
    m_config = Config::Instance();
    m_db_manager = DataBaseManager::Instance();
}

Frame_WaterMeterDisplay::~Frame_WaterMeterDisplay()
{
    delete ui;
}

void Frame_WaterMeterDisplay::SyncView()
{
    // remove view
    QList<WaterDeviceInfo> device_info = m_db_manager->QueryWaterDeviceInfoList();


    for(auto it : device_info)
    {
        if(m_view_map.contains(it.device_name))
    }
}
