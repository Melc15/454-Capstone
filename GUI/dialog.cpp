#include "dialog.h"
#include "ui_dialog.h"

QTableWidget* createCmpTable(QWidget *parent = nullptr)
{
    auto *table = new QTableWidget(12, 1, parent);  // 12 rows, 1 column
    table->setHorizontalHeaderLabels(QStringList() << "Pills");  // column name

    QStringList rowLabels;
    for (int i = 0; i < 12; ++i) {
        rowLabels << QString("CMP %1").arg(i + 1);
    }
    table->setVerticalHeaderLabels(rowLabels);

    // Optional: some table settings
    table->horizontalHeader()->setStretchLastSection(true);
    table->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    return table;
}


Dialog::Dialog(QWidget *parent)
    : QDialog(parent), sun(), mon(), tue(), wed(), thu(), fri(), sat(), DOW("Sun")
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    QButtonGroup *dayGroup = new QButtonGroup(this);
    dayGroup->setExclusive(true);
    arduino = new QSerialPort;
    arduino_is_available = false;
    arduino_port_name = "";
    qDebug() << "Number of Ports: " << QSerialPortInfo::availablePorts().length();
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()){
            qDebug() << "Vendor ID: " << serialPortInfo.vendorIdentifier();
            qDebug() << "Product ID: " << serialPortInfo.productIdentifier();
        }
    }
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()){
            if(serialPortInfo.vendorIdentifier() == arduino_uno_vendorID){
                if(serialPortInfo.productIdentifier() == arduino_uno_productID){
                    arduino_port_name = serialPortInfo.portName();
                    arduino_is_available = true;
                    qDebug() << "Port Available";
                }
            }
        }
    }

    if(arduino_is_available){
        arduino->setPortName(arduino_port_name);
        arduino->open(QSerialPort::WriteOnly);
        arduino->setBaudRate(QSerialPort::Baud115200);
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        arduino->setFlowControl(QSerialPort::NoFlowControl);
        arduino->setDataTerminalReady(false);
        arduino->setRequestToSend(false);
    }

    ui->button_sun->setCheckable(true);
    ui->button_mon->setCheckable(true);
    ui->button_tue->setCheckable(true);
    ui->button_wed->setCheckable(true);
    ui->button_thu->setCheckable(true);
    ui->button_fri->setCheckable(true);
    ui->button_sat->setCheckable(true);
    dayGroup->addButton(ui->button_sun, 0);
    dayGroup->addButton(ui->button_mon, 1);
    dayGroup->addButton(ui->button_tue, 2);
    dayGroup->addButton(ui->button_wed, 3);
    dayGroup->addButton(ui->button_thu, 4);
    dayGroup->addButton(ui->button_fri, 5);
    dayGroup->addButton(ui->button_sat, 6);


    connect(dayGroup, QOverload<int>::of(&QButtonGroup::idClicked),
            this, [this](int id){
                ui->stack_week->setCurrentIndex(id);
                switch (id) {
                case 0: DOW = "Sun"; break;
                case 1: DOW = "Mon"; break;
                case 2: DOW = "Tue"; break;
                case 3: DOW = "Wed"; break;
                case 4: DOW = "Thu"; break;
                case 5: DOW = "Fri"; break;
                case 6: DOW = "Sat"; break;
                }
            });

    ui->button_sun->setChecked(true);
    ui->stack_week->setCurrentIndex(0);
}
Dialog::~Dialog()
{
    delete ui;
}




void Dialog::on_AddAlarm_Button_clicked()
{
    std::string time = ui->timeEdit->time().toString().toStdString();


    QWidget *outerPage = ui->stack_week->currentWidget();
    auto *stackPills  = outerPage->findChild<QStackedWidget*>();
    auto *comboAlarms = outerPage->findChild<QComboBox*>();
    QWidget *page = new QWidget(stackPills);
    auto *layout  = new QVBoxLayout(page);
    for (int i = 0; i < comboAlarms->count(); ++i) {
        QString text = comboAlarms->itemText(i);
        QTime t = QTime::fromString(text, "h:mm AP");
        std::string military = t.toString("HH:mm:ss").toStdString();
        if(military == time){
            return;
        }
    }
    QTableWidget *table = createCmpTable(page);
    layout->addWidget(table);
    page->setLayout(layout);
    int pageIndex  = stackPills->addWidget(page);
    int comboIndex = comboAlarms->count();
    comboAlarms->addItem(ui->timeEdit->time().toString("h:mm AP"));
    comboAlarms->setItemData(comboIndex, pageIndex);
    connect(comboAlarms, QOverload<int>::of(&QComboBox::currentIndexChanged),
            stackPills, &QStackedWidget::setCurrentIndex,
            Qt::UniqueConnection);


    printf("Hello");
    if(DOW == "Sun"){
        sun.add_alarm(time);
    }
    if(DOW == "Mon"){
        mon.add_alarm(time);
    }
    if(DOW == "Tue"){
        tue.add_alarm(time);
    }
    if(DOW == "Wed"){
        wed.add_alarm(time);
    }
    if(DOW == "Thu"){
        thu.add_alarm(time);
    }
    if(DOW == "Fri"){
        fri.add_alarm(time);
    }
    if(DOW == "Sat"){
        sat.add_alarm(time);
    }

    for(int i = 0; i < 12; i++){
        table->setItem(i, 0, new QTableWidgetItem());
        QTableWidgetItem *item = table->item(i, 0);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        item->setText("0");
        item->setTextAlignment(Qt::AlignCenter);
    }
}

