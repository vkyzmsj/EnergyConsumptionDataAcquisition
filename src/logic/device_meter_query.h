#pragma once
#include <QTimer>
#include <QObject>
#include <QUdpSocket>

struct ServerInfo
{
    QString ip;
    quint16 port;
};

class DeviceMeterQuery : public QObject
{
    Q_OBJECT
public:
    explicit DeviceMeterQuery(QObject *parent = nullptr);
    void SetQueryTimeInval(int s);
    void Start(); // 初始化
    void Stop(); // 销毁
    void AddDeviceInfo(const QString &device_name, const ServerInfo &info, const QByteArray &query_cmd);

    virtual void SocketRead();

signals:
    void MeterReady(QString, double value);

public slots:

private slots:
    void QueryMeter();

private:
    void InitSocket();
    void ReleaseSocket();

protected:
    QTimer m_query_timer;
    QMap<QString, QByteArray> m_query_cmd_map;         // 存储查询字符串
    QMap<QString, ServerInfo> m_server_info_map;    // 存储服务器信息
    QMap<QString, QUdpSocket*> m_udp_socket_map;    // 存储通道句柄
};

