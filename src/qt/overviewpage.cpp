#include "overviewpage.h"
#include "ui_overviewpage.h"

#include "walletmodel.h"
#include "bitcoinunits.h"
#include "optionsmodel.h"
#include "transactiontablemodel.h"
#include "transactionfilterproxy.h"
#include "guiutil.h"
#include "guiconstants.h"

#include <QAbstractItemDelegate>
#include <QPainter>
#include <QPixmap>

#define DECORATION_SIZE 64
#define NUM_ITEMS 8

class TxViewDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
public:
    TxViewDelegate(): QAbstractItemDelegate(), units(N_COLORS, BitcoinUnits::BTC)
    {
    }

    inline void paint(QPainter *painter, const QStyleOptionViewItem &option,
                      const QModelIndex &index ) const
    {
        painter->save();

        QIcon icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
        QRect mainRect = option.rect;
        QRect decorationRect(mainRect.topLeft(), QSize(DECORATION_SIZE, DECORATION_SIZE));
        int xspace = DECORATION_SIZE + 8;
        int ypad = 6;
        int halfheight = (mainRect.height() - 2*ypad)/2;
        QRect amountRect(mainRect.left() + xspace, mainRect.top()+ypad, mainRect.width() - xspace, halfheight);
        QRect addressRect(mainRect.left() + xspace, mainRect.top()+ypad+halfheight, mainRect.width() - xspace, halfheight);
        icon.paint(painter, decorationRect);

        QDateTime date = index.data(TransactionTableModel::DateRole).toDateTime();
        QString address = index.data(Qt::DisplayRole).toString();
        qint64 amount = index.data(TransactionTableModel::AmountRole).toLongLong();
        QString ticker = index.data(TransactionTableModel::TickerRole).toString();
        bool confirmed = index.data(TransactionTableModel::ConfirmedRole).toBool();
        QVariant value = index.data(Qt::ForegroundRole);
        QColor foreground = option.palette.color(QPalette::Text);
        if(value.canConvert<QBrush>())
        {
            QBrush brush = qvariant_cast<QBrush>(value);
            foreground = brush.color();
        }

        painter->setPen(foreground);
        painter->drawText(addressRect, Qt::AlignLeft|Qt::AlignVCenter, address);

        if(amount < 0)
        {
            foreground = COLOR_NEGATIVE;
        }
        else if(!confirmed)
        {
            foreground = COLOR_UNCONFIRMED;
        }
        else
        {
            foreground = option.palette.color(QPalette::Text);
        }
        int nColor = PSN_COLOR_P00;
        if (!GetColorFromTicker(ticker.toUtf8().constData(), nColor))
        {
            foreground = COLOR_INVALID;
        }
        painter->setPen(foreground);
        QString amountText = BitcoinUnits::formatWithUnit(units[nColor], amount, nColor, true);
        if(!confirmed)
        {
            amountText = QString("[") + amountText + QString("]");
        }
        painter->drawText(amountRect, Qt::AlignRight|Qt::AlignVCenter, amountText);

        painter->setPen(option.palette.color(QPalette::Text));
        painter->drawText(amountRect, Qt::AlignLeft|Qt::AlignVCenter, GUIUtil::dateTimeStr(date));

        painter->restore();
    }

    inline QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        return QSize(DECORATION_SIZE, DECORATION_SIZE);
    }

    std::vector<int> units;

};
#include "overviewpage.moc"

OverviewPage::OverviewPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OverviewPage),
    txdelegate(new TxViewDelegate()),
    filter(0)
{
    ui->setupUi(this);

    // Recent transactions
    ui->listTransactions->setItemDelegate(txdelegate);
    ui->listTransactions->setIconSize(QSize(DECORATION_SIZE, DECORATION_SIZE));
    ui->listTransactions->setMinimumHeight(NUM_ITEMS * (DECORATION_SIZE + 2));
    ui->listTransactions->setAttribute(Qt::WA_MacShowFocusRect, false);

    connect(ui->listTransactions, SIGNAL(clicked(QModelIndex)), this, SLOT(handleTransactionClicked(QModelIndex)));

    // init "out of sync" warning labels
    ui->labelWalletStatus->setText("(" + tr("out of sync") + ")");
    ui->labelTransactionsStatus->setText("(" + tr("out of sync") + ")");

    // start with displaying the "out of sync" warnings
    showOutOfSyncWarning(true);
}

void OverviewPage::handleTransactionClicked(const QModelIndex &index)
{
    if(filter)
        emit transactionClicked(filter->mapToSource(index));
}

OverviewPage::~OverviewPage()
{
    delete ui;
}

