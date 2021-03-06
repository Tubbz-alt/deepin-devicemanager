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

    /**
     * @brief setLimitRow 限制行数
     * @param row
     */
    void setLimitRow(int row);

    /**
     * @brief setColumnAndRow 设置行数和列数
     * @param row　行
     * @param column　列
     */
    void setColumnAndRow(int row, int column = 2);

    /**
     * @brief setItem 设置item
     * @param row　行
     * @param column　列
     * @param item
     */
    void setItem(int row, int column, QTableWidgetItem *item);

    /**
     * @brief toString 以字符串的方式获取信息
     * @return
     */
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
     * @brief setDeviceEnable
     * @param e
     */
    void setDeviceEnable(bool e);

    /**
     * @brief clear : 清空数据
     */
    void clear();

    /**
     * @brief setRowHeight 设置行高
     * @param row　指定哪一行
     * @param height　行高
     */
    void setRowHeight(int row, int height);

    /**
     * @brief resizeRowToContent 设置行高适应内容
     * @param row 指定行
     */
    void resizeRowToContent(int row);

    /**
     * @brief setItemDelegateForRow 设置每一行的委托
     * @param row　指定行
     * @param itemDelegate　指定委托
     */
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

    /**
     * @brief expandTable 手动扩展表格
     */
    void expandTable();

signals:
    void enableDeviceSignal();

public slots:
    void changeSize();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void initUI();

private:
    DetailTreeView *mp_Table;
};

#endif // PAGETABLEWIDGET_H
