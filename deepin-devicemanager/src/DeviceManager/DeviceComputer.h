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

#ifndef DEVICECOMPUTER_H
#define DEVICECOMPUTER_H
#include "DeviceInfo.h"


class DeviceComputer : public DeviceBaseInfo
{
    Q_OBJECT
    Q_DISABLE_COPY(DeviceComputer)
public:
    DeviceComputer();

    /**
     * @brief homeUrl:获取URL属性值
     * @return QString:URL属性值
     */
    const QString &homeUrl()const;

    /**
     * @brief osDescription:获取操作系统描述属性值
     * @return QString:操作系统描述属性值
     */
    const QString &osDescription();

    /**
     * @brief os:获取操作系统属性值
     * @return QString:操作系统属性值
     */
    const QString &os();

    /**
     * @brief vendor:获取制造商属性值
     * @return QString:制造商属性值
     */
    const QString &vendor();

    /**
     * @brief name:获取名称属性值
     * @return QString:名称属性值
     */
    const QString &name() const override;

    const QString &driver() const override;

    /**
     * @brief type:获取类型属性值
     * @return QString:类型属性值
     */
    const QString &type();

    /**
     * @brief setHomeUrl:设置URL属性值
     * @param value:URL属性值
     */
    void setHomeUrl(const QString &value);

    /**
     * @brief setOsDescription:设置操作系统描述属性值
     * @param value:操作系统描述属性值
     */
    void setOsDescription(const QString &value);

    /**
     * @brief setOS:设置操作系统属性值
     * @param value:操作系统属性值
     */
    void setOS(const QString &value);

    /**
     * @brief setVendor:设置制造商属性值
     * @param value:制造商属性值
     */
    void setVendor(const QString &value);

    /**
     * @brief setName:设置名称属性值
     * @param value:名称属性值
     */
    void setName(const QString &value);

    /**
     * @brief setType:设置类型属性值
     * @param value:类型属性值
     */
    void setType(const QString &value);

    /**
     * @brief setVendor:设置制造商属性值
     * @param dm1Vendor:制造商属性值1
     * @param dm2Vendor:制造商属性值2
     */
    void setVendor(const QString &dm1Vendor, const QString &dm2Vendor);

    /**
     * @brief setName:设置名称属性值
     * @param dm1Name:名称属性值1
     * @param dm2Name:名称属性值1
     * @param dm1Family:family属性值
     * @param dm1Version:版本属性值
     */
    void setName(const QString &dm1Name, const QString &dm2Name, const QString &dm1Family, const QString &dm1Version);

    /**
     * @brief getOverviewInfo:获取概况信息
     * @return 概况信息
     */
    const QString getOverviewInfo() override;

    const QString getOSInfo();

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
    QString            m_HomeUrl;
    QString            m_OsDescription;
    QString            m_OS;
    QString            m_Vendor;
    QString            m_Name;
    QString            m_Type;
    QString            m_Driver;
};

#endif // DEVICECOMPUTER_H
