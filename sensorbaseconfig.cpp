#include "sensorbaseconfig.h"
#include <QDebug>

sensorBaseConfig::sensorBaseConfig(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *baseLayout = new QVBoxLayout();
    baseLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(baseLayout);

    QHBoxLayout *modeLayout = new QHBoxLayout();
    abpMode = new QCheckBox("ABP模式");
    modeLayout->addWidget(abpMode);
    otaaMode = new QCheckBox("OTAA模式");
    modeLayout->addWidget(otaaMode);
    connect(abpMode, SIGNAL(stateChanged(int)), this, SLOT(ABPModeCheck(int)));
    connect(otaaMode, SIGNAL(stateChanged(int)), this, SLOT(OTAAModeCheck(int)));

    modeLayout->addStretch();
    QLabel *channelLabel = new QLabel("信道");
    modeLayout->addWidget(channelLabel);
    channelSelect = new QComboBox();
    channelSelect->setMinimumWidth(200);
    for (uint32_t n = 0; n <= channelMax; n++) {
        channelSelect->addItem(QString("通信通道-%1").arg(n));
    }

    modeLayout->addWidget(channelSelect);
    baseLayout->addLayout(modeLayout);

    QRegExp hex32Exp("[0-9A-Fa-f]{,32}");
    QValidator *hex32validator = new QRegExpValidator(hex32Exp, this);
    QRegExp hex8Exp("[0-9A-Fa-f]{,8}");
    QValidator *hex8validator = new QRegExpValidator(hex8Exp, this);

    QGridLayout *addrLayout = new QGridLayout();
    baseLayout->addLayout(addrLayout);
    QLabel *daLabel = new QLabel("设备地址:");
    addrLayout->addWidget(daLabel, 0, 0);
    addrCombox = new QComboBox();
    addrCombox->setEditable(true);
    addrLineEdit = addrCombox->lineEdit();
    addrLineEdit->setValidator(hex8validator);
    addrLineEdit->setPlaceholderText("下拉选择历史记录");
    QFont addrfont = addrLineEdit->font();
    addrfont.setBold(true);
    addrLineEdit->setFont(addrfont);
    addrLayout->addWidget(addrCombox, 0, 1);

    QPushButton *upaddrBtn = new QPushButton();
    upaddrBtn->setFixedWidth(30);
    addrLayout->addWidget(upaddrBtn, 0, 2);
    upaddrBtn->setIcon(QIcon(":/icon/add.png"));
    upaddrBtn->setShortcut(Qt::Key_Up);
    QObject::connect(upaddrBtn, &QPushButton::clicked, this, [this]() {
        bool ok;

        int addr = addrLineEdit->text().toInt(&ok, 16);
        if (!ok) return ;

        addrLineEdit->setText(QString("%1").arg(addr + 1, 8, 16));
    });

    QPushButton *downaddrBtn = new QPushButton();
    downaddrBtn->setFixedWidth(30);
    addrLayout->addWidget(downaddrBtn, 0, 3);
    downaddrBtn->setIcon(QIcon(":/icon/minus.png"));
    downaddrBtn->setShortcut(Qt::Key_Down);
    QObject::connect(downaddrBtn, &QPushButton::clicked, this, [this]() {
        bool ok;

        int addr = addrLineEdit->text().toInt(&ok, 16);
        if (!ok) return ;

        addrLineEdit->setText(QString("%1").arg(addr - 1, 8, 16));
    });

    QLabel *askLabel = new QLabel("APPSKey:");
    addrLayout->addWidget(askLabel, 1, 0);
    askLineEdit = new QLineEdit();
    askLineEdit->setValidator(hex32validator);
    askLineEdit->setPlaceholderText("1234567890abcdef1234567890abcdef");
    addrLayout->addWidget(askLineEdit, 1, 1);

    QLabel *nskLabel = new QLabel("NWSKey:");
    addrLayout->addWidget(nskLabel, 2, 0);
    nskLineEdit = new QLineEdit();
    nskLineEdit->setValidator(hex32validator);
    nskLineEdit->setPlaceholderText("1234567890abcdef1234567890abcdef");
    addrLayout->addWidget(nskLineEdit, 2, 1);
}

sensorBaseConfig::~sensorBaseConfig()
{
    qDebug() << "~sensorBaseConfig()";
}

bool sensorBaseConfig::setConfig(SensorData::sensorBaseConfig &val)
{
    if (val.wm == SensorData::ABP) {
        abpMode->setCheckState(Qt::CheckState::Checked);
        otaaMode->setCheckState(Qt::CheckState::Unchecked);
    }
    else if (val.wm == SensorData::OTAA) {
        abpMode->setCheckState(Qt::CheckState::Unchecked);
        otaaMode->setCheckState(Qt::CheckState::Checked);
    }
    else {
        abpMode->setCheckState(Qt::CheckState::Unchecked);
        otaaMode->setCheckState(Qt::CheckState::Unchecked);
    }
    addrLineEdit->clear();
    askLineEdit->setText(val.appskey);
    nskLineEdit->setText(val.nwskey);
    channelSelect->setCurrentIndex(val.channelNum);

    return true;
}

bool sensorBaseConfig::getConfig(SensorData::sensorBaseConfig &val)
{
    if (abpMode->isChecked()) {
        val.wm = SensorData::ABP;
    }
    else if (otaaMode->isChecked()) {
        val.wm = SensorData::OTAA;
    }
    val.addr = addrLineEdit->text();
    val.appskey = askLineEdit->text();
    val.nwskey = nskLineEdit->text();
    val.channelNum = channelSelect->currentIndex();

    return true;
}

void sensorBaseConfig::setEnabled(bool b)
{
    abpMode->setEnabled(b);
    otaaMode->setEnabled(b);
    addrLineEdit->setEnabled(b);
    askLineEdit->setEnabled(b);
    nskLineEdit->setEnabled(b);
    channelSelect->setEnabled(b);
}

bool sensorBaseConfig::setDevAddr(const QString &addr)
{
    addrLineEdit->setText(addr);
    addAddrHistory(addr);

    return true;
}

void sensorBaseConfig::addAddrHistory(const QString &addr)
{
    if (!addrHistory.contains(addr)) {
        addrHistory << addr;
        addrCombox->insertItem(0, addr);
    }
}

bool sensorBaseConfig::installEventRecord(const QString &prefix)
{
    EventRecord::addEventObj(abpMode, this, EventRecord::objName(prefix, "ABP模式"));
    EventRecord::addEventObj(otaaMode, this, EventRecord::objName(prefix, "OTAA模式"));
    EventRecord::addEventObj(channelSelect, this, EventRecord::objName(prefix, "通信信道"));
    EventRecord::addEventObj(addrLineEdit, this, EventRecord::objName(prefix, "设备地址"));
    EventRecord::addEventObj(askLineEdit, this, EventRecord::objName(prefix, "APPSKEY"));
    EventRecord::addEventObj(nskLineEdit, this, EventRecord::objName(prefix, "NWSKEY"));

    return true;
}

void sensorBaseConfig::ABPModeCheck(int state)
{
    if (state == Qt::CheckState::Checked) {
        otaaMode->setCheckState(Qt::CheckState::Unchecked);
    }
    else {
        otaaMode->setCheckState(Qt::CheckState::Checked);
    }
}

void sensorBaseConfig::OTAAModeCheck(int state)
{
    if (state == Qt::CheckState::Checked) {
        abpMode->setCheckState(Qt::CheckState::Unchecked);
    }
    else {
        abpMode->setCheckState(Qt::CheckState::Checked);
    }
}
