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

#ifndef WAITINGWIDGET_H
#define WAITINGWIDGET_H

#include <QObject>
#include <DWidget>
#include <DSpinner>
#include <DLabel>

using namespace Dtk::Widget;
class WaitingWidget : public DWidget
{
    Q_OBJECT
public:
    WaitingWidget(QWidget *parent = nullptr);
    ~WaitingWidget();

    /**
     * @brief start开始转动小圈圈
     */
    void start();

    /**
     * @brief stop:停止转动小圈圈
     */
    void stop();

    /**
     * @brief setLabelTxt:设置Label文本
     * @param txt :[in]需要设置成的文本
     */
    void setLabelTxt(const QString &txt);

private:
    DSpinner        *mp_Spinner;       // 小圈圈
    DLabel          *mp_Label;         // 小圈圈下面的label
};

#endif // WAITINGWIDGET_H