void Dialog::on_addpills_button_clicked()
{
    int cmpt = ui->cmpt_input->value() - 1;

    QWidget *outerPage = ui->stack_week->currentWidget();
    auto *stackPills  = outerPage->findChild<QStackedWidget*>();
    auto *comboAlarms = outerPage->findChild<QComboBox*>();
    QWidget *alarmPage = stackPills->currentWidget();
    int currentIndex = comboAlarms->currentIndex();
    if (currentIndex < 0) return;
    QTableWidget *table = alarmPage->findChild<QTableWidget*>();
    table->item(cmpt, 0)->setText(ui->pill_count_input->text());
    QString text = ui->combo_alarms->currentText();
    QTime t = QTime::fromString(text, "h:mm AP");
    QString military = t.toString("HH:mm:ss");
    if(DOW == "Sun"){
        sun.set_pills(military.toStdString(), cmpt, ui->pill_count_input->value());
    }
    if(DOW == "Mon"){
        mon.set_pills(military.toStdString(), cmpt, ui->pill_count_input->value());
    }
    if(DOW == "Tue"){
        tue.set_pills(military.toStdString(), cmpt, ui->pill_count_input->value());
    }
    if(DOW == "Wed"){
        wed.set_pills(military.toStdString(), cmpt, ui->pill_count_input->value());
    }
    if(DOW == "Thu"){
        thu.set_pills(military.toStdString(), cmpt, ui->pill_count_input->value());
    }
    if(DOW == "Fri"){
        fri.set_pills(military.toStdString(), cmpt, ui->pill_count_input->value());
    }
    if(DOW == "Sat"){
        sat.set_pills(military.toStdString(), cmpt, ui->pill_count_input->value());
    }

    for (Alarm& x : sun.alarms) {
        qDebug() << x.time;
        qDebug() << x.pill_counts;
    }
}


void Dialog::on_removealarm_button_2_clicked()
{
    QWidget *outerPage = ui->stack_week->currentWidget();
    auto *comboAlarms = outerPage->findChild<QComboBox*>();
    auto *stackPills  = outerPage->findChild<QStackedWidget*>();
    int currentIndex = comboAlarms->currentIndex();
    if (currentIndex < 0) return;
    QWidget *page = stackPills->widget(currentIndex);
    stackPills->removeWidget(page);
    page->deleteLater();
    comboAlarms->removeItem(currentIndex);
    QString text = comboAlarms->currentText();
    QTime t = QTime::fromString(text, "h:mm AP");
    std::string time = t.toString("HH:mm:ss").toStdString();

    if(DOW == "Sun"){
        sun.remove_alarm(time);
    }
    if(DOW == "Mon"){
        mon.remove_alarm(time);
    }
    if(DOW == "Tue"){
        tue.remove_alarm(time);
    }
    if(DOW == "Wed"){
        wed.remove_alarm(time);
    }
    if(DOW == "Thu"){
        thu.remove_alarm(time);
    }
    if(DOW == "Fri"){
        fri.remove_alarm(time);
    }
    if(DOW == "Sat"){
        sat.remove_alarm(time);
    }

}

void sendDay(std::string dayName, const DOW& day, QSerialPort* arduino)
{
    arduino->write("CLEAR\n");
    for (Alarm x : day.alarms) {
        qDebug() << "Alarm write start";
        arduino->write("BEGIN\n");
        arduino->write(dayName.c_str());
        arduino->write("\n");

        arduino->write(x.time.c_str());
        arduino->write("\n");

        for (int y : x.pill_counts) {
            std::string s = std::to_string(y);
            arduino->write(s.c_str());
            arduino->write("\n");      // separator so they don't smush
        }
        arduino->write("\n");         // end of this
        arduino->write("END\n");
        qDebug() << "Alarm written";
    }
    arduino->flush();
}

void Dialog::on_arduino_button_clicked()
{
    sendDay("Sunday",    sun, arduino);
    sendDay("Monday",    mon, arduino);
    sendDay("Tuesday",   tue, arduino);
    sendDay("Wednesday", wed, arduino);
    sendDay("Thursday",  thu, arduino);
    sendDay("Friday",    fri, arduino);
    sendDay("Saturday",  sat, arduino);
}


