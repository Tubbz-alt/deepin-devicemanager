#include "CmdButtonWidget.h"
#include "DetailTreeView.h"

#include <QHBoxLayout>
#include <QPainter>
#include <QDebug>

#include <DFontSizeManager>
#include <DApplication>
#include <DApplicationHelper>

CmdButtonWidget::CmdButtonWidget(DetailTreeView *parent)
    : DWidget(parent)
    , mp_cmdButton(new DCommandLinkButton(tr("More"), this))
{
    initUI();
    connect(mp_cmdButton, &DCommandLinkButton::clicked, this, &CmdButtonWidget::expandCommandLinkClicked);
}

void CmdButtonWidget::setButtonText(const QString &str)
{
    mp_cmdButton->setText(str);
}

void CmdButtonWidget::expandCommandLinkClicked()
{
    emit cmdButtonClicked();
}

void CmdButtonWidget::paintEvent(QPaintEvent *event)
{
    DWidget::paintEvent(event);


    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing, true);
    painter.setOpacity(1);
    painter.setClipping(true);

    // 获取调色板
    DApplicationHelper *dAppHelper = DApplicationHelper::instance();
    DPalette palette = dAppHelper->applicationPalette();

    // 获取窗口当前的状态,激活，禁用，未激活
    DPalette::ColorGroup cg;
    DWidget *wid = DApplication::activeWindow();
    if (wid/* && wid == this*/) {
        cg = DPalette::Active;
    } else {
        cg = DPalette::Inactive;
    }

    this->rect().setWidth(808);
    QRect rect  = this->rect();
    rect.setWidth(808);
    // 开始绘制边框 *********************************************************
    // 计算绘制区域
    int width = 1;
    QPainterPath paintPath, paintPathOut, paintPathIn;
    paintPathOut.addRect(rect);

    QRect rectIn = QRect(rect.x(), rect.y() + width, rect.width(), rect.height() - width);
    paintPathIn.addRect(rectIn);

    paintPath = paintPathOut.subtracted(paintPathIn);

    QBrush bgBrush(palette.color(cg, DPalette::FrameShadowBorder));
    painter.fillPath(paintPath, bgBrush);


    painter.restore();
}

void CmdButtonWidget::initUI()
{
    // 设置字号
    DFontSizeManager::instance()->bind(mp_cmdButton, DFontSizeManager::T8);

    // 将mp_CommandBtn放到布局中，居中
    QHBoxLayout *pHBoxLayout = new QHBoxLayout();
    pHBoxLayout->addStretch();
    pHBoxLayout->addWidget(mp_cmdButton);
    pHBoxLayout->addStretch();

    QVBoxLayout *pVBoxLayout = new QVBoxLayout();
    pVBoxLayout->addLayout(pHBoxLayout);

    this->setLayout(pVBoxLayout);
}
