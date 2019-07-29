#include "water_meter_query_thread.h"
#include <QTime>
#include <QDebug>

WaterMeterQueryThread::WaterMeterQueryThread(QObject *parent) : QThread(parent)
{

}

void WaterMeterQueryThread::run()
{
    m_stop_thread = false;
    QTime req_cycle_timer, file_save_max_timer;


    QByteArray req_cmd = GenReqCmd();
    QByteArray recv_data;
    {
        //test
        recv_data = QByteArray::fromHex("FE FE 68 10 26 13 06 07 00 33 78 81 16 1F 90 04 00 68 00 00 2C 00 00 00 00 2C 00 00 00 00 00 00 00 00 00 73 16");
        AnalysisData(recv_data);
    }
    return;




    QUdpSocket udp_socket;
    emit NetConnectStatusChanged(QAbstractSocket::SocketState::ConnectingState);
    udp_socket.connectToHost(m_water_meter_config.server_ip, m_water_meter_config.server_port);
    if(udp_socket.waitForConnected())
    {
        emit NetConnectStatusChanged(QAbstractSocket::SocketState::ConnectedState);

    }else{
        emit NetConnectStatusChanged(QAbstractSocket::SocketState::UnconnectedState);
        emit NetConnectErrorOccur(udp_socket.errorString());
        qWarning() << __FUNCTION__ <<": connect faield, exit thread";
        return;
    }

    while(!m_stop_thread)
    {
        if(m_stop_thread)
            break;
        req_cycle_timer.start();
        udp_socket.readAll();
        if(!WriteData(&udp_socket, req_cmd))
        {
            emit NetConnectErrorOccur(udp_socket.errorString() + "write data failed");
            udp_socket.disconnectFromHost();
            emit NetConnectStatusChanged(QAbstractSocket::SocketState::UnconnectedState);
            break;
        }
        recv_data.clear();
        RecvData(&udp_socket, recv_data, 5);

        if(recv_data.size() == 0)
            continue;

        AnalysisData(recv_data);


//        while(req_cycle_timer.elapsed() < m_water_meter_config.GetDegreeQueryCycleS() * 1000)
//        {
//            QThread::msleep(1000);
//            if(m_stop_thread)
//                break;
//        }
    }
    udp_socket.abort();
    qDebug() <<__FUNCTION__<< ": exit thread";
}

void WaterMeterQueryThread::SetWaterMeterConfig(const WaterDeviceInfo &water_meter_config)
{
    m_water_meter_config = water_meter_config;
}

void WaterMeterQueryThread::StopReq()
{
    m_stop_thread = true;
}

QByteArray WaterMeterQueryThread::GenReqCmd() const
{
    //fefefefe68 10 0000000000 3378 01 031f9004 cs 16
    // 0  1  2  3  4   5  6  7  8  9  10   11 12  13 14 15 16 17 18   19
    // FE FE FE FE 68 10 *26 13 06 07 00* *33 78* 01 03 1F 90 04 *20* 16
    static QString p1("FE FE FE FE 68 10"), p2("01 03 1F 90 04"), p_end("16");
    QByteArray cmd_data = QByteArray::fromHex(QString("%1 %2 %3 %4 %5 %6")
                                              .arg(p1)
                                              .arg(m_water_meter_config.device_address.toHex(' ').toStdString().c_str())
                                              .arg(m_water_meter_config.manu_code.toHex(' ').toStdString().c_str())
                                              .arg(p2)
                                              .arg("00")
                                              .arg(p_end).toStdString().c_str());
    for(int i = 4; i <= 17; i++ )
    {
        cmd_data[18] = (cmd_data[i] + cmd_data[18]);
    }
    qDebug() <<__FUNCTION__ <<": req code " << cmd_data.toHex(' ').toUpper();
    return cmd_data;
}

bool WaterMeterQueryThread::WriteData(QUdpSocket *socket, const QByteArray &data)
{
    int left_data_size = data.size();
    while(left_data_size > 0)
    {
        int ret = socket->write(data.data() + data.size() - left_data_size, left_data_size);
        if(ret < 0)
            return false;
        left_data_size -= ret;
    }
    return true;
}

void WaterMeterQueryThread::RecvData(QUdpSocket *socket, QByteArray &data, int recv_max_times_s)
{
    QTime recv_timer;
    recv_timer.start();
    while(recv_timer.elapsed() < (recv_max_times_s * 1000))
    {
        if(m_stop_thread)
            return;
        socket->waitForReadyRead(recv_max_times_s * 1000 / 50);
        if(socket->bytesAvailable() > 0)
            data += socket->readAll();
    }
}

void WaterMeterQueryThread::AnalysisData(const QByteArray &data)
{
    qDebug() <<__FUNCTION__ << ": recv data " << data.toHex(' ').toUpper();
    if(data.startsWith(QByteArray::fromHex("fe")) && (data.size() >= (35 + data.count((char)0xfe))))
    {
        QByteArray bak_data = data.mid(data.indexOf(0xfe) + 1);
        if(bak_data.size() > 17)
        {
            uchar v1_l = bak_data[15];
            uchar v2_h = bak_data[16];
            QString val_str = QString("%1%2").arg(v2_h, 2, 16, QChar('0'))
                          .arg(v1_l, 2, 16, QChar('0'));

            bool ok = false;
            double val = val_str.toDouble(&ok);
            if(ok)
            {
                if(val >= 0)
                {
                    emit MeterReady(QDateTime::currentDateTime() ,val_str.toDouble());
                }
            }else{
                qDebug() << __FUNCTION__ <<": cvt data failed";
            }
        }
    }
}
