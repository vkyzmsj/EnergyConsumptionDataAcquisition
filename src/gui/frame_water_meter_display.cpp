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
    QList<WaterDeviceInfo> device_info_list = m_db_manager->QueryWaterDeviceInfoList();
    auto HasDevice = [](const QList<WaterDeviceInfo> &list, const QString &name){
        for(auto it : list)
        {
            if(it.device_name == name)
            {
                return true;
            }
        }
        return false;
    };
    for(auto it = m_view_map.begin(); it != m_view_map.end();)
    {
        if(!HasDevice(device_info_list, it.key()))
        {
            ui->treeWidget_wate_devices->topLevelItem(0)->takeChild(ui->treeWidget_wate_devices->topLevelItem(0)->indexOfChild(*it));
            m_view_map.erase(it);
        }else{
            it++;
        }
    }
    // add view
    for(auto it : device_info_list)
    {
        if(!m_view_map.contains(it.device_name))
        {
            Frame_WaterMeterDisplayItem *item = new Frame_WaterMeterDisplayItem;
            item->SetWaterMeterDeviceConfig(it);
            m_view_map[it.device_name] = item;
            if(m_start_monitor)
            {
                item->StartUpdate();
            }
            ui->treeWidget_wate_devices->topLevelItem(0)->addChild();
        }
    }
}
