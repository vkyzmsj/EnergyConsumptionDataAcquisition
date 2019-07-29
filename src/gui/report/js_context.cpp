#include "js_context.h"
#include <QDebug>

JsContext::JsContext(QObject *parent) : QObject(parent)
{
}

void JsContext::SendMsg(QWebEnginePage *page, const QString &msg)
{
    page->runJavaScript(QString("recvMessage('%1');").arg(msg));
}

void JsContext::OnMsg(const QString &msg)
{
    qDebug() <<__FUNCTION__ <<msg;
    emit RecvdMsg(msg);
}
