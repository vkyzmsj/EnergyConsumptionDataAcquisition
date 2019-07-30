#include "frame_power_meter_display.h"
#include "ui_frame_power_meter_display.h"

Frame_PowerMeterDisplay::Frame_PowerMeterDisplay(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Frame_power_meter_display)
{
    ui->setupUi(this);
    m_config = Config::Instance();
    m_db_manager = DataBaseManager::Instance();
    m_tree_top_item = new QTreeWidgetItem(QStringList() << tr("Power Device"));
    ui->treeWidget->clear();
    ui->treeWidget->addTopLevelItem(m_tree_top_item);
    ui->treeWidget->header()->hide();
    setWindowTitle(tr("Water Meter Display"));
    ui->treeWidget->setStyleSheet(GetStyleSheet());
    ui->treeWidget->expandAll();
}

Frame_PowerMeterDisplay::~Frame_PowerMeterDisplay()
{
    delete ui;
}

void Frame_PowerMeterDisplay::SyncView()
{
    QList<WaterDeviceInfo> device_info_list = m_db_manager->QueryWaterDeviceInfoList();

    for(int i = 0 ; i< device_info_list.size(); i++)
    {
        device_info_list[i].device_name.replace('k', 'P');
    }
    device_info_list = device_info_list.mid(4);

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
            item->SetUnitName(tr("kWh"));
            m_tree_node_map[it.device_name] = new QTreeWidgetItem();
            m_tree_top_item->addChild(m_tree_node_map[it.device_name]);
            ui->treeWidget->setItemWidget(m_tree_node_map[it.device_name], 0, item);
        }
    }
}

QString Frame_PowerMeterDisplay::GetStyleSheet() const
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
