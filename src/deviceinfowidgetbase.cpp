#include "deviceinfowidgetbase.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include "DScrollBar"
#include <QSizePolicy>
#include "DFileDialog"
#include <QTextStream>
#include "document.h"
#include "table.h"
//#include "thirdlib/docx/include/table.h"
#include "xlsxdocument.h"
#include <QFile>
#include <mainwindow.h>

DWIDGET_USE_NAMESPACE

bool DeviceInfoWidgetBase::isTitleFontInit_ = false;
QFont DeviceInfoWidgetBase::titleFont_;
bool DeviceInfoWidgetBase::isSubTitleFontInit_ = false;
QFont DeviceInfoWidgetBase::subTitleFont_;

DeviceInfoWidgetBase::DeviceInfoWidgetBase(QWidget *parent, const QString& deviceName) : QWidget(parent), deviceName_(deviceName)
{
    vLayout_ = new QVBoxLayout;
    setLayout(vLayout_);
    setMinimumWidth(700);
    initContextMenu();
}

DeviceInfoWidgetBase::~DeviceInfoWidgetBase()
{
}

void DeviceInfoWidgetBase::initContextMenu()
{
    contextMenu_ = new QMenu;
    QAction* refreshAction = new QAction("Refresh", this);
    connect(refreshAction, &QAction::triggered, \
            [this]()
            {
                MainWindow* mainWindow = dynamic_cast<MainWindow*>(parent()->parent()->parent());
                if(mainWindow)
                {
                    mainWindow->refresh();
                }
            }
    );
    contextMenu_->addAction(refreshAction);


    QAction* exportAction = new QAction("Export to File");
    connect(exportAction, &QAction::triggered, this, &DeviceInfoWidgetBase::exportToFile);
    contextMenu_->addAction(exportAction);


    contextMenu_->addAction(new QAction("Attach",this));
    contextMenu_->addAction(new QAction("Detach",this));
}

void DeviceInfoWidgetBase::DeviceInfoWidgetBase::setTitle(const QString& title)
{
    if(titleInfo_==nullptr)
    {
       titleInfo_ = new DeviceInfo;
    }

    if(nullptr == titleInfo_->title)
    {
        titleInfo_->title = new DLabel(this);
    }

    if(isTitleFontInit_== false)
    {
        titleFont_ = titleInfo_->title->font();
        titleFont_.setBold(true);
        //titleFont_.setPixelSize(titleFont_.pixelSize() + 2);
        isTitleFontInit_ = true;
    }
    titleInfo_->title->setText(title);
    titleInfo_->title->setFont(titleFont_);

    vLayout_->insertWidget(vLayout_->count(), titleInfo_->title);
}

void DeviceInfoWidgetBase::addInfo(const QStringList& names, const QStringList& contents)
{
    initDownWidget();

    if(titleInfo_ == nullptr)
    {
       titleInfo_ = new DeviceInfo;
    }

    QHBoxLayout* hly = new QHBoxLayout;

    int increaseHeight = 10;
    hly->addSpacing(10);

    QGridLayout* gridLayout = new QGridLayout;
    gridLayout->setMargin(0);
    //gridLayout->setSizePo(QLayout::QSizePolicy::Minimum);
    hly->addLayout(gridLayout);
    downWidgetLayout->insertLayout(downWidgetLayout->count()-1, hly);

    for(int i = 0; i < names.size(); ++i)
    {
        QLabel* nameLabel = new DLabel(names.at(i), this);
        //nameLabel->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
        QLabel* contentLabel = new DLabel(contents.at(i), this);
        //contentLabel->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
        nameLabel->setFixedHeight(30);
        nameLabel->setFixedWidth(200);

        contentLabel->setFixedHeight(30);

        titleInfo_->nameLabels.push_back(nameLabel);
        titleInfo_->contentLabels.push_back(contentLabel);
        gridLayout->addWidget(nameLabel, i+1, 0);
        gridLayout->addWidget(contentLabel, i+1, 1);
        increaseHeight+= 30;
    }

    downWidgetLayout->insertSpacing(downWidgetLayout->count()-1, 25);
    downWidget_->setFixedHeight(downWidget_->height() + increaseHeight+25);
}

