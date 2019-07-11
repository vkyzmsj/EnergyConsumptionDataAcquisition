#ifndef HELPER_H
#define HELPER_H
#include <QString>
#include <QList>
#include <QList>
#include <QFile>
#include <QObject>
#include <QDir>
#include <QThread>
#include <QMap>
//#include "helper_global.h"
#define HELPERSHARED_EXPORT
class HELPERSHARED_EXPORT Helper
{

public:

    static int Cvt24BitAcqDataToInt(const char *data);
    static void CvtAcqDataToMem(const char *src, int src_len, int *dest);
    static void CvtAcqDataToMem(const char *src, int src_len, float *dest);
    static int Cvt4BToInt(const char *data);
    static float Cvt4BToFloat(const char *data);
    static void CvtNumTo4B(int num, char *data);
    static void CvtNumTo4B(float num, char *data);
    static void CvtNumToMem(const int *num, int len, char *dest);
    static void CvtNumToMem(const float *num, int len, char *dest);
    static void CvtNumTo3BMem(const int *num, int point_num, const char *dest);
    static void CvtNumTo3BMem(const float *num, int point_num, const char *dest);

    template <class T>
    static QString QList2String(const QList<T> &data)
    {
        QString str;
        for (auto it = data.constBegin(); it!=data.constEnd(); it++)
        {
            str += QString("%1 ").arg(*it);
        }
        return str;
    }

    static QFile* OpenFile(const QString &file_path);
    static bool WriteFile(QFile &file, const char *data, qint64 len);
    static bool WriteFile(QFile &file, const QByteArray &data);
    static bool WriteFile(QFile &file, const QString &str);
    static bool WriteFile(QFile &file, const std::string &str);

    static bool ReadFile(QFile &file, char *data, qint64 len);
    static bool ReadFile(QFile &file, QByteArray &data, int len);
    static bool ReadFile(QFile &file, QString &str, int len);

    template <class T>
    static void DelObj(T *&obj){
        delete obj;
        obj = nullptr;
    };

    static bool CreateDir(const QString &dir_path);
    static bool ReCreateDir(const QString &dir_path);
    static bool DelDir(const QString &dir_path);

    // 正则表达式
    static QRegExp IpRegExp();

    static qintptr GetObjPtrValue(const void *obj);

    static void MoveObjectToThreadListAndStart(QObject *object, QMap<qintptr, QThread *> &thread_list);
    static void MoveOutObjectFromThreadList(const QObject *object, QMap<qintptr, QThread *> &thread_list);

    template <typename T>
    static void Delete(T *&t)
    {
        delete t;
        t = nullptr;
    }

    static inline qintptr GetAddr(void *val);

private:
    Helper();
};

#endif // HELPER_H
