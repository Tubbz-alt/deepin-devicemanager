#include "DeviceCpu.h"

DeviceCpu::DeviceCpu()
    : DeviceBaseInfo()
    , m_Vendor("")
    , m_Name("")
    , m_PhysicalID("")
    , m_CoreID("")
    , m_ThreadNum("")
    , m_Frequency("")
    , m_CurFrequency("")
    , m_BogoMIPS("")
    , m_Architecture(""),
      m_Familly("")
    , m_Model("")
    , m_Step("")
    , m_CacheL1Data("")
    , m_CacheL1Order("")
    , m_CacheL2("")
    , m_CacheL3("")
    , m_Extensions("")
    , m_Flags("")
    , m_HardwareVirtual("")
    , m_LogicalCPUNum("")
    , m_CPUCoreNum("")
    , m_Driver("")
    , m_FrequencyIsRange(false)
{
    initFilterKey();
}

void DeviceCpu::initFilterKey()
{
    addFilterKey(QObject::tr("CPU implementer"));
    addFilterKey(QObject::tr("CPU architecture"));
    addFilterKey(QObject::tr("CPU variant"));
    addFilterKey(QObject::tr("CPU part"));
    addFilterKey(QObject::tr("CPU revision"));
}

void DeviceCpu::loadBaseDeviceInfo()
{
    addBaseDeviceInfo(tr("Name"), m_Name);
    addBaseDeviceInfo(tr("Vendor"), m_Vendor);
    addBaseDeviceInfo(tr("CPU ID"), m_PhysicalID);
    addBaseDeviceInfo(tr("Core ID"), m_CoreID);
    addBaseDeviceInfo(tr("Threads"), m_ThreadNum);
    addBaseDeviceInfo(tr("Current Speed"), m_CurFrequency);
    addBaseDeviceInfo(tr("BogoMIPS"), m_BogoMIPS);
    addBaseDeviceInfo(tr("Architecture"), m_Architecture);
    addBaseDeviceInfo(tr("CPU Family"), m_Familly);
    addBaseDeviceInfo(tr("Model"), m_Model);
}

void DeviceCpu::setCpuInfo(const QMap<QString, QString> &mapLscpu, const QMap<QString, QString> &mapLshw, const QMap<QString, QString> &mapDmidecode, const QMap<QString, QString> &catInfo)
{
    setInfoFromLscpu(mapLscpu);
    setInfoFromLshw(mapLshw);
    setInfoFromDmidecode(mapDmidecode);
    setInfoFromCatCpuinfo(catInfo);

    // CPU 名称后面不需要加个数
    m_Name.replace(QRegExp("/[0-9]*$"), "");
    m_Name.replace(QRegExp("x [0-9]*$"), "");
}

const QString &DeviceCpu::vendor() const
{
    return m_Vendor;
}

const QString &DeviceCpu::name() const
{
    return m_Name;
}

const QString &DeviceCpu::driver() const
{
    return m_Driver;
}

const QString &DeviceCpu::physicalID() const
{
    return m_PhysicalID;
}

const QString &DeviceCpu::coreID() const
{
    return m_CoreID;
}

const QString &DeviceCpu::threadNum() const
{
    return m_ThreadNum;
}

const QString &DeviceCpu::frequency() const
{
    return m_Frequency;
}

const QString &DeviceCpu::curFrequency() const
{
    return m_CurFrequency;
}

const QString &DeviceCpu::bogoMIPS() const
{
    return m_BogoMIPS;
}

const QString &DeviceCpu::architecture() const
{
    return m_Architecture;
}

const QString &DeviceCpu::familly() const
{
    return m_Familly;
}

const QString &DeviceCpu::model() const
{
    return m_Model;
}

const QString &DeviceCpu::step() const
{
    return m_Step;
}

const QString &DeviceCpu::cacheL1Data() const
{
    return m_CacheL1Data;
}

const QString &DeviceCpu::cacheL1Order() const
{
    return m_CacheL1Order;
}

const QString &DeviceCpu::cacheL2() const
{
    return m_CacheL2;
}

const QString &DeviceCpu::cacheL3() const
{
    return m_CacheL3;
}

const QString &DeviceCpu::extensions() const
{
    return m_Extensions;
}

const QString &DeviceCpu::flags() const
{
    return m_Flags;
}

const QString &DeviceCpu::hardwareVirtual() const
{
    return m_HardwareVirtual;
}

const QString &DeviceCpu::logicalCPUNum() const
{
    return m_LogicalCPUNum;
}

const QString &DeviceCpu::cpuCores() const
{
    return m_CPUCoreNum;
}

bool DeviceCpu::frequencyIsRange()const
{
    return m_FrequencyIsRange;
}

