#include "frame_water_meter_display.h"
#include "ui_frame_water_meter_display.h"

Frame_WaterMeterDisplay::Frame_WaterMeterDisplay(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Frame_water_meter_display)
{
    ui->setupUi(this);
    m_config = Config::Instance();
    m_db_manager = DataBaseManager::Instance();
    m_tree_top_item = new QTreeWidgetItem(QStringList() << tr("Water Device"));
    ui->treeWidget_wate_devices->clear();
    ui->treeWidget_wate_devices->addTopLevelItem(m_tree_top_item);
    ui->treeWidget_wate_devices->header()->hide();
    setWindowTitle(tr("Water Meter Display"));
    ui->treeWidget_wate_devices->setStyleSheet(GetStyleSheet());
    ui->treeWidget_wate_devices->expandAll();
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

    // 删除无效的视图
    for(auto it = m_view_map.begin(); it != m_view_map.end();)
    {
        if(!HasDevice(device_info_list, it.key()))
        {
            m_tree_top_item->removeChild(m_tree_node_map[it.key()]);
            delete m_tree_node_map[it.key()];
            m_tree_node_map.remove(it.key());
            m_view_map.erase(it);
        }else{
            it++;
        }
    }
    // 增加新的视图
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
            m_tree_node_map[it.device_name] = new QTreeWidgetItem();
            m_tree_top_item->addChild(m_tree_node_map[it.device_name]);
            ui->treeWidget_wate_devices->setItemWidget(m_tree_node_map[it.device_name], 0, item);
        }
    }
}

void Frame_WaterMeterDisplay::UpdateWaterMeasureValue(const QString &name, double value)
{
    for(auto it = m_view_map.begin(); it != m_view_map.end(); ++it)
    {
        if(name == it.key())
        {
            (*it)->UpdateMeterVal(name, QDateTime::currentDateTime(), value);
            return;
        }
    }
}

QString Frame_WaterMeterDisplay::GetStyleSheet() const
{
QString style =  "    QTreeView::branch:has-siblings:!adjoins-item {\
        border-image: url(:/source/vline.png) 0;\
    }\
\
    QTreeView::branch:has-siblings:adjoins-item {\
        border-image: url(:/source/branch-more.png) 4;\
    width: 40px;\
    heigh: 40px;\
    }\
\
    QTreeView::branch:!has-children:!has-siblings:adjoins-item {\
        border-image: url(:/source/branch-end.png) 0;\
    }\
\
    QTreeView::branch:has-children:!has-siblings:closed,\
    QTreeView::branch:closed:has-children:has-siblings {\
            border-image: none;\
            image: url(:/source/branch-closed.png);\
    }\
\
    QTreeView::branch:open:has-children:!has-siblings,\
    QTreeView::branch:open:has-children:has-siblings  {\
            border-image: none;\
            image: url(:/source/branch-open.png);\
    }";
    return style;
}