void DeviceInfoWidgetBase::addLinefeed()
{

}

void DeviceInfoWidgetBase::addSubInfo(const QString& subTitle, const QStringList& names, const QStringList& contents)
{
    initDownWidget();

    int increaseHeight = 0;

    DeviceInfo subInfo;
    if(false == subTitle.isEmpty())
    {
        subInfo.title = new DLabel(subTitle, this);
        if(isSubTitleFontInit_== false)
        {
            subTitleFont_ = subInfo.title->font();
            subTitleFont_.setBold(true);
            //subTitleFont_.setPixelSize(subTitleFont_.pixelSize() + 2);
            isSubTitleFontInit_ = true;
        }

        subInfo.title->setFont(subTitleFont_);
        subInfo.title->setFixedHeight(30);
        downWidgetLayout->insertWidget(downWidgetLayout->count()-1, subInfo.title);
        increaseHeight += 30;
    }

    QHBoxLayout* hly = new QHBoxLayout;

    QGridLayout* gridLayout = new QGridLayout;
    gridLayout->setMargin(0);
    hly->setMargin(0);
    hly->addSpacing(10);
    increaseHeight += 10;
    hly->addLayout(gridLayout);

    downWidgetLayout->insertLayout(downWidgetLayout->count()-1, hly);

    for(int i = 0; i < names.size(); ++i)
    {
        DLabel* nameLabel = new DLabel(names.at(i), this);
        //nameLabel->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
        DLabel* contentLabel = new DLabel(contents.at(i), this);
        //contentLabel->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
        nameLabel->setFixedHeight(30);
        nameLabel->setFixedWidth(200);
        contentLabel->setFixedHeight(30);
        subInfo.nameLabels.push_back(nameLabel);
        subInfo.contentLabels.push_back(contentLabel);
        gridLayout->addWidget(nameLabel, i+1, 1);
        gridLayout->addWidget(contentLabel, i+1, 2);
        increaseHeight+= 30;
    }

    deviceInfos_.push_back(subInfo);
    downWidgetLayout->insertSpacing(downWidgetLayout->count()-1, 25);
    increaseHeight += 25;
    downWidget_->setFixedHeight(downWidget_->height() + increaseHeight);
}

void DeviceInfoWidgetBase::addTable(const QStringList& headers, const QList<QStringList>& contentsList)
{
    if(tableWidget_ == nullptr)
    {
        tableWidget_ = new Dtk::Widget::DTableWidget(this);
        tableWidget_->setFixedHeight(150);
        tableWidget_->setVerticalScrollBar(new DScrollBar(this));
        tableWidget_->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);
        tableWidget_->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        //tableWidget_->horizontalHeader()->setClickable(false);
    }

    tableWidget_->setRowCount(contentsList.size());
    tableWidget_->setColumnCount(headers.size());

    tableWidget_->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    tableWidget_->setSelectionMode(QAbstractItemView::SingleSelection);

    tableWidget_->verticalHeader()->setVisible(false);
    tableWidget_->setGridStyle( Qt::PenStyle::NoPen);
    tableWidget_->setShowGrid(false);

    tableWidget_->setHorizontalHeaderLabels(headers);
    tableWidget_->resizeRowsToContents();
    tableWidget_->horizontalHeader()->setSectionResizeMode(headers.size() - 2, QHeaderView::Stretch);
    tableWidget_->horizontalHeader()->setDefaultAlignment(Qt::AlignmentFlag::AlignLeft);
    tableWidget_->setColumnWidth(0, 200);
    tableWidget_->setAlternatingRowColors(true);
    tableWidget_->setEditTriggers(QAbstractItemView::NoEditTriggers);

    vLayout_->insertWidget(0, tableWidget_);


    for(int i = 0; i < contentsList.size(); ++i)
    {
        const QStringList& contents = contentsList[i];
        for(int j = 0; j < contents.size(); ++j )
        {
            tableWidget_->setItem(i, j,new QTableWidgetItem(contents[j]));
        }
    }

}

