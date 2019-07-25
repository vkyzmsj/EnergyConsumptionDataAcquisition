#include "device_meter_query.h"

DeviceMeterQuery::DeviceMeterQuery(QObject *parent) : QObject(parent)
{

}

void DeviceMeterQuery::SetQueryTimeInval(int s)
{
    m_query_timer.setInterval(s * 1000);
}

void DeviceMeterQuery::Start()
{
    m_query_timer.start();
}

void DeviceMeterQuery::Stop()
{
    m_query_timer.stop();
}

void DeviceMeterQuery::AddDeviceInfo(const QString &device_name, const ServerInfo &info, const QByteArray &query_cmd)
{
    m_server_info_map[device_name] = info;
    m_query_cmd_map[device_name] = query_cmd;
}

void DeviceMeterQuery::QueryMeter()
{
    for(auto it = m_udp_socket_map.begin(); it !=m_udp_socket_map.end(); ++it)
    {
        if(!m_query_cmd_map.value(it.key(), "").isEmpty())
        {
            if((*it)->isOpen())
                (*it)->write(m_query_cmd_map.value(it.key()));
        }
    }
}

void DeviceMeterQuery::InitSocket()
{
    m_udp_socket_map.clear();
    for(auto it = m_server_info_map.begin(); it != m_server_info_map.end(); ++it)
    {
        QUdpSocket *socket = new QUdpSocket();
        socket->connectToHost((*it).ip, (*it).port);
        m_udp_socket_map[it.key()] = socket;
        connect(socket, &QUdpSocket::readyRead, [&](){
            SocketRead();
        });
    }
}

void DeviceMeterQuery::ReleaseSocket()
{
    for(auto it = m_udp_socket_map.begin(); it != m_udp_socket_map.end(); ++it)
    {
        (*it)->deleteLater();
    }
    m_udp_socket_map.clear();
}