// Maturity: applies to coinbase (mint)
// Confirmed: 1 or more depth in chain
// Stake: applies to stake
// maps are color:amount
void OverviewPage::setBalances(const std::map<int, qint64> &mapBalance,
                               const std::map<int, qint64> &mapStake,
                               const std::map<int, qint64> &mapUnconfirmedBalance,
                               const std::map<int, qint64> &mapImmatureBalance)
{
    int unitPoSNCoin = model->getOptionsModel()->getDisplayUnitPoSNCoin();
    std::map<int, qint64> mapSpendable;
    model->FillNets(mapUnconfirmedBalance, mapBalance, mapSpendable);
    mapCurrentBalance = mapBalance;
    mapCurrentStake = mapStake;
    mapCurrentUnconfirmedBalance = mapUnconfirmedBalance;
    mapCurrentImmatureBalance = mapImmatureBalance;


    ui->labelBalancePoSNCoin->setText(BitcoinUnits::formatWithUnitLocalized(
             unitPoSNCoin, mapSpendable[PSN_COLOR_P02], PSN_COLOR_P02));
    ui->labelStakePoSNCoin->setText(BitcoinUnits::formatWithUnitLocalized(
             unitPoSNCoin, mapCurrentStake[PSN_COLOR_P02], PSN_COLOR_P02));
    ui->labelUnconfirmedPoSNCoin->setText(BitcoinUnits::formatWithUnitLocalized(
             unitPoSNCoin, mapCurrentUnconfirmedBalance[PSN_COLOR_P02], PSN_COLOR_P02));
    ui->labelImmaturePoSNCoin->setText(BitcoinUnits::formatWithUnitLocalized(
             unitPoSNCoin, mapCurrentImmatureBalance[PSN_COLOR_P02], PSN_COLOR_P02));
    ui->labelTotalPoSNCoin->setText(BitcoinUnits::formatWithUnitLocalized(
             unitPoSNCoin, mapSpendable[PSN_COLOR_P02] +
                           mapCurrentStake[PSN_COLOR_P02] +
                           mapCurrentUnconfirmedBalance[PSN_COLOR_P02], PSN_COLOR_P02));

    // only show unconfirmed, stake, immature balances non-zero, so as not to complicate things
    bool showUnconfirmedPoSNCoin = mapCurrentUnconfirmedBalance[PSN_COLOR_P02] != 0;
    ui->labelUnconfirmedPoSNCoin->setVisible(showUnconfirmedPoSNCoin);
    ui->lblUniUn->setVisible(showUnconfirmedPoSNCoin);
    bool showStakePoSNCoin = mapCurrentStake[PSN_COLOR_P02] != 0;
    ui->labelStakePoSNCoin->setVisible(showStakePoSNCoin);
    ui->lblUniStk->setVisible(showStakePoSNCoin);
    bool showImmaturePoSNCoin = mapCurrentImmatureBalance[PSN_COLOR_P02] != 0;
    ui->labelImmaturePoSNCoin->setVisible(showImmaturePoSNCoin);
    ui->lblUniImm->setVisible(showImmaturePoSNCoin);

}

void OverviewPage::setModel(WalletModel *model)
{
    this->model = model;
    if(model && model->getOptionsModel())
    {
        // Set up transaction list
        filter = new TransactionFilterProxy();
        filter->setSourceModel(model->getTransactionTableModel());
        filter->setLimit(NUM_ITEMS);
        filter->setDynamicSortFilter(true);
        filter->setSortRole(Qt::EditRole);
        filter->setShowInactive(false);
        filter->sort(TransactionTableModel::Status, Qt::DescendingOrder);

        ui->listTransactions->setModel(filter);
        ui->listTransactions->setModelColumn(TransactionTableModel::ToAddress);

        // Keep up to date with wallet
        std::map<int, qint64> mapBalance, mapStake,
                              mapUnconfirmedBalance, mapImmatureBalance;
        std::vector<int64_t> vBalance(0, N_COLORS);
        model->getBalance(GUI_OVERVIEW_COLORS, mapBalance);
        model->getStake(GUI_OVERVIEW_COLORS, mapStake);
        model->getUnconfirmedBalance(GUI_OVERVIEW_COLORS, mapUnconfirmedBalance);
        model->getImmatureBalance(GUI_OVERVIEW_COLORS, mapImmatureBalance);
        setBalances(mapBalance, mapStake, mapUnconfirmedBalance, mapImmatureBalance);
        connect(model, SIGNAL(balanceChanged(const std::map<int, qint64>&, const std::map<int, qint64>&,
                                             const std::map<int, qint64>&, const std::map<int, qint64>&)),
                this, SLOT(setBalances(const std::map<int, qint64>&, const std::map<int, qint64>&,
                                       const std::map<int, qint64>&, const std::map<int, qint64>&)));

        connect(model->getOptionsModel(), SIGNAL(displayUnitChangedPoSNCoin(int)),
                                                      this, SLOT(updateDisplayUnit()));
    }

    // update the display unit, to not use the default ("BTC")
    updateDisplayUnit();
}

void OverviewPage::updateDisplayUnit()
{
    if(model && model->getOptionsModel())
    {
        if(!mapCurrentBalance.empty())
        {
            std::map<int, qint64> mapStake;
            model->getStake(GUI_OVERVIEW_COLORS, mapStake);
            setBalances(mapCurrentBalance, mapStake,
                        mapCurrentUnconfirmedBalance, mapCurrentImmatureBalance);
        }

        // Update txdelegate->units with the current units
        for (int i = 1; i < N_COLORS; ++i)
        {
           txdelegate->units[i] = model->getOptionsModel()->getDisplayUnit(i);
        }

        ui->listTransactions->update();
    }
}

void OverviewPage::showOutOfSyncWarning(bool fShow)
{
    ui->labelWalletStatus->setVisible(fShow);
    ui->labelTransactionsStatus->setVisible(fShow);
}