void DeviceInfoWidgetBase::addStrecch()
{
    vLayout_->addStretch(1);
}

void DeviceInfoWidgetBase::initDownWidget()
{
    if(downWidget_)
    {
        return;
    }

    DScrollArea* downWidgetScrollArea_ = new DScrollArea(this);
    downWidgetScrollArea_->setFrameShape(QFrame::NoFrame);
    downWidget_ = new DWidget(downWidgetScrollArea_);
    //downWidget_->setFixedHeight(100);
    downWidget_->setBaseSize(700, 100);
    //downWidget_->setFixedWidth(700);
    downWidget_->setMinimumWidth(700);
    downWidgetLayout = new QVBoxLayout;
    downWidget_->setLayout(downWidgetLayout);
    downWidgetLayout->addStretch(1);
    downWidgetScrollArea_->setWidget(downWidget_);
    //downWidget_->setSizePolicy(QSizePolicy::Minimum);
    //downWidgetScrollArea_->setFixedHeight(100);
    downWidgetScrollArea_->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    //downWidgetScrollArea_->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    vLayout_->insertWidget(vLayout_->count(), downWidgetScrollArea_);
}

QString DeviceInfoWidgetBase::getDeviceName()
{
    return deviceName_;
}

void DeviceInfoWidgetBase::contextMenuEvent(QContextMenuEvent *event)
{
    contextMenu_->exec(event->globalPos());
}

bool DeviceInfoWidgetBase::exportToFile()
{
   QString selectFilter;
   QString exportFile = DFileDialog::getSaveFileName(this, tr("Export File"), "./" + deviceName_.remove(QRegExp("\\s")) + ".txt", \
                        tr("TEXT (*.txt);; Doc (*.doc);; Xls (*.xls);; Html (*.html)"), &selectFilter);

   if(exportFile.isEmpty() == true)
   {
       return true;
   }

    if(selectFilter == "TEXT (*.txt)")
    {
        return exportToTxt(exportFile);
    }

    if(selectFilter == "Doc (*.doc)")
    {
        return exportToDoc(exportFile);
    }

    if(selectFilter == "Xls (*.xls)")
    {
        return exportToXls(exportFile);
    }

    if(selectFilter == "Html (*.html)")
    {
        return exportToHtml(exportFile);
    }

    return false;
}

QTextStream& operator<<(QTextStream& ds, const DeviceInfo& di)
{
    if(di.title)
    {
        ds << di.title->text() << "\n";
    }


    for(int i = 0; i < di.nameLabels.size(); ++i)
    {
        ds.setFieldWidth(20);
        ds.setFieldAlignment(QTextStream::FieldAlignment::AlignLeft);
        ds << di.nameLabels[i]->text();
        ds.setFieldWidth(0);
        ds << di.contentLabels[i]->text()<< "\n";
    }

    return ds;
}

QTextStream& operator<<(QTextStream& ds, DTableWidget* tableWidget)
{
    if(tableWidget == nullptr || tableWidget->rowCount() < 1)
    {
        return ds;
    }

    for(int col = 0; col < tableWidget->columnCount(); ++col)
    {
        auto item = tableWidget->horizontalHeaderItem(col);
        ds.setFieldWidth(tableWidget->columnWidth(col) *100.0 / tableWidget->width());
        ds.setFieldAlignment(QTextStream::FieldAlignment::AlignLeft);
        if(item)
        {
             ds << tableWidget->horizontalHeaderItem(col)->text();
        }
        else
        {
            ds << "";
        }
        ds.setFieldWidth(0);
    }
    ds << "\n";


    for( int row = 0; row < tableWidget->rowCount(); ++row )
    {
        for( int col = 0; col < tableWidget->columnCount(); ++col )
        {
            ds.setFieldWidth(tableWidget->columnWidth(col) *100.0 / tableWidget->width());
            ds.setFieldAlignment(QTextStream::FieldAlignment::AlignLeft);
            ds << tableWidget->item(row,col)->text();
            ds.setFieldWidth(0);
        }

        ds << "\n";
    }

    return ds;
}

