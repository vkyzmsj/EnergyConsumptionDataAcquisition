#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "src/base/config.h"
#include "src/gui/frame_water_meter_display_item.h"
#include "src/gui/frame_water_meter_device_config_edit.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void InitWaterMeterDevice();

private slots:
    void on_actionHelpTools_triggered();

    void on_actionAddWaterDevice_triggered();

    void on_actionUpdateDeviceStatus_triggered();

private:
    Ui::MainWindow *ui;
    QMap<QString, Frame_WaterMeterDisplayItem> m_water_meter_device_display_items;
};

#endif // MAINWINDOW_H
