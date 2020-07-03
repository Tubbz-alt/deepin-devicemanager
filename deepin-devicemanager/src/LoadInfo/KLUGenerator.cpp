#include "KLUGenerator.h"
#include "../DeviceManager/DeviceManager.h"
#include "../DeviceManager/DeviceGpu.h"
#include "../DeviceManager/DeviceMonitor.h"
#include "../DeviceManager/DeviceKeyboard.h"
#include "../DeviceManager/DeviceOthers.h"
#include "../DeviceManager/DeviceStorage.h"
#include "../DeviceManager/DeviceAudio.h"
#include "../DeviceManager/DeviceComputer.h"

KLUGenerator::KLUGenerator()
{

}

void KLUGenerator::generatorComputerDevice()
{
    const QList<QMap<QString, QString> >  &cmdInfo = DeviceManager::instance()->cmdInfo("cat_os_release");

    DeviceComputer device;

    // home url
    if (cmdInfo.size() > 0) {
        QString value = cmdInfo[0]["HOME_URL"];
        device.setHomeUrl(value.replace("\"", ""));
    }

    // name type
    const QList<QMap<QString, QString> >  &sysInfo = DeviceManager::instance()->cmdInfo("lshw_system");
    if (sysInfo.size() > 0) {
        device.setType(sysInfo[0]["description"]);
        //device.setVendor(sysInfo[0]["vendor"]);
        device.setName(sysInfo[0]["product"]);
    }

    // setOsDescription
    QString os = "UOS";
    DSysInfo::DeepinType type = DSysInfo::deepinType();
    if (DSysInfo::DeepinProfessional == type) {
        os =  "UOS 20";
    } else if (DSysInfo::DeepinPersonal == type) {
        os =  "UOS 20 Home";
    } else if (DSysInfo::DeepinDesktop == type) {
        os =  "Deepin 20 Beta";
    }
    device.setOsDescription(os);

    // os
    const QList<QMap<QString, QString> >  &verInfo = DeviceManager::instance()->cmdInfo("cat_version");
    if (verInfo.size() > 0) {
        QString info = verInfo[0]["OS"].trimmed();
        info = info.trimmed();
        QRegExp reg("\\(gcc [\\s\\S]*(\\([\\s\\S]*\\))\\)", Qt::CaseSensitive);
        int index = reg.indexIn(info);
        if (index != -1) {
            QString tmp = reg.cap(0);
            info.remove(tmp);
            info.insert(index, reg.cap(1));
        }

        // 按照华为要求,去除所有括号中的信息
        reg.setPattern("\\([\\s\\S]*\\)");
        index = reg.indexIn(info);
        if (index != -1) {
            QString tmp = reg.cap(0);
            info.remove(tmp);
        }

        device.setOS(info);
    }
    DeviceManager::instance()->addComputerDevice(device);
}

void KLUGenerator::generatorGpuDevice()
{
    const QList<QMap<QString, QString>> lstMap = DeviceManager::instance()->cmdInfo("gpuinfo");
    QList<QMap<QString, QString> >::const_iterator it = lstMap.begin();
    for (; it != lstMap.end(); ++it) {
        if ((*it).size() < 2) {
            continue;
        }
        DeviceGpu device;
        device.setGpuInfo(*it);
        DeviceManager::instance()->addGpuDevice(device);
    }
}

void KLUGenerator::generatorMonitorDevice()
{
    QMap<QString, QString> mapInfo;
    mapInfo.insert("Name", "Notebook monitor");
    mapInfo.insert("Vendor", "HUAWEI");
    mapInfo.insert("CurResolution", "2160x1440@60Hz");
    mapInfo.insert("SupportResolution", "2160x1440@60Hz");
    mapInfo.insert("Size", "296x197mm");
    mapInfo.insert("Date", "2019年7月");

    DeviceMonitor monitor;
    monitor.setInfoFromSelfDefine(mapInfo);
    DeviceManager::instance()->addMonitor(monitor);
}

void KLUGenerator::generatorAudioDevice()
{
    getAudioInfoFromCatAudio();
}

void KLUGenerator::getKeyboardInfoFromHwinfo()
{
    const QList<QMap<QString, QString>> lstMap = DeviceManager::instance()->cmdInfo("hwinfo_keyboard");
    QList<QMap<QString, QString> >::const_iterator it = lstMap.begin();
    for (; it != lstMap.end(); ++it) {
        if ((*it).size() < 1) {
            continue;
        }
        if ((*it).contains("Device Files")) {
            DeviceKeyboard device;
            device.setKLUInfoFromHwinfo(*it);
            DeviceManager::instance()->addKeyboardDevice(device);
            addBusIDFromHwinfo((*it)["SysFS BusID"]);
        }
    }
}


void KLUGenerator::getOthersInfoFromHwinfo()
{
    const QList<QMap<QString, QString>> lstMap = DeviceManager::instance()->cmdInfo("hwinfo_usb");
    QList<QMap<QString, QString> >::const_iterator it = lstMap.begin();
    for (; it != lstMap.end(); ++it) {
        if ((*it).size() < 5) {
            continue;
        }

        bool isOtherDevice = true;
        QString curBus = (*it)["SysFS BusID"];
        curBus.replace(QRegExp("\\.[0-9]{1,2}$"), "");
        const QStringList &lstBusId = DeviceManager::instance()->getBusId();
        if (lstBusId.indexOf(curBus) != -1) {
            isOtherDevice = false;
        }
        if ((*it)["Driver"].contains("usb-storage")) {
            isOtherDevice = false;
        }

        if (isOtherDevice) {
            DeviceOthers device;
            device.setInfoFromHwinfo(*it);
            DeviceManager::instance()->addOthersDevice(device);
        }
    }
}

void KLUGenerator::getDiskInfoFromHwinfo()
{
    const QList<QMap<QString, QString>> lstDisk = DeviceManager::instance()->cmdInfo("hwinfo_disk");
    QList<QMap<QString, QString> >::const_iterator dIt = lstDisk.begin();
    for (; dIt != lstDisk.end(); ++dIt) {
        if ((*dIt).size() < 2) {
            continue;
        }
        DeviceStorage device;
        if (device.setKLUHwinfoInfo(*dIt) && device.isValid()) {
            DeviceManager::instance()->addStorageDeivce(device);
            addBusIDFromHwinfo((*dIt)["SysFS BusID"]);
        }
    }
}

void KLUGenerator::getDiskInfoFromLsblk()
{
    const QList<QMap<QString, QString>> lstblk = DeviceManager::instance()->cmdInfo("lsblk_d");
    if (lstblk.size() > 0) {
        QMap<QString, QString>::const_iterator it = lstblk[0].begin();
        for (; it != lstblk[0].end(); ++it) {
            DeviceManager::instance()->setKLUStorageDeviceMediaType(it.key(), it.value());
        }
    }
}

void KLUGenerator::getAudioInfoFromCatAudio()
{
    const QList<QMap<QString, QString>> lstAudio = DeviceManager::instance()->cmdInfo("cat_audio");
    QList<QMap<QString, QString> >::const_iterator it = lstAudio.begin();
    for (; it != lstAudio.end(); ++it) {
        if ((*it).size() < 2) {
            continue;
        }
        DeviceAudio device;
        device.setInfoFromCatAudio(*it);
        DeviceManager::instance()->addAudioDevice(device);
    }
}