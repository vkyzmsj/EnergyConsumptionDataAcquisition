#include "helper.h"
#include <QDateTime>

int Helper::Cvt24BitAcqDataToInt(const char *data)
{
    uchar *data_ = (uchar *)data;
    return ((*(data_)&0x80)==0x00) ?
        (0x00000000|(*(data_+0)<<16)|(*(data_+1)<<8)|(*(data_+2)))
              : ((0xFF000000|(*(data_+0)<<16)|(*(data_+1)<<8)|(*(data_+2))));
}

void Helper::CvtAcqDataToMem(const char *src, int src_len, int *dest)
{
    for(int i = 0; i < src_len/3; i++)
    {
        dest[i] = Cvt24BitAcqDataToInt(src + i * 3);
    }
}

void Helper::CvtAcqDataToMem(const char *src, int src_len, float *dest)
{
    for(int i = 0; i < src_len/3; i++)
    {
        dest[i] = static_cast<float>(Cvt24BitAcqDataToInt(src + i * 3));
    }
}

int Helper::Cvt4BToInt(const char *data)
{
    uchar *data_ = (uchar *)data;
    return (data_[0]<<24)|(data_[1]<<16)|(data_[2]<<8)|data_[3];
}

float Helper::Cvt4BToFloat(const char *data)
{
    return static_cast<float>(Cvt4BToInt(data));

}

void Helper::CvtNumTo4B(int num, char *data)
{
    data[0] = (num >> 24) & 0xff;
    data[1] = (num >> 16) & 0xff;
    data[2] = (num >> 8 ) & 0xff;
    data[3] = (num >> 0 ) & 0xff;
}

void Helper::CvtNumTo4B(float num, char *data)
{
    CvtNumTo4B(static_cast<int>(num), data);
}

void Helper::CvtNumToMem(const int *num, int len, char *dest)
{
    for(int i = 0; i < len; i++)
    {
        CvtNumTo4B(num[i], dest);
        dest += 4;
    }
}

void Helper::CvtNumToMem(const float *num, int len, char *dest)
{
    for(int i = 0; i < len; i++)
    {
        CvtNumTo4B(num[i], dest);
        dest += 4;
    }
}

void Helper::CvtNumTo3BMem(const int *num, int point_num, const char *dest)
{
    auto unit = [](int n, char *p){
        p[0] = n >> 16;
        p[1] = n >> 8;
        p[2] = n;
    };

    char *base = const_cast<char *>(dest);
    for(int i = 0; i < point_num; i++)
    {
        unit(num[i], base);
        base += 3;
    }
}

void Helper::CvtNumTo3BMem(const float *num, int point_num, const char *dest)
{
    auto unit = [](int n, char *p){
        p[0] = n >> 16;
        p[1] = n >> 8;
        p[2] = n;
    };

    char *base = const_cast<char *>(dest);
    for(int i = 0; i < point_num; i++)
    {
        unit((int)num[i], base);
        base += 3;
    }
}

QFile *Helper::OpenFile(const QString &file_path)
{
    QFile *file = new QFile(file_path);
    if (file->open(QIODevice::WriteOnly|QIODevice::Text))
    {
        return file;
    } else
    {
        delete file;
        return nullptr;
    }
}

bool Helper::WriteFile(QFile &file, const char *data, qint64 len)
{
    if(!file.isOpen())
    {
        if(!file.open(QIODevice::WriteOnly | QIODevice::Unbuffered))
        {
            return false;
        }
    }
    if(!file.isOpen())
        return false;

    qint64 left_len = len;
    qint64 ret = 0;
    while(left_len > 0)
    {
        ret = file.write(data + len - left_len, left_len);
        if(ret < 0)
          break;
        left_len -= ret;
    }
    return left_len == 0;
}

bool Helper::WriteFile(QFile &file, const QByteArray &data)
{
    return WriteFile(file, data.data(), data.size());
}

bool Helper::WriteFile(QFile &file, const QString &str)
{
//    string std_str = str.toStdString();
    QByteArray data(str.toUtf8());
    return WriteFile(file, data);
}

bool Helper::WriteFile(QFile &file, const std::string &str)
{
    return WriteFile(file, str.c_str(), str.size());
}

bool Helper::ReadFile(QFile &file, char *data, qint64 len)
{
    int left_len = len;
    int ret = 0;
    if(!file.isOpen())
    {
        file.open(QIODevice::ReadOnly | QIODevice::Unbuffered);
    }
    if(file.isOpen())
    {
        while((left_len > 0) && (!file.atEnd()))
        {
            ret = file.read(data, left_len);
            if(ret < 0)
                break;
            left_len -= ret;
        }
    }
    return left_len == 0;
}

bool Helper::ReadFile(QFile &file, QByteArray &data, int len)
{
    data.resize(len);
    return ReadFile(file, data.data(), len);
}

bool Helper::ReadFile(QFile &file, QString &str, int len)
{
    QScopedArrayPointer<char> file_data_sp(new char[len+1]{0});
    if(file_data_sp.isNull())
        return false;
    if(ReadFile(file, file_data_sp.data(), len))
    {
        *(file_data_sp.data() + len) = '\0';
        str = QString(file_data_sp.data());
        return true;
    }
    return false;
}

bool Helper::CreateDir(const QString &dir_path)
{
    QDir dir(dir_path);
    if (dir.exists())
    {
        return true;
    } else
    {
        return dir.mkpath(".");
    }
}

bool Helper::ReCreateDir(const QString &dir_path)
{
    if (DelDir(dir_path))
    {
        return CreateDir(dir_path);
    }
    return false;
}

bool Helper::DelDir(const QString &dir_path)
{
    QDir dir(dir_path);
    if (!dir.exists())
    {
        return true;
    }
    dir.setFilter(QDir::AllEntries|QDir::NoDotAndDotDot);
    QFileInfoList fileList = dir.entryInfoList();
    foreach(QFileInfo file, fileList)
    {
        if (file.isFile())
        {
            file.dir().remove(file.fileName());
        } else
        {
            DelDir(file.absoluteFilePath());
        }
    }
    return dir.rmpath(dir.absolutePath());
}

QRegExp Helper::IpRegExp()
{
    return QRegExp("^((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)$");
}

qintptr Helper::GetObjPtrValue(const void *obj)
{
    return qintptr(obj);
}

void Helper::MoveObjectToThreadListAndStart(QObject *object, QMap<qintptr, QThread *> &thread_list)
{
    qintptr key = GetObjPtrValue(object);
    if (thread_list.contains(key))
    {
        if (!thread_list[key]->isRunning())
        {
            thread_list[key]->start();
        }
    } else
    {
        thread_list[key] = new QThread;
        object->moveToThread(thread_list[key]);
        thread_list[key]->start();
    }
}

void Helper::MoveOutObjectFromThreadList(const QObject *object, QMap<qintptr, QThread *> &thread_list)
{
    qintptr key = GetObjPtrValue(object);
    if (thread_list.contains(key))
    {
        thread_list[key]->quit();
        thread_list[key]->deleteLater();
        thread_list.remove(key);
    }
}

qintptr Helper::GetAddr(void *val)
{
    return reinterpret_cast<qintptr>(val);
}
