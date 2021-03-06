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

#ifndef DEVICEPRINT_H
#define DEVICEPRINT_H
#include"DeviceInfo.h"


class DevicePrint : public DeviceBaseInfo
{
    Q_OBJECT
    Q_DISABLE_COPY(DevicePrint)
public:
    DevicePrint();

    /**
     * @brief setInfo:设置打印机信息
     * @param info:由cups第三方库获取的打印机信息map
     */
    void setInfo(const QMap<QString, QString> &info);

    /**
     * @brief name:获取名称属性值
     * @return QString 名称属性值
     */
    const QString &name()const override;

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
     * @brief serialNumber:获取序列号属性值
     * @return Qstring:序列号属性值
     */
    const QString &serialNumber()const;

    /**
     * @brief interfaceType:获取接口类型属性值
     * @return QString:接口类型属性值
     */
    const QString &interfaceType()const;

    /**
     * @brief URI:获取URL属性值
     * @return QString:URL属性值
     */
    const QString &URI()const;

    /**
     * @brief status:获取状态属性值
     * @return QString:状态属性值
     */
    const QString &status()const;

    /**
     * @brief shared:获取shared属性值
     * @return QString:shared属性值
     */
    const QString &shared()const;

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

    /**
     * @brief setEnable 设置设备的禁用状态
     * @param enable 是否禁用
     * @return 禁用是否成功
     */
    EnableDeviceStatus setEnable(bool enable);

    /**
     * @brief enable : 判断当前是否是禁用状态
     * @return
     */
    bool enable();

protected:

    /**
     * @brief initFilterKey:初始化可现实的可显示的属性,m_FilterKey
     */
    void initFilterKey() override;

    /**
     * @brief loadBaseDeviceInfo:加载基本信息
     */
    void loadBaseDeviceInfo() override;

    /**
     * @brief loadOtherDeviceInfo:加载基本信息
     */
    void loadOtherDeviceInfo() override;

    /**
     * @brief loadTableData:加载表头信息
     */
    void loadTableData() override;

private:
    QString             m_Name;                         //<! 【名称】
    QString             m_Vendor;                       //<! 【制造商】
    QString             m_Model;                        //<! 【型号】
    QString             m_SerialNumber;                 //<! 【序列号】
    QString             m_InterfaceType;                //<! 【接口类型】
    QString             m_URI;                          //<! 【URI】
    QString             m_Status;                       //<! 【状态】
    QString             m_Shared;                       //<! 【共享】
    QString             m_Driver;
};

#endif // DEVICEPRINT_H
