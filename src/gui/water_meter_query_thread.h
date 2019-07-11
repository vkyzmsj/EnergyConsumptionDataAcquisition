#ifndef WATER_METER_QUERY_THREAD_H
#define WATER_METER_QUERY_THREAD_H

#include <QObject>
#include <QThread>
#include <QUdpSocket>
#include "../base/config.h"
#include <QDateTime>

class WaterMeterQueryThread : public QThread
{
    Q_OBJECT
public:
    explicit WaterMeterQueryThread(QObject *parent = nullptr);
    void run();
    void SetWaterMeterConfig(const WaterMeterDeviceConfig &water_meter_config);
    void StopReq();

private:
//    void CheckDataInvaild();
    QByteArray GenReqCmd() const;
    bool WriteData(QUdpSocket *socket, const QByteArray &data);
    void RecvData(QUdpSocket *socket, QByteArray &data, int recv_max_times_s);
    void AnalysisData(const QByteArray &data);

signals:
    void NetConnectStatusChanged(QAbstractSocket::SocketState state);
    void NetConnectErrorOccur(QString msg);
    void MeterReady(QDateTime date_time, double val);

public slots:


private:
    WaterMeterDeviceConfig m_water_meter_config;
    bool m_stop_thread;
};

#endif // WATER_METER_QUERY_THREAD_H