QString DeviceCpu::subTitle()
{
    return QString("%1 %2").arg(tr("Processor")).arg(m_PhysicalID);
}

const QString DeviceCpu::getOverviewInfo()
{
    getTrNumber();

    QString ov = QString("%1 (%2%3 / %4%5)") \
                 .arg(m_Name) \
                 .arg(m_trNumber[m_CPUCoreNum.toInt()]) \
                 .arg(tr("Core(s)")) \
                 .arg(m_trNumber[m_LogicalCPUNum.toInt()]) \
                 .arg(tr("Processor"));

    return ov;
}

void DeviceCpu::setInfoFromLscpu(const QMap<QString, QString> &mapInfo)
{
    setAttribute(mapInfo, "Model name", m_Name);
    setAttribute(mapInfo, "Vendor ID", m_Vendor);
    setAttribute(mapInfo, "Thread(s) per core", m_ThreadNum);
    setAttribute(mapInfo, "BogoMIPS", m_BogoMIPS);
    setAttribute(mapInfo, "Architecture", m_Architecture);
    setAttribute(mapInfo, "CPU family", m_Familly);
    setAttribute(mapInfo, "Model", m_Model);
    setAttribute(mapInfo, "Stepping", m_Step);
    setAttribute(mapInfo, "L1d cache", m_CacheL1Data);
    setAttribute(mapInfo, "L1i cache", m_CacheL1Order);
    setAttribute(mapInfo, "L2 cache", m_CacheL2);
    setAttribute(mapInfo, "L3 cache", m_CacheL3);
    setAttribute(mapInfo, "Flags", m_Flags);
    setAttribute(mapInfo, "Virtualization", m_HardwareVirtual);
    setAttribute(mapInfo, "CPU(s)", m_LogicalCPUNum);
    // 计算频率范围
    bool min = mapInfo.find("CPU min MHz") != mapInfo.end();
    bool max = mapInfo.find("CPU max MHz") != mapInfo.end();
    if (min && max) {
        double minHz = mapInfo["CPU min MHz"].toDouble() / 1000;
        double maxHz = mapInfo["CPU max MHz"].toDouble() / 1000;
        m_Frequency = QString("%1-%2 GHz").arg(minHz).arg(maxHz);
        m_FrequencyIsRange = true;
    }

    //获取扩展指令集
    QStringList orders = {"MMX", "SSE", "SSE2", "SSE3", "3D Now", "SSE4", "SSSE3", "SSE4_1", "SSE4_2", "AMD64", "EM64T"};
    foreach (const QString &order, orders) {
        if (mapInfo["Flags"].contains(order, Qt::CaseInsensitive)) {
            m_Extensions += QString("%1 ").arg(order);
        }
    }

    getOtherMapInfo(mapInfo);
}

void DeviceCpu::setInfoFromLshw(const QMap<QString, QString> &mapInfo)
{
    // longxin CPU型号不从lshw中获取
    // bug39874
    if (m_Name.contains("Loongson", Qt::CaseInsensitive)) {
        setAttribute(mapInfo, "version", m_Name, false);
    } else {
        setAttribute(mapInfo, "version", m_Name);
    }

    setAttribute(mapInfo, "vendor", m_Vendor);
//    setAttribute(mapInfo,"Thread(s) per core",m_ThreadNum);
//    setAttribute(mapInfo,"BogoMIPS",m_BogoMIPS);
//    setAttribute(mapInfo,"Architecture",m_Architecture);
//    setAttribute(mapInfo,"CPU family",m_Familly);
//    setAttribute(mapInfo,"Model",m_Model);
//    setAttribute(mapInfo,"Stepping",m_Step);
//    setAttribute(mapInfo,"L1d cache",m_CacheL1Data);
//    setAttribute(mapInfo,"L1i cache",m_CacheL1Order);
//    setAttribute(mapInfo,"L2 cache",m_CacheL2);
//    setAttribute(mapInfo,"L3 cache",m_CacheL3);
//    setAttribute(mapInfo,"Flags",m_Flags);
//    setAttribute(mapInfo,"Virtualization",m_HardwareVirtual);
//    setAttribute(mapInfo,"CPU(s)",m_LogicalCPUNum);

//    setAttribute(mapInfo, "capacity", m_Frequency, false);
//    setAttribute(mapInfo, "capacity", m_CurFrequency, false);

//    // 联想FT的环境没有capacity字段，但是有size字段
//    setAttribute(mapInfo, "size", m_Frequency, false);
//    setAttribute(mapInfo, "size", m_CurFrequency, false);

    getOtherMapInfo(mapInfo);
}

