#include "DeviceStorage.h"
#include<QDebug>

DeviceStorage::DeviceStorage()
    : DeviceBaseInfo()
    , m_Model("")
    , m_Vendor("")
    , m_MediaType("")
    , m_Size("")
    , m_RotationRate("")
    , m_Interface("")
    , m_SerialNumber("")
    , m_Version("")
    , m_Capabilities("")
    , m_Description("")
    , m_KeyToLshw("")
    , m_KeyFromStorage("")
{
    initFilterKey();
}

bool DeviceStorage::setHwinfoInfo(const QMap<QString, QString> &mapInfo)
{
    // 龙芯机器中 hwinfo --disk会列出所有的分区信息
    // 存储设备不应包含分区，根据SysFS BusID 来确定是否是分区信息
    if (mapInfo.find("SysFS BusID") == mapInfo.end()) {
        return false;
    }
    setAttribute(mapInfo, "Model", m_Model);
    setAttribute(mapInfo, "Vendor", m_Vendor);
    setAttribute(mapInfo, "Drive", m_Driver);
    QRegExp exp("pci 0x[0-9a-zA-Z]*");
    if (exp.indexIn(m_Vendor) != -1) {
        m_Vendor = "";
    }

    setAttribute(mapInfo, "Attached to", m_Interface);
    QRegExp re(".*\\((.*)\\).*");
    if (re.exactMatch(m_Interface)) {
        m_Interface = re.cap(1);
        m_Interface.replace("Controller", "");
        m_Interface.replace("controller", "");
    }
    setAttribute(mapInfo, "Revision", m_Version);
    setAttribute(mapInfo, "Hardware Class", m_Description);
    setAttribute(mapInfo, "Capacity", m_Size);
    // hwinfo里面显示的内容是  14 GB (15376000000 bytes) 需要处理
    m_Size.replace(QRegExp("\\(.*\\)"), "").replace(" ", "");
    if (m_Size.startsWith("0") || m_Size == "") {
        return false;
    }

    setAttribute(mapInfo, "Serial ID", m_SerialNumber);
    ///setDiskSerialID(mapInfo["Device Files"]);
    setAttribute(mapInfo, "SysFS BusID", m_KeyToLshw);
    setAttribute(mapInfo, "Device File", m_DeviceFile);
    if (m_KeyToLshw.contains("nvme0", Qt::CaseInsensitive)) {
        setAttribute(mapInfo, "SysFS Device Link", m_NvmeKey);
    }
    getOtherMapInfo(mapInfo);
    return true;
}

bool DeviceStorage::setKLUHwinfoInfo(const QMap<QString, QString> &mapInfo)
{
    // 龙芯机器中 hwinfo --disk会列出所有的分区信息
    // 存储设备不应包含分区，根据SysFS BusID 来确定是否是分区信息
    if (mapInfo.find("SysFS BusID") == mapInfo.end()) {
        return false;
    }
    setAttribute(mapInfo, "Model", m_Model);
    setAttribute(mapInfo, "Vendor", m_Vendor);
    setAttribute(mapInfo, "Drive", m_Driver);

    setAttribute(mapInfo, "Attached to", m_Interface);
    QRegExp re(".*\\((.*)\\).*");
    if (re.exactMatch(m_Interface)) {
        m_Interface = re.cap(1);
        m_Interface.replace("Controller", "");
        m_Interface.replace("controller", "");
    }
    setAttribute(mapInfo, "Revision", m_Version);
    setAttribute(mapInfo, "Hardware Class", m_Description);
    setAttribute(mapInfo, "Capacity", m_Size);
    // hwinfo里面显示的内容是  14 GB (15376000000 bytes) 需要处理
    m_Size.replace(QRegExp("\\(.*\\)"), "").replace(" ", "");
    if (m_Size.startsWith("0") || m_Size == "") {
        return false;
    }


    setAttribute(mapInfo, "Serial ID", m_SerialNumber);
//    setDiskSerialID(mapInfo["Device Files"]);
    setAttribute(mapInfo, "SysFS BusID", m_KeyToLshw);
    setAttribute(mapInfo, "Device File", m_DeviceFile);

    // KLU里面的介质类型的处理方式比较特殊
    if (mapInfo["Driver"].contains("usb-storage")) {
        m_MediaType = "USB";
    }

    getOtherMapInfo(mapInfo);
    return true;
}

