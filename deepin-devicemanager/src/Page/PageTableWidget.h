#ifndef PAGETABLEWIDGET_H
#define PAGETABLEWIDGET_H

#include <QObject>
#include <QTableWidgetItem>

#include <DWidget>

DWIDGET_USE_NAMESPACE

class DetailTreeView;
class RichTextDelegate;

class PageTableWidget : public DWidget
{
    Q_OBJECT

public:
    PageTableWidget(DWidget *parent = nullptr);

    void setLimitRow(int row);

    void setColumnAndRow(int row, int column = 2);

    void setItem(int row, int column, QTableWidgetItem *item);

    QString toString();

    /**
     * @brief isOverview:是否是概况界面
     * @return false : 不是概况界面
     */
    virtual bool isOverview();

    /**
     * @brief isBaseBoard : 是否是主板信息界面
     * @return false : 不是主板信息界面
     */
    virtual bool isBaseBoard();

    /**
     * @brief clear : 清空数据
     */
    void clear();

    void setRowHeight(int row, int height);
    void resizeRowToContent(int row);

    void setItemDelegateForRow(int row, RichTextDelegate *itemDelegate);

    /**
     * @brief isCurDeviceEnable:当前设备是否启用
     * @return true:启用，false:禁用
     */
    bool isCurDeviceEnable();

    /**
     * @brief setCurDeviceState:设置当前设备状态
     * @param state true:启用;false:禁用
     */
    void setCurDeviceState(bool state);
signals:
    void enableDeviceSignal();

public slots:
    void enableDevice();

    void changeSize();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void initUI();

private:
    DetailTreeView *mp_Table;
};

#endif // PAGETABLEWIDGET_H