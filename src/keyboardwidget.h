#pragma once

#include "deviceinfowidgetbase.h"

class KeyboardWidget : public DeviceInfoWidgetBase
{
    Q_OBJECT
public:
    explicit KeyboardWidget(QWidget *parent = nullptr);

    void initWidget() override;
};

