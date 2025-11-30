#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <DOW.h>
#include <QVBoxLayout>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTAbleWidget>
#include <QButtonGroup>
#include <QHeaderView>

QT_BEGIN_NAMESPACE
namespace Ui {
class Dialog;
}
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();
    DOW sun;
    DOW mon;
    DOW tue;
    DOW wed;
    DOW thu;
    DOW fri;
    DOW sat;
    std::string DOW;

private slots:

    void on_AddAlarm_Button_clicked();

    void on_arduino_button_clicked();

    void on_changetime_button_clicked();

    void on_repeatalarm_button_clicked();

    void on_removealarm_button_clicked();

    void on_setforall_button_clicked();

    void on_setpills_button_clicked();

    void on_checkall_button_clicked();

    void on_uncheckall_button_clicked();

    void on_clear_button_clicked();

    int dayIndexFromName(const QString &dayName);
    class::DOW* dayModelFromName(const QString &dayName);
    void addAlarmFromArduino(const QString &dayName,
                             const QString &timeHHMMSS,
                             const QVector<int> &pills);
    void loadAlarmsFromArduino();
    void on_print_button_clicked();

private:
    Ui::Dialog *ui;
    QSerialPort *arduino;
    static const quint16 arduino_uno_vendorID = 9025;
    static const quint16 arduino_uno_productID = 66;
    QString arduino_port_name;
    bool arduino_is_available;
};

#endif // DIALOG_H