bool DeviceStorage::addInfoFromlshw(const QMap<QString, QString> &mapInfo)
{

    // 先获取需要进行匹配的关键字
    QStringList keys = mapInfo["bus info"].split("@");
    if (keys.size() != 2) {
        return false;
    }
    QString key = keys[1].trimmed();
    key.replace(".", ":");
    if (key != m_KeyToLshw) {
        return false;
    }


    // 获取唯一key
    QStringList words = mapInfo["bus info"].split(":");
    if (words.size() == 2) {
        m_KeyFromStorage = words[0];
        m_KeyFromStorage.replace("@", "");
    }

    // 更新接口
    setAttribute(mapInfo, "interface", m_Interface, false);


    // 获取基本信息
    getInfoFromLshw(mapInfo);

    // 获取其它设备信息
    getOtherMapInfo(mapInfo);

    return true;
}

bool DeviceStorage::addNVMEInfoFromlshw(const QMap<QString, QString> &mapInfo)
{
    QStringList keys = mapInfo["bus info"].split("@");
    if (keys.size() != 2) {
        return false;
    }

    QString key = keys[1].trimmed();

    // SysFS Device Link: /devices/pci0000:00/0000:00:05.0/0000:0d:00.0/nvme/nvme0
    //bus info: pci@0000:0d:00.0
    // 确认为同一设备
    if (m_NvmeKey.contains(key, Qt::CaseInsensitive)) {
        setAttribute(mapInfo, "vendor", m_Vendor);
    }

    return true;

}

bool DeviceStorage::addInfoFromSmartctl(const QString &name, const QMap<QString, QString> &mapInfo)
{
    // 查看传入的设备信息与当前的设备信息是不是同一个设备信息
    if (!m_DeviceFile.contains(name, Qt::CaseInsensitive)) {
        return false;
    }
    // 获取基本信息
    getInfoFromsmartctl(mapInfo);
    return true;
}

bool DeviceStorage::setMediaType(const QString &name, const QString &value)
{
    if (!m_DeviceFile.contains(name)) {
        return false;
    }

    if (QString("0") == value) {
        m_MediaType = QObject::tr("SSD");
    } else if (QString("1") == value) {
        m_MediaType = QObject::tr("HDD");
    } else {
        m_MediaType = QObject::tr("Unknown");
    }

    return true;
}

bool DeviceStorage::setKLUMediaType(const QString &name, const QString &value)
{
    if (!m_DeviceFile.contains(name)) {
        return false;
    }

    if (m_MediaType == "USB") {
        return true;
    }

    if (QString("0") == value) {
        m_MediaType = QObject::tr("SSD");
    } else if (QString("1") == value) {
        m_MediaType = QObject::tr("HDD");
    } else {
        m_MediaType = QObject::tr("Unknown");
    }

    return true;
}

bool DeviceStorage::isValid()
{
    // 若是m_Size为空则 该设备无效
    if (m_Size.isEmpty() == true) {
        return false;
    }

    return true;
}