void DeviceCpu::setInfoFromDmidecode(const QMap<QString, QString> &mapInfo)
{
    // longxin CPU型号不从dmidecode中获取
    // bug39874
    if (m_Name.contains("Loongson", Qt::CaseInsensitive)) {
        setAttribute(mapInfo, "product", m_Name, false);
    } else {
        setAttribute(mapInfo, "product", m_Name);
    }

    setAttribute(mapInfo, "Manufacturer", m_Vendor);
    setAttribute(mapInfo, "Max Speed", m_Frequency, false);
    setAttribute(mapInfo, "Current Speed", m_CurFrequency);
//    setAttribute(mapInfo,"Thread(s) per core",m_ThreadNum);
//    setAttribute(mapInfo,"BogoMIPS",m_BogoMIPS);
//    setAttribute(mapInfo,"Architecture",m_Architecture);
    setAttribute(mapInfo, "Family", m_Familly);
//    setAttribute(mapInfo,"Model",m_Model);
//    setAttribute(mapInfo,"Stepping",m_Step);
//    setAttribute(mapInfo,"L1d cache",m_CacheL1Data);
//    setAttribute(mapInfo,"L1i cache",m_CacheL1Order);
//    setAttribute(mapInfo,"L2 cache",m_CacheL2);
//    setAttribute(mapInfo,"L3 cache",m_CacheL3);
//    setAttribute(mapInfo,"Flags",m_Flags);
//    setAttribute(mapInfo,"Virtualization",m_HardwareVirtual);
    setAttribute(mapInfo, "Core Count", m_CPUCoreNum);

    // 获取其他cpu信息
    getOtherMapInfo(mapInfo);
}

void DeviceCpu::setInfoFromCatCpuinfo(const QMap<QString, QString> &mapInfo)
{
    setAttribute(mapInfo, "processor", m_PhysicalID);

    // 兆芯和X86机器的关键字 core id  龙芯的关键字是 core
    setAttribute(mapInfo, "core id", m_CoreID);
    setAttribute(mapInfo, "core", m_CoreID);

    // 在FT-2000和pangu(都是arm) 的机器上没有 core 和 core id
    setAttribute(mapInfo, "processor", m_CoreID, false);

    // 在FT-2000和pangu(都是arm) 的机器上没有 cpu cores
    setAttribute(mapInfo, "cpu cores", m_CPUCoreNum);
    if (m_CPUCoreNum.isEmpty()) {
        m_CPUCoreNum = m_LogicalCPUNum;
    }

    // 龙芯机器无法获取型号 但是有cpu model
    setAttribute(mapInfo, "cpu model", m_Model, false);

    // 龙芯机器无法获取特性，需要在cat cpu中使用Loongson Features
    setAttribute(mapInfo, "Loongson Features", m_Flags, false);


    setAttribute(mapInfo, "CPU MHz", m_Frequency, false);
    setAttribute(mapInfo, "cpu MHz", m_Frequency, false);

    if (m_Frequency.contains(".")) {
        m_Frequency.replace(QRegExp("\\.00"), "MHz");
    }
    if (!m_Frequency.contains("MHz") && !m_Frequency.contains("GHz")) {
        m_Frequency += "MHz";
    }

    // 获取其他cpu信息
    getOtherMapInfo(mapInfo);
}

void DeviceCpu::loadOtherDeviceInfo()
{
    // 倒序，头插，保证原来的顺序
    addOtherDeviceInfo(tr("Virtualization"), m_HardwareVirtual);
    addOtherDeviceInfo(tr("Flags"), m_Flags);
    addOtherDeviceInfo(tr("Extensions"), m_Extensions);
    addOtherDeviceInfo(tr("L3 Cache"), m_CacheL3);
    addOtherDeviceInfo(tr("L2 Cache"), m_CacheL2);
    addOtherDeviceInfo(tr("L1i Cache"), m_CacheL1Order);
    addOtherDeviceInfo(tr("L1d Cache"), m_CacheL1Data);
    addOtherDeviceInfo(tr("Stepping"), m_Step);

    mapInfoToList();
}

void DeviceCpu::loadTableHeader()
{
    m_TableHeader.append(tr("Name"));
    m_TableHeader.append(tr("Vendor"));
    m_TableHeader.append(frequencyIsRange() ? tr("Speed") : tr("Max Speed"));
    m_TableHeader.append(tr("Architecture"));
}

void DeviceCpu::loadTableData()
{
    m_TableData.append(m_Name);
    m_TableData.append(m_Vendor);
    m_TableData.append(m_Frequency);
    m_TableData.append(m_Architecture);
}