bool DeviceInfoWidgetBase::writeToTextStream(QTextStream& ts)
{
    if(tableWidget_)
    {
        ts << tableWidget_ << "\n";
    }

    if(titleInfo_)
    {
        ts << *titleInfo_ << "\n";
    }
    foreach(const DeviceInfo& di, deviceInfos_)
    {
        ts << di << "\n";
    }

    return true;
}

bool DeviceInfoWidgetBase::exportToTxt(const QString& txtFile)
{
    QFile file( txtFile );
    if( false == file.open(QIODevice::WriteOnly))
    {
        return false;
    }

    QTextStream out(&file);
    writeToTextStream(out);

    file.close();

    return true;
}

bool writeTabwidgetToDoc(DTableWidget* tableWidget, Docx::Document& doc)
{
    if(tableWidget == nullptr || tableWidget->rowCount() < 1)
    {
        return false;
    }

    Docx::Table* tab = doc.addTable(tableWidget->rowCount()+1, tableWidget->columnCount());
    tab->setAlignment(Docx::WD_TABLE_ALIGNMENT::LEFT);

    for(int col = 0; col < tableWidget->columnCount(); ++col)
    {
        auto item = tableWidget->horizontalHeaderItem(col);
        auto cel = tab->cell(0, col);
        if(item)
        {
             cel->addText(tableWidget->horizontalHeaderItem(col)->text());
        }
    }

    for( int row = 0; row < tableWidget->rowCount(); ++row )
    {
        for( int col = 0; col < tableWidget->columnCount(); ++col )
        {
            auto cel = tab->cell(row+1, col);
            cel->addText(tableWidget->item(row,col)->text());
        }
    }

    return true;
}

bool writeDeviceInfoToDoc(const DeviceInfo& di, Docx::Document& doc)
{
    if(di.title)
    {
        doc.addHeading(di.title->text(), 5);
    }

    for(int i = 0; i < di.nameLabels.size(); ++i)
    {
        QString name = di.nameLabels[i]->text();
        QString content = di.contentLabels[i]->text();
        QString line;
        if(name.isEmpty() == false || false == content.isEmpty())
        {
            line = name + " : " + content;
        }

        doc.addParagraph(line);
    }

    return true;
}

bool DeviceInfoWidgetBase::exportToDoc(const QString& docFile)
{
    Docx::Document doc(":/doc_template/template.doc");
    if(tableWidget_)
    {
        writeTabwidgetToDoc(tableWidget_, doc);
    }

    if(titleInfo_)
    {
        doc.addParagraph("\n");
        writeDeviceInfoToDoc(*titleInfo_, doc);
    }

    foreach(auto di, deviceInfos_)
    {
        doc.addParagraph("\n");
        writeDeviceInfoToDoc(di, doc);
    }

    doc.save(docFile);
    return true;
}

static int currentXlsRow = 1;

bool writeTabwidgetToXls(DTableWidget* tableWidget, QXlsx::Document& xlsx)
{
    if(tableWidget == nullptr || tableWidget->rowCount() < 1)
    {
        return false;
    }


    for(int col = 0; col < tableWidget->columnCount(); ++col)
    {
        auto item = tableWidget->horizontalHeaderItem(col);
        if(item)
        {
            QXlsx::Format boldFont;
            boldFont.setFontBold(true);
            xlsx.write( currentXlsRow, col+1, item->text(), boldFont );
        }
    }
    ++currentXlsRow;

    for( int row = 0; row < tableWidget->rowCount(); ++row )
    {
        for( int col = 0; col < tableWidget->columnCount(); ++col )
        {
            xlsx.write( currentXlsRow, col+1, tableWidget->item(row,col)->text());
        }
        ++currentXlsRow;
    }

    ++currentXlsRow;

    return true;
}

