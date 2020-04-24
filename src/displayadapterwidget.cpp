/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     AaronZhang <ya.zhang@archermind.com>
 *
 * Maintainer: AaronZhang <ya.zhang@archermind.com>
 * Maintainer: Yaobin <yao.bin@archermind.com>
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

#include "displayadapterwidget.h"
#include "deviceinfoparser.h"
#include <DApplication>

DWIDGET_USE_NAMESPACE

DisplayadapterWidget::DisplayadapterWidget(QWidget *parent) : DeviceInfoWidgetBase(parent, tr("Display Adapter"))
{
    initWidget();
}

void DisplayadapterWidget::initWidget()
{
    // 华为设备从gpuinfo中进行加载信息
    if (DeviceInfoParser::Instance().isHuaweiAndroidUos()) {
        return initGpuInof();
    }
    QList<QStringList> tabList;
    QList<ArticleStruct> articles;
    QSet<QString> existArticles;

    //从lshw，xrandr获取
    QStringList displayadapterList = DeviceInfoParser::Instance().getLshwDiaplayadapterList();
    QStringList screenList = DeviceInfoParser::Instance().getXrandrScreenName();
    //获取显存大小
    QStringList varm = DeviceInfoParser::Instance().getDmesgVram();

    for (int i = 0; i < displayadapterList.size(); ++i) {
        articles.clear();
        existArticles.clear();

        QString displayadapter = displayadapterList[i];
        QString pci_bus = DeviceInfoParser::Instance().queryData("lshw", displayadapter, "bus info");
        QRegExp reg("^pci@[0-9]*:([\\s\\S]*)$");
        if (reg.exactMatch(pci_bus)) {
            pci_bus = reg.cap(1);
        }

        QString lspciDeviceName;
        //从lspci模糊查找
        DeviceInfoParser::Instance().fuzzeyQueryKey("lspci", pci_bus, lspciDeviceName);

        QString lspciName = DeviceInfoParser::Instance().fuzzyQueryData("lspci", lspciDeviceName, "bus info");
        ArticleStruct name(tr("Name"));
        name.queryData("lspci", lspciDeviceName, "Name");
        name.value.remove(" Corporation", Qt::CaseInsensitive);

        int index = name.value.indexOf('(');
        if (index > 0) {
            name.value = name.value.mid(0, index).trimmed();
        }
        articles.push_back(name);
        existArticles.insert("product");

        ArticleStruct vendor(tr("Vendor"));
        vendor.queryData("lshw", displayadapter, "vendor");
        articles.push_back(vendor);
        existArticles.insert("vendor");

        ArticleStruct description(tr("Description"));
        description.queryData("lshw", displayadapter, "description", existArticles, articles);
        articles.push_back(description);
        existArticles.insert("Description");
// 显存字段
        ArticleStruct graphicMemory(tr("Graphic Memory"));
        //end with a empty char,for avoiding this article to be hidden
//        graphicMemory.value = QString("%1 ").arg(tr("Unknown"));

        graphicMemory.value = DeviceInfoParser::Instance().varmSize;
        articles.push_back(graphicMemory);
        existArticles.insert("Memory");

        if (i < screenList.size()) {
            QString screenName = screenList[i];
// 最大分辨率和最小分辨率
            ArticleStruct minimum(tr("Minimum Resolution"));
//            minimum.queryData( "xrandr", screenName, "minresolution");
//            articles.push_back(minimum);
            minimum.value = DeviceInfoParser::Instance().minResolution;
            articles.push_back(minimum);

            ArticleStruct maximum(tr("Maximum Resolution"));
//            maximum.queryData( "xrandr", screenName, "maxresolution");
//            articles.push_back(maximum);
            maximum.value = DeviceInfoParser::Instance().maxResolution;
            articles.push_back(maximum);
        }
//驱动从lspci中获取
        ArticleStruct driver(tr("Driver"));
        driver.queryData("lspci", lspciDeviceName, "Kernel modules");
        articles.push_back(driver);
        existArticles.insert("driver");

//        ArticleStruct interface(tr("Support Interface");
//        interface.value = DeviceInfoParser::Instance().getDisplayInterfaceList().join(", ");
//        articles.push_back(interface);

        ArticleStruct version(tr("Version"));
        version.queryData("lshw", displayadapter, "version");
        articles.push_back(version);
        existArticles.insert("version");

        ArticleStruct width(tr("Width"));
        width.queryData("lshw", displayadapter, "width");
        articles.push_back(width);
        existArticles.insert("width");

        ArticleStruct clock(tr("Clock"));
        clock.queryData("lshw", displayadapter, "clock");
        articles.push_back(clock);
        existArticles.insert("clock");

        ArticleStruct capabilities(tr("Capabilities"));
        capabilities.queryData("lshw", displayadapter, "capabilities");
        articles.push_back(capabilities);
        existArticles.insert("capabilities");

        ArticleStruct type(tr("Type"));
        type.queryData("lshw", displayadapter, "Type");
        articles.push_back(type);
        existArticles.insert("Type");

        ArticleStruct busInfo(tr("bus info"));
        busInfo.queryData("lshw", displayadapter, "bus info");
        articles.push_back(busInfo);
        existArticles.insert("bus info");

        ArticleStruct ioport(tr("ioport"));
        ioport.queryData("lshw", displayadapter, "ioport");
        articles.push_back(ioport);
        existArticles.insert("ioport");

        ArticleStruct irq(tr("irq"));
        irq.queryData("lshw", displayadapter, "irq");
        articles.push_back(irq);
        existArticles.insert("irq");

        ArticleStruct latency(tr("latency"));
        ioport.queryData("lshw", displayadapter, "latency");
        articles.push_back(ioport);
        existArticles.insert("latency");

        ArticleStruct memory(tr("memory"));
        memory.queryData("lshw", displayadapter, "memory");
        articles.push_back(memory);
        existArticles.insert("memory");

        ArticleStruct physicalId(tr("physical id"));
        physicalId.queryData("lshw", displayadapter, "physical id");
        articles.push_back(physicalId);
        existArticles.insert("physical id");

        DeviceInfoParser::Instance().queryRemainderDeviceInfo("lshw", displayadapter, articles, existArticles,"ManulTrack__DisplayAdapter","DisplayAdapter information");

        QString dpName = "";
        if (displayadapterList.size() > 1) {
            dpName = name.value;
        }

        addDevice(dpName, articles,  displayadapterList.size());

        QStringList tab = {
            name.value,
            vendor.value
        };

        tabList.push_back(tab);

        if (overviewInfo_.value.isEmpty() == false) {
            overviewInfo_.value += " / ";
        }

        overviewInfo_.value += name.value.remove(vendor.value, Qt::CaseInsensitive).trimmed();

        QList<ArticleStruct> subArticles;
        subArticles <</* memory <<*/ vendor;
        QString subValue = joinArticle(subArticles, " / ");

        if (subValue.isEmpty() == false) {
            overviewInfo_.value += " (";
            overviewInfo_.value += subValue;
            overviewInfo_.value += ")";
        }
    }

    if (displayadapterList.size() > 1) {
        QStringList headers = { tr("Name"), tr("Vendor")};
        addTable(headers, tabList);
    }
}
//此为华为设备显示适配器函数GPU
void DisplayadapterWidget::initGpuInof()
{
    QList<ArticleStruct> articles;
    if (DeviceInfoParser::Instance().isHuaweiAndroidUos()) {
        const auto &db = DeviceInfoParser::Instance().toolDatabase_.value("gpuinfo");
        foreach (auto gpuKey, db.keys()) {
            overviewInfo_.value += (overviewInfo_.value.isEmpty()) ? "" : " / ";
            overviewInfo_.value += gpuKey;

            articles.clear();
            foreach (auto artTitle, db.value(gpuKey).keys()) {
                ArticleStruct t(artTitle);
                t.value = db.value(gpuKey).value(artTitle);
                articles.push_back(t);
            }
            addDevice(gpuKey, articles, db.keys().count(), true);
        }
    } else {
        setCentralInfo(tr("Failed to find display adapter information"));
    }
}

