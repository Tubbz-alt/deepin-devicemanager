/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     LZ <zhou.lu@archermind.com>
 *
 * Maintainer: LZ <zhou.lu@archermind.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DEVICEMEMORY_H
#define DEVICEMEMORY_H
#include"DeviceInfo.h"

class DeviceMemory : public DeviceBaseInfo
{
    Q_OBJECT
    Q_DISABLE_COPY(DeviceMemory)
public:
    DeviceMemory();

    /**
     * @brief setInfoFromLshw:设置从 sudo lshw -C memory 获取的内存信息
     * @param mapInfo:由lshw获取的信息map
     */
    void setInfoFromLshw(const QMap<QString, QString> &mapInfo);

    /**
     * @brief setInfoFromDmidecode:设置从 sudo dmideocde -t memory 获取的内存信息
     * @param mapInfo:由dmideocde获取的信息map
     * @return 布尔值，true:信息设置成功；false:信息设置失败
     */
    bool setInfoFromDmidecode(const QMap<QString, QString> &mapInfo);

    /**
     * @brief name:获取名称属性值
     * @return QString 名称属性值
     */
    const QString &name()const override;

    /**
     * @brief name:获取驱动属性值
     * @return QString 驱动属性值
     */
    const QString &driver()const override;

    /**
     * @brief vendor:获取制造商属性值
     * @return QString:制造商属性值
     */
    const QString &vendor()const;

    /**
     * @brief model:获取型号属性值
     * @return QString:型号属性值
     */
    const QString &model()const;

    /**
     * @brief size:获取大小属性值
     * @return QString:大小属性值
     */
    const QString &size()const;

    /**
     * @brief type:获取类型属性值
     * @return QString:类型属性值
     */
    const QString &type()const;

    /**
     * @brief speed:获取速度属性值
     * @return QString:速度属性值
     */
    const QString &speed()const;

    /**
     * @brief totalBandwidth:获取总带宽属性值
     * @return QString:总带宽属性值
     */
    const QString &totalBandwidth()const;

    /**
     * @brief dataBandwidth:获取数据带宽属性值
     * @return QString:数据带宽属性值
     */
    const QString &dataBandwidth()const;

    /**
     * @brief locator:获取内存槽位属性值
     * @return QString:内存槽位属性值
     */
    const QString &locator()const;

    /**
     * @brief serialNumber:获取序列号属性值
     * @return Qstring:序列号属性值
     */
    const QString &serialNumber()const;

    /**
     * @brief configuredSpeed:获取配置速度属性值
     * @return QString:配置速度属性值
     */
    const QString &configuredSpeed()const;

    /**
     * @brief minimumVoltage:获取最小电压属性值
     * @return QString:最小电压属性值
     */
    const QString &minimumVoltage()const;

    /**
     * @brief maximumVoltage:获取最大电压属性值
     * @return QString:最大电压属性值
     */
    const QString &maximumVoltage()const;

    /**
     * @brief configuredVoltage:获取配置电压属性值
     * @return QString:配置电压属性值
     */
    const QString &configuredVoltage()const;

    /**
     * @brief subTitle:获取子标题
     * @return 子标题
     */
    QString subTitle() override;

    /**
     * @brief getOverviewInfo:获取概况信息
     * @return 概况信息
     */
    const QString getOverviewInfo() override;

protected:

    /**
     * @brief initFilterKey:初始化可现实的可显示的属性,m_FilterKey
     */
    virtual void initFilterKey() override;

    /**
     * @brief loadBaseDeviceInfo:加载基本信息
     */
    void loadBaseDeviceInfo() override;

    /**
     * @brief loadOtherDeviceInfo:加载基本信息
     */
    void loadOtherDeviceInfo() override;

    /**
     * @brief loadTableHeader : 过去表格的表头数据
     */
    void loadTableHeader() override;

    /**
     * @brief loadTableData:加载表头信息
     */
    void loadTableData() override;

private:
    QString           m_Name;                   //<! 【名称】
    QString           m_Vendor;                 //<! 【制造商】
    QString           m_Model;                  //<! 【型号】
    QString           m_Size;                   //<! 【大小】
    QString           m_Type;                   //<! 【类型】
    QString           m_Speed;                  //<! 【速度】
    QString           m_TotalBandwidth;         //<! 【总带宽】
    QString           m_DataBandwidth;          //<! 【数据带宽】
    QString           m_Locator;                //<! 【内存槽位】
    QString           m_SerialNumber;           //<! 【序列号】
    QString           m_ConfiguredSpeed;        //<! 【配置速度】
    QString           m_MinimumVoltage;         //<! 【最小电压】
    QString           m_MaximumVoltage;         //<! 【最大电压】
    QString           m_ConfiguredVoltage;      //<! 【配置电压】
    QString           m_Driver;
};

#endif // DEVICEMEMORY_H