void DeviceStorage::setDiskSerialID(const QString &deviceFiles)
{
    // Serial ID 与 device Files 中信息一致
    if (!m_SerialNumber.isEmpty() && deviceFiles.contains(m_SerialNumber)) {
        return;
    }

    QStringList itemList = deviceFiles.split(",");

    QString modelName = m_Model;
    modelName.replace(" ", "_");

    foreach (auto item, itemList) {
        if (item.contains("by-id", Qt::CaseInsensitive) &&
                item.contains(modelName, Qt::CaseInsensitive)) {

            int index;/* = item.indexOf(QRegExp("_[\\S\\d]*[\\s\\S]*$"));
            item = item.mid(index);*/
            index = item.lastIndexOf("_");
            item = item.mid(index + 1);
            item.replace(QRegExp("-[\\s\\S]*$"), "");
            m_SerialNumber = item;
            break;
        }
    }
}

QString DeviceStorage::compareSize(const QString &size1, const QString &size2)
{
    if (size1.isEmpty() || size2.isEmpty()) {
        return size1 + size2;
    }

    QRegExp reg("[0-9]*");
    int index = reg.indexIn(size1);
    int num1 = 0;
    int num2 = 0;
    if (index > 0) {
        num1 = reg.cap(0).toInt();
    }

    index = reg.indexIn(size2);

    if (index > 0) {
        num2 = reg.cap(0).toInt();
    }

    if (num1 > num2) {
        return size1;
    } else {
        return size2;
    }
}

const QString &DeviceStorage::name() const
{
    return m_Model;
}

const QString &DeviceStorage::driver() const
{
    return m_Driver;
}

const QString &DeviceStorage::model()const
{
    return m_Model;
}

const QString &DeviceStorage::vendor()const
{
    return m_Vendor;
}

const QString &DeviceStorage::mediaType()const
{
    return m_MediaType;
}

const QString &DeviceStorage::size()const
{
    return m_Size;
}
const QString &DeviceStorage::rotationRate()const
{
    return m_RotationRate;
}

const QString &DeviceStorage::interface()const
{
    return m_Interface;
}

const QString &DeviceStorage::serialNumber()const
{
    return m_SerialNumber;
}

const QString &DeviceStorage::version()const
{
    return m_Version;
}

const QString &DeviceStorage::capabilities()const
{
    return m_Capabilities;
}

const QString &DeviceStorage::description()const
{
    return m_Description;
}

const QString &DeviceStorage::keyFromStorage()const
{
    return m_KeyFromStorage;
}

const QString &DeviceStorage::firmwareVersion()const
{
    return m_FirmwareVersion;
}

const QString &DeviceStorage::speed()const
{
    return m_Speed;
}

const QString &DeviceStorage::powerOnHours()const
{
    return m_PowerOnHours;
}

const QString &DeviceStorage::powerCycleCount()const
{
    return m_PowerCycleCount;
}

QString DeviceStorage::subTitle()
{
    return m_Model;
}

const QString DeviceStorage::getOverviewInfo()
{
    return QString("%1 (%2)").arg(m_Model).arg(m_Size);;
}

void DeviceStorage::initFilterKey()
{
    // hwinfo --disk
    addFilterKey(QObject::tr("Hardware Class"));
    addFilterKey(QObject::tr("Device File"));
    addFilterKey(QObject::tr("ansiversion"));
    addFilterKey(QObject::tr("bus info"));
    addFilterKey(QObject::tr("logical name"));
    addFilterKey(QObject::tr("logicalsectorsize"));
    addFilterKey(QObject::tr("physical id"));
    addFilterKey(QObject::tr("sectorsize"));
    addFilterKey(QObject::tr("guid"));
    addFilterKey(QObject::tr("Config Status"));
    addFilterKey(QObject::tr("Device Number"));
    addFilterKey(QObject::tr("Geometry (Logical)"));
    addFilterKey(QObject::tr("SubDevice"));
    addFilterKey(QObject::tr("SubVendor"));
}

void DeviceStorage::loadBaseDeviceInfo()
{
    // 添加基本信息
    addBaseDeviceInfo(tr("Model"), m_Model);
    addBaseDeviceInfo(tr("Vendor"), m_Vendor);
    addBaseDeviceInfo(tr("Media Type"), m_MediaType);
    addBaseDeviceInfo(tr("Size"), m_Size);
    addBaseDeviceInfo(tr("Version"), m_Version);
    addBaseDeviceInfo(tr("Capabilities"), m_Capabilities);
}

void DeviceStorage::loadOtherDeviceInfo()
{
    // 添加其他信息,成员变量
    addOtherDeviceInfo(tr("Power Cycle Count"), m_PowerCycleCount);
    addOtherDeviceInfo(tr("Power On Hours"), m_PowerOnHours);
    addOtherDeviceInfo(tr("Firmware Version"), m_FirmwareVersion);
    addOtherDeviceInfo(tr("Speed"), m_Speed);
    addOtherDeviceInfo(tr("Description"), m_Description);
    addOtherDeviceInfo(tr("Serial Number"), m_SerialNumber);
    addOtherDeviceInfo(tr("Interface"), m_Interface);
    addOtherDeviceInfo(tr("Rotation Rate"), m_RotationRate);

    // 将QMap<QString, QString>内容转存为QList<QPair<QString, QString>>
    mapInfoToList();
}

void DeviceStorage::loadTableHeader()
{
    m_TableHeader.append(tr("Model"));
    m_TableHeader.append(tr("Vendor"));
    m_TableHeader.append(tr("Media Type"));
    m_TableHeader.append(tr("Size"));
}

void DeviceStorage::loadTableData()
{
    m_TableData.append(m_Model);
    m_TableData.append(m_Vendor);
    m_TableData.append(m_MediaType);
    m_TableData.append(m_Size);
}

void DeviceStorage::getInfoFromLshw(const QMap<QString, QString> &mapInfo)
{
    setAttribute(mapInfo, "capabilities", m_Capabilities);
    setAttribute(mapInfo, "version", m_Version);
    setAttribute(mapInfo, "serial", m_SerialNumber, false);
    setAttribute(mapInfo, "product", m_Model);
    setAttribute(mapInfo, "description", m_Description);
    setAttribute(mapInfo, "size", m_Size);
    // 223GiB (240GB)
    QRegExp re(".*\\((.*)\\)$");
    if (re.exactMatch(m_Size)) {
        m_Size = re.cap(1);
    }
}

void DeviceStorage::getInfoFromsmartctl(const QMap<QString, QString> &mapInfo)
{
    // 固件版本
    m_FirmwareVersion = mapInfo["Firmware Version"];

    // 速度
    QString sataVersion = mapInfo["SATA Version is"];
    QStringList strList = sataVersion.split(",");
    if (strList.size() == 2) {
        m_Speed = strList[1];
    }

    setAttribute(mapInfo, "Rotation Rate", m_RotationRate);

    // 通电时间
    m_PowerOnHours = mapInfo["Power_On_Hours"];
    if (m_PowerOnHours == "") {
        m_PowerOnHours = mapInfo["Power On Hours"];
    }

    // 通电次数
    m_PowerCycleCount = mapInfo["Power_Cycle_Count"];
    if (m_PowerCycleCount == "") {
        m_PowerCycleCount = mapInfo["Power Cycles"];
    }

    // 安装大小
    QString capacity = mapInfo["User Capacity"];
    if (capacity == "") {
        capacity = compareSize(mapInfo["Total NVM Capacity"], mapInfo["Namespace 1 Size/Capacity"]);
    }

    if (capacity != "") {
        QRegExp reg(".*\\[(.*)\\]$");
        if (reg.exactMatch(capacity)) {
            m_Size = reg.cap(1);
        }
    }

    // 型号
    //SATA
    if (mapInfo["Device Model"].isEmpty() == false) {
        m_Model = mapInfo["Device Model"];
    }

    //NVME
    if (mapInfo["Model Number"].isEmpty() == false) {
        m_Model = mapInfo["Model Number"];
    }

    setAttribute(mapInfo, "Serial Number", m_SerialNumber, true);
}
