#include "WaitingWidget.h"

#include <QHBoxLayout>

#include "MacroDefinition.h"


#define SPINNER_WIDTH  32  // 小圈圈的宽度
#define SPINNER_HEIGHT 32  // 小圈圈的高度


WaitingWidget::WaitingWidget(QWidget *parent)
    : DWidget(parent)
    , mp_Spinner(new DSpinner(this))
    , mp_Label(new DLabel(tr("Loading..."), this))
{
    // 设置小圈圈的参数,小圈圈的大小
    mp_Spinner->setFixedSize(SPINNER_WIDTH, SPINNER_HEIGHT);

    // 初始化布局
    QHBoxLayout *hLayout = new QHBoxLayout(this);
    hLayout->addStretch();

    QVBoxLayout *vLayout = new QVBoxLayout(this);
    vLayout->addStretch();

    QHBoxLayout *spinnerLayout = new QHBoxLayout(this);
    spinnerLayout->addStretch();
    spinnerLayout->addWidget(mp_Spinner);
    spinnerLayout->addStretch();
    vLayout->addLayout(spinnerLayout);

    QHBoxLayout *labelLayout = new QHBoxLayout(this);
    labelLayout->addStretch();
    labelLayout->addWidget(mp_Label);
    labelLayout->addStretch();
    vLayout->addLayout(labelLayout);

    vLayout->addStretch();

    hLayout->addLayout(vLayout);
    hLayout->addStretch();
    setLayout(hLayout);
}

WaitingWidget::~WaitingWidget()
{
    DELETE_PTR(mp_Spinner);
    DELETE_PTR(mp_Label);
}

void WaitingWidget::start()
{
    if (mp_Spinner) {
        mp_Spinner->start();
    }
}
void WaitingWidget::stop()
{
    if (mp_Spinner) {
        mp_Spinner->stop();
    }
}

void WaitingWidget::setLabelTxt(const QString &txt)
{
    if (mp_Label)
        mp_Label->setText(txt);
}
