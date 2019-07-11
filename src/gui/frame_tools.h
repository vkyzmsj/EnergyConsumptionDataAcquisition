#ifndef FRAME_TOOLS_H
#define FRAME_TOOLS_H

#include <QFrame>

namespace Ui {
class Frame_Tools;
}

class Frame_Tools : public QFrame
{
    Q_OBJECT

public:
    explicit Frame_Tools(QWidget *parent = nullptr);
    ~Frame_Tools();

private slots:
    void on_pushButton_gen_cs_code_clicked();

private:
    Ui::Frame_Tools *ui;
};

#endif // FRAME_TOOLS_H