void DeviceCpu::getTrNumber()
{
    m_trNumber.insert(1, QObject::tr("One"));
    m_trNumber.insert(2, QObject::tr("Two"));
    m_trNumber.insert(4, QObject::tr("Four"));
    m_trNumber.insert(6, QObject::tr("Six"));
    m_trNumber.insert(8, QObject::tr("Eight"));

    m_trNumber.insert(10, QObject::tr("Ten"));
    m_trNumber.insert(12, QObject::tr("Twelve"));
    m_trNumber.insert(14, QObject::tr("Fourteen"));
    m_trNumber.insert(16, QObject::tr("Sixteen"));
    m_trNumber.insert(18, QObject::tr("Eighteen"));

    m_trNumber.insert(20, QObject::tr("Twenty"));
    m_trNumber.insert(22, QObject::tr("Twenty-two"));
    m_trNumber.insert(24, QObject::tr("Twenty-four"));
    m_trNumber.insert(26, QObject::tr("Twenty-six"));
    m_trNumber.insert(28, QObject::tr("Twenty-eight"));

    m_trNumber.insert(30, QObject::tr("Thirty"));
    m_trNumber.insert(32, QObject::tr("Thirty-two"));
    m_trNumber.insert(34, QObject::tr("Thirty-four"));
    m_trNumber.insert(36, QObject::tr("Thirty-six"));
    m_trNumber.insert(38, QObject::tr("Thirty-eight"));

    m_trNumber.insert(40, QObject::tr("Forty"));
    m_trNumber.insert(42, QObject::tr("Forty-two"));
    m_trNumber.insert(44, QObject::tr("Forty-four"));
    m_trNumber.insert(46, QObject::tr("Forty-six"));
    m_trNumber.insert(48, QObject::tr("Forty-eight"));

    m_trNumber.insert(50, QObject::tr("Fifty"));
    m_trNumber.insert(52, QObject::tr("Fifty-two"));
    m_trNumber.insert(54, QObject::tr("Fifty-four"));
    m_trNumber.insert(56, QObject::tr("Fifty-six"));
    m_trNumber.insert(58, QObject::tr("Fifty-eight"));

    m_trNumber.insert(60, QObject::tr("Sixty"));
    m_trNumber.insert(62, QObject::tr("Sixty-two"));
    m_trNumber.insert(64, QObject::tr("Sixty-four"));
    m_trNumber.insert(66, QObject::tr("Sixty-six"));
    m_trNumber.insert(68, QObject::tr("Sixty-eight"));

    m_trNumber.insert(70, QObject::tr("Seventy"));
    m_trNumber.insert(72, QObject::tr("Seventy-two"));
    m_trNumber.insert(74, QObject::tr("Seventy-four"));
    m_trNumber.insert(76, QObject::tr("Seventy-six"));
    m_trNumber.insert(78, QObject::tr("Seventy-eight"));

    m_trNumber.insert(80, QObject::tr("Eighty"));
    m_trNumber.insert(82, QObject::tr("Eighty-two"));
    m_trNumber.insert(84, QObject::tr("Eighty-four"));
    m_trNumber.insert(86, QObject::tr("Eighty-six"));
    m_trNumber.insert(88, QObject::tr("Eighty-eight"));

    m_trNumber.insert(90, QObject::tr("Ninety"));
    m_trNumber.insert(92, QObject::tr("Ninety-two"));
    m_trNumber.insert(94, QObject::tr("Ninety-four"));
    m_trNumber.insert(96, QObject::tr("Ninety-six"));
    m_trNumber.insert(98, QObject::tr("Ninety-eight"));

    m_trNumber.insert(100, QObject::tr("One hundred"));
    m_trNumber.insert(102, QObject::tr("One hundred and Two"));
    m_trNumber.insert(104, QObject::tr("One hundred and four"));
    m_trNumber.insert(106, QObject::tr("One hundred and Six"));
    m_trNumber.insert(108, QObject::tr("One hundred and Eight"));

    m_trNumber.insert(110, QObject::tr("One hundred and Ten"));
    m_trNumber.insert(112, QObject::tr("One hundred and Twelve"));
    m_trNumber.insert(114, QObject::tr("One hundred and Fourteen"));
    m_trNumber.insert(116, QObject::tr("One hundred and Sixteen"));
    m_trNumber.insert(118, QObject::tr("One hundred and Eighteen"));

    m_trNumber.insert(120, QObject::tr("One hundred and Twenty"));
    m_trNumber.insert(122, QObject::tr("One hundred and Twenty-two"));
    m_trNumber.insert(124, QObject::tr("One hundred and Twenty-four"));
    m_trNumber.insert(126, QObject::tr("One hundred and Twenty-six"));
    m_trNumber.insert(128, QObject::tr("One hundred and Twenty-eight"));

}

