#include "frame_tools.h"
#include "ui_frame_tools.h"

Frame_Tools::Frame_Tools(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Frame_Tools)
{
    ui->setupUi(this);
    setWindowTitle(tr("Tools"));
}

Frame_Tools::~Frame_Tools()
{
    delete ui;
}

void Frame_Tools::on_pushButton_gen_cs_code_clicked()
{
    QByteArray raw_data = QByteArray::fromHex(ui->plainTextEdit_cs_source_code->toPlainText().trimmed().toStdString().c_str());
    if(raw_data.isEmpty())
        return;
    char cs_code = 0x00;
    for(int i = 0; i < raw_data.size(); i++)
    {
        cs_code += raw_data.at(i);
    }
    ui->lineEdit_cs_code->setText(QString::number((uchar)(cs_code), 16));
    ui->pushButton_gen_cs_code->setToolTip(raw_data.toHex(' ').toUpper());
}