void Dialog::on_addpills_button_2_clicked()
{
    qDebug() << "Hello";
    int cmpt = ui->cmpt_input->value() - 1;
    int val  = ui->pill_count_input->value();
    QString valStr = QString::number(val);

    QWidget *outerPage = ui->stack_week->currentWidget();
    auto *stackPills  = outerPage->findChild<QStackedWidget*>();
    auto *comboAlarms = outerPage->findChild<QComboBox*>();

    if (!stackPills || !comboAlarms) return;
    if (comboAlarms->count() == 0) return;

    // ---- 1) Update ALL alarm tables in UI using combo->itemData(pageIndex) ----
    for (int i = 0; i < comboAlarms->count(); ++i) {

        int pageIndex = comboAlarms->itemData(i).toInt();
        if (pageIndex < 0 || pageIndex >= stackPills->count()) {
            pageIndex = i; // fallback if itemData missing
        }

        QWidget *alarmPage = stackPills->widget(pageIndex);
        if (!alarmPage) continue;

        QTableWidget *table = alarmPage->findChild<QTableWidget*>();
        if (!table) continue;

        if (!table->item(cmpt, 0)) {
            table->setItem(cmpt, 0, new QTableWidgetItem());
            table->item(cmpt, 0)->setFlags(table->item(cmpt, 0)->flags() & ~Qt::ItemIsEditable);
        }

        table->item(cmpt, 0)->setText(valStr);
        table->item(cmpt, 0)->setTextAlignment(Qt::AlignCenter);
    }

    // ---- 2) Update ALL alarms in the DATA MODEL for this day ----
    if (DOW == "Sun") {
        for (Alarm &a : sun.alarms) {
            if ((int)a.pill_counts.size() > cmpt) a.pill_counts[cmpt] = val;
            else sun.set_pills(a.time, cmpt, val);
        }
    }
    if (DOW == "Mon") {
        for (Alarm &a : mon.alarms) {
            if ((int)a.pill_counts.size() > cmpt) a.pill_counts[cmpt] = val;
            else mon.set_pills(a.time, cmpt, val);
        }
    }
    if (DOW == "Tue") {
        for (Alarm &a : tue.alarms) {
            if ((int)a.pill_counts.size() > cmpt) a.pill_counts[cmpt] = val;
            else tue.set_pills(a.time, cmpt, val);
        }
    }
    if (DOW == "Wed") {
        for (Alarm &a : wed.alarms) {
            if ((int)a.pill_counts.size() > cmpt) a.pill_counts[cmpt] = val;
            else wed.set_pills(a.time, cmpt, val);
        }
    }
    if (DOW == "Thu") {
        for (Alarm &a : thu.alarms) {
            if ((int)a.pill_counts.size() > cmpt) a.pill_counts[cmpt] = val;
            else thu.set_pills(a.time, cmpt, val);
        }
    }
    if (DOW == "Fri") {
        for (Alarm &a : fri.alarms) {
            if ((int)a.pill_counts.size() > cmpt) a.pill_counts[cmpt] = val;
            else fri.set_pills(a.time, cmpt, val);
        }
    }
    if (DOW == "Sat") {
        for (Alarm &a : sat.alarms) {
            if ((int)a.pill_counts.size() > cmpt) a.pill_counts[cmpt] = val;
            else sat.set_pills(a.time, cmpt, val);
        }
    }
}


void Dialog::on_changetime_button_clicked()
{
    QWidget *outerPage = ui->stack_week->currentWidget();
    auto *comboAlarms = outerPage->findChild<QComboBox*>();
    auto *stackPills  = outerPage->findChild<QStackedWidget*>();

    int currentIndex = comboAlarms->currentIndex();
    if (currentIndex < 0) return;

    // preserve the correct page index for THIS alarm
    int pageIndex = comboAlarms->itemData(currentIndex).toInt();

    // old time from current combo text (display format)
    QString oldText = comboAlarms->currentText();
    QTime oldT = QTime::fromString(oldText, "h:mm AP");
    std::string old_time = oldT.toString("HH:mm:ss").toStdString();

    // new time from timeEdit
    QTime newT = ui->timeEdit->time();
    std::string new_time = newT.toString("HH:mm:ss").toStdString();
    QString newDisplay = newT.toString("h:mm AP");

    // prevent duplicates in this day's combo
    for (int i = 0; i < comboAlarms->count(); ++i) {
        if (i == currentIndex) continue;
        QTime t = QTime::fromString(comboAlarms->itemText(i), "h:mm AP");
        if (t.toString("HH:mm:ss").toStdString() == new_time) {
            return;  // already exists, do nothing
        }
    }

    // update combo display text but keep same page mapping
    comboAlarms->setItemText(currentIndex, newDisplay);
    comboAlarms->setItemData(currentIndex, pageIndex);

    // (optional) make sure stack is still showing the correct page
    stackPills->setCurrentIndex(pageIndex);

    // update model
    if(DOW == "Sun") sun.set_time(new_time, old_time);
    if(DOW == "Mon") mon.set_time(new_time, old_time);
    if(DOW == "Tue") tue.set_time(new_time, old_time);
    if(DOW == "Wed") wed.set_time(new_time, old_time);
    if(DOW == "Thu") thu.set_time(new_time, old_time);
    if(DOW == "Fri") fri.set_time(new_time, old_time);
    if(DOW == "Sat") sat.set_time(new_time, old_time);
}
