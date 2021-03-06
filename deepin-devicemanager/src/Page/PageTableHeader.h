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

#ifndef DEVICETABLEPAGE_H
#define DEVICETABLEPAGE_H

#include <QObject>
#include <DWidget>

class TableWidget;

using namespace Dtk::Widget;
class PageTableHeader : public DWidget
{
    Q_OBJECT
public:
    PageTableHeader(QWidget *parent = nullptr);
    ~PageTableHeader();

    /**
     * @brief updateTable:更新表格
     * @param lst : 表格内容
     */
    void updateTable(const QList<QStringList> &lst);

    /**
     * @brief setColumnAverage:设置每列等宽
     */
    void setColumnAverage();

    /**
     * @brief updateCurItemEnable
     * @param row
     * @param enable
     */
    void updateCurItemEnable(int row, int enable);

signals:
    /**
     * @brief itemClicked:点击item发出信号
     * @param row : 点击某行的行号
     */
    void itemClicked(int row);

    /**
     * @brief refreshInfo:刷新信息发出信号
     */
    void refreshInfo();

    /**
     * @brief exportInfo:导出信息发出信号
     */
    void exportInfo();
    void enableDevice(int row, bool enable);

protected:
    void paintEvent(QPaintEvent *e) override;

private slots:
    /**
     * @brief slotItemClicked:Item 点击槽函数
     * @param row:点击某行的行号
     */
    void slotItemClicked(int row);

    /**
     * @brief slotRefreshInfo:刷新信息槽函数
     */
    void slotRefreshInfo();

    /**
     * @brief slotExportInfo:导出信息槽函数
     */
    void slotExportInfo();
    void slotEnableDevice(int row, bool enable);

private:
    /**
     * @brief initWidgets : 初始化界面布局
     */
    void initWidgets();

private:
    TableWidget               *mp_Table;
};

#endif // DEVICETABLEPAGE_H
