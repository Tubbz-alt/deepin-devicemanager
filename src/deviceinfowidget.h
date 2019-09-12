#pragma once

#include <QWidget>
#include <QList>
#include "DTableWidget"
#include "DScrollArea"

class QLabel;
class QVBoxLayout;

struct DeviceInfo
{
    QLabel* subTitle = nullptr;
    QList<QLabel*>  nameLabels;
    QList<QLabel*>  contentLabels;
};

class DeviceInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DeviceInfoWidget(QWidget *parent = nullptr);
    void setTitle(const QString& title);
    void addSubInfo(const QString& subTitle, const QStringList& names, const QStringList& contents);
    void addTable(const QStringList& headers, const QList<QStringList>& contentsList);

protected:
    //virtual void focusInEvent(QFocusEvent *e) override{};
    //virtual void focusOutEvent(QFocusEvent *e) override{};
signals:

public slots:

private:
    QLabel* title_ = nullptr;
    QList<DeviceInfo> deviceInfos_;
    QVBoxLayout* vLayout_ = nullptr;
    Dtk::Widget::DTableWidget* tableWidget_ = nullptr;
    Dtk::Widget::DWidget*   downWidget_  = nullptr;
    QVBoxLayout* downWidgetLayout = nullptr;
};