bool writeDeviceInfoToXls(const DeviceInfo& di, QXlsx::Document& xlsx)
{
    if(di.title)
    {
        QXlsx::Format boldFont;
        boldFont.setFontBold(true);
        xlsx.write( currentXlsRow++, 1, di.title->text(), boldFont);
    }

    for(int i = 0; i < di.nameLabels.size(); ++i)
    {
        xlsx.write(currentXlsRow, 1, di.nameLabels[i]->text());
        xlsx.write(currentXlsRow++, 2, di.contentLabels[i]->text());
    }

    ++currentXlsRow;

    return true;
}

bool DeviceInfoWidgetBase::exportToXls(const QString& xlsFile)
{
    currentXlsRow = 1;
    QXlsx::Document xlsx;

    if(tableWidget_)
    {
        writeTabwidgetToXls(tableWidget_, xlsx);
    }

    if(titleInfo_)
    {
        writeDeviceInfoToXls(*titleInfo_, xlsx);
    }

    foreach(auto di, deviceInfos_)
    {
        writeDeviceInfoToXls(di, xlsx);
    }

    xlsx.saveAs(xlsFile);

    return true;
}

bool writeTabwidgetToHtml(DTableWidget* tableWidget, QFile& html)
{
    if(tableWidget == nullptr || tableWidget->rowCount() < 1)
    {
        return false;
    }

    html.write("<table border=\"0\">\n");
    html.write("<thead><tr>\n");
    for(int col = 0; col < tableWidget->columnCount(); ++col)
    {
        auto item = tableWidget->horizontalHeaderItem(col);
        if(item)
        {
            html.write( QString("<th style=\"width:200px;text-align:left;\">" + item->text() + "</th>").toUtf8().data() );
        }
        else {
             html.write( "<td style=\"width:200px;text-align:left;\"> </td>" );
        }
    }
    html.write("</tr></thead>\n");

    for( int row = 0; row < tableWidget->rowCount(); ++row )
    {
        html.write("<tr>\n");
        for( int col = 0; col < tableWidget->columnCount(); ++col )
        {
            html.write( QString("<td style=\"width:200px;text-align:left;\">" + tableWidget->item(row,col)->text() + "</td>").toUtf8().data() );
        }
        html.write("</tr>\n");
    }


    html.write("</table>\n");
    return true;
}

bool writeDeviceInfoToHtml(const DeviceInfo& di, QFile& html)
{
    html.write("<table border=\"0\">\n");
    if(di.title)
    {
        html.write("<thead><tr>\n");
        html.write( QString("<th style=\"text-align:left;\">" + di.title->text() + "</th>\n").toUtf8().data() );
        html.write("</tr></thead>\n");
    }

    for(int i = 0; i < di.nameLabels.size(); ++i)
    {
        html.write("<tr>\n");
        html.write( QString("<td style=\"width:200px;text-align:left;\">" + di.nameLabels[i]->text() + "</td>").toUtf8().data() );
        html.write( QString( "<td>" + di.contentLabels[i]->text() + "</td>\n").toUtf8().data() );
        html.write("</tr>\n");
    }
    html.write("</table>\n");
    return true;
}

bool DeviceInfoWidgetBase::exportToHtml(const QString& htmlFile)
{
    QFile html(htmlFile);
    if(false == html.open(QIODevice::OpenModeFlag::WriteOnly))
    {
        return false;
    }

    html.write("<!DOCTYPE html>\n");
    html.write("<html>\n");
    html.write("<body>\n");

    if(tableWidget_)
    {
        writeTabwidgetToHtml(tableWidget_, html);
        html.write("<br />\n");
    }
    if(titleInfo_)
    {
        writeDeviceInfoToHtml(*titleInfo_, html);
        html.write("<br />\n");
    }

    foreach(auto di, deviceInfos_)
    {
        writeDeviceInfoToHtml(di, html);
    }

    html.write("</body>\n");
    html.write("</html>\n");
    html.close();
    return true;
}
