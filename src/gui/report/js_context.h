#ifndef JS_CONTEXT_H
#define JS_CONTEXT_H

#include <QObject>
#include <QWebEnginePage>

class JsContext : public QObject
{
    Q_OBJECT
public:
    explicit JsContext(QObject *parent = nullptr);


signals:
    void RecvdMsg(const QString& msg);

public:
    // 向页面发送消息
    void SendMsg(QWebEnginePage* page, const QString& msg);

public slots:
    // 接收到页面发送来的消息
    void OnMsg(const QString& msg);
};

#endif // JS_CONTEXT_H
