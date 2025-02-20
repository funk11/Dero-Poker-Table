/// dReam Tables Five Card Draw Poker
#include "mainwindow.h"
#include "confirm.h"
#include "menu.h"
#include "ui_menu.h"
#include "rpc/rpc.h"


QString Menu::userInfo;
QString Menu::passInfo;
QString Menu::contractAddress;
QString Menu::listingAddress;
QString Menu::donationAddress;
bool Menu::mainnet;
bool Menu::autoPayout;


Menu::Menu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Menu)
{
    ui->setupUi(this);
    ui->contractCheckBox->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->contractCheckBox->setFocusPolicy(Qt::NoFocus);
    ui->daemonConnectedBox->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->daemonConnectedBox->setFocusPolicy(Qt::NoFocus);
    ui->walletConnectedBox->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->walletConnectedBox->setFocusPolicy(Qt::NoFocus);
    rpc::IdHash = "null";
    Menu::listingAddress = "cc747db82060d99076689646e188986cc239d046c6bcbbb312a53df7f849e5d1";
    Menu::donationAddress = "dero1qyr8yjnu6cl2c5yqkls0hmxe6rry77kn24nmc5fje6hm9jltyvdd5qq4hn5pn";
    ui->autoPayRButton->setEnabled(false);
    ui->listTableButton->setEnabled(false);
    ui->delistTableButton->setEnabled(false);
    ui->ownerGroupBox->setEnabled(false);
    ui->daemonRPCinput->setText(rpc::daemonAddress);
    ui->walletRPCinput->setText(rpc::playerAddress);
    ui->contractLineEdit->setText(Menu::contractAddress);
    for (int i = 0; i < 9; i++) {
      ui->daemonRPCinput->backspace();
      ui->walletRPCinput->backspace();
    }
    ui->userInput->setText(Menu::userInfo);
    ui->userInput->backspace();
    ui->passwordInput->setText(Menu::passInfo);

    if(Menu::autoPayout == true){
        ui->autoPayRButton->setChecked(true);
    }

    connect(ui->daemonRPCinput, SIGNAL(textChanged(QString)),
          this, SLOT(daemonToggle()));

    connect(ui->walletRPCinput, SIGNAL(textChanged(QString)),
          this, SLOT(walletToggle()));

    connect(ui->userInput, SIGNAL(textChanged(QString)),
          this, SLOT(walletToggle()));

    connect(ui->passwordInput, SIGNAL(textChanged(QString)),
          this, SLOT(walletToggle()));

    connect(ui->contractLineEdit, SIGNAL(textChanged(QString)),
          this, SLOT(contractToggle()));

    checkDaemon();
    checkAddress();
    checkWallet();
    checkContract();
    checkIfListed();
    setFonts();
}


Menu::~Menu()
{
    delete ui;
}


void Menu::daemonToggle()
{
    rpc::daemonAddress =  ui->daemonRPCinput->text();
    ui->daemonConnectedBox->setChecked(false);
    ui->ownerGroupBox->setEnabled(false);
    ui->getTableButton->setEnabled(false);
    ui->findTablesButton->setEnabled(false);
    ui->menuTextBrowser->setText("Daemon Not Connected");
}


void Menu::walletToggle()
{
    rpc::playerAddress = ui->walletRPCinput->text();
    loginInfo();
    ui->walletConnectedBox->setChecked(false);
    ui->ownerGroupBox->setEnabled(false);
    ui->getTableButton->setEnabled(false);
    ui->menuTextBrowser->setText("Wallet Not Connected");
}


void Menu::contractToggle()
{
    Menu::contractAddress = ui->contractLineEdit->text();
    ui->contractCheckBox->setChecked(false);
    ui->ownerGroupBox->setEnabled(false);
    ui->menuTextBrowser->setText("Could Not Connect To Contract");
}


void Menu::setFonts()
{
    int fontId = QFontDatabase::addApplicationFont(":/fonts/Macondo-Regular.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont macondoRegular(fontFamily);
    macondoRegular.setPointSize(17);
    macondoRegular.setBold(true);
    ui->menuTextBrowser->setFont(macondoRegular);
    ui->menuTextBrowser->setText("Welcome to dReam Tables Five Card Poker\nTable v1.1.0");

    int fontId2 = QFontDatabase::addApplicationFont(":/fonts/Ubuntu-R.ttf");
    QString fontFamily2 = QFontDatabase::applicationFontFamilies(fontId2).at(0);
    QFont ubuntuRegular(fontFamily2);
    ubuntuRegular.setPointSize(10);
    ui->playerGroupBox->setFont(ubuntuRegular);
    ui->userInput->setFont(ubuntuRegular);
    ui->passwordInput->setFont(ubuntuRegular);
    ui->daemonRPCinput->setFont(ubuntuRegular);
    ui->daemonConnectedBox->setFont(ubuntuRegular);
    ui->daemonRPCbutton->setFont(ubuntuRegular);
    ui->walletRPCinput->setFont(ubuntuRegular);
    ui->walletConnectedBox->setFont(ubuntuRegular);
    ui->walletRPCbutton->setFont(ubuntuRegular);
    ui->contractLineEdit->setFont(ubuntuRegular);
    ui->contractCheckBox->setFont(ubuntuRegular);
    ui->contractButton->setFont(ubuntuRegular);
    ui->findTablesButton->setFont(ubuntuRegular);
    ui->getTableButton->setFont(ubuntuRegular);
    ui->buttonBox->setFont(ubuntuRegular);
    ui->ownerGroupBox->setFont(ubuntuRegular);
    ui->playersComboBox->setFont(ubuntuRegular);
    ui->anteSpinBox->setFont(ubuntuRegular);
    ui->setTableButton->setFont(ubuntuRegular);
    ui->cleanTableButton->setFont(ubuntuRegular);
    ui->delistTableButton->setFont(ubuntuRegular);
    ui->listTableButton->setFont(ubuntuRegular);
    ui->newTableButton->setFont(ubuntuRegular);
    ui->newTableButton->setFont(ubuntuRegular);
    ui->autoPayRButton->setFont(ubuntuRegular);
    ui->buttonBox->setFont(ubuntuRegular);
}


void Menu::on_contractButton_clicked()
{
    Menu::contractAddress = ui->contractLineEdit->text();
    checkContract();
    checkIfListed();
}


void Menu::on_daemonRPCbutton_clicked()
{
    rpc::daemonAddress = ui->daemonRPCinput->text()+"/json_rpc";
    checkDaemon();
}


void Menu::on_walletRPCbutton_clicked()
{
    loginInfo();
    rpc::playerAddress = ui->walletRPCinput->text()+"/json_rpc";
    checkAddress();
    checkWallet();
}


void Menu::on_setTableButton_clicked()
{
    setTable();
}


void Menu::on_cleanTableButton_clicked()
{
    cleanTable();
}


void Menu::on_getTableButton_clicked()
{
    Confirm::whichText = 1;
    confirmationBox();

    if(Confirm::actionConfirmed == true){
            getDreams();
            ui->menuTextBrowser->setText("Your table is being prepared and will be ready for use in one moment...");
            delay();
            fetchInfo();
            ui->getTableButton->setEnabled(false);
            ui->menuTextBrowser->setText("Check Tables.txt for table Id");
            Confirm::actionConfirmed = false;
    }else {

        Confirm::actionConfirmed = false;
    }
}


void Menu::on_newTableButton_clicked()
{
    Confirm::whichText = 2;
    confirmationBox();

    if(Confirm::actionConfirmed == true){
        system("./contract/createTable.sh >> contract/Tables.txt");
        ui->menuTextBrowser->setText("Check Tables.txt for table Id");
        Confirm::actionConfirmed = false;
    }else {

        Confirm::actionConfirmed = false;
    }

}


void Menu::confirmationBox()
{
    Confirm m;
    m.setModal(true);
    m.exec();
}


void Menu::on_autoPayRButton_clicked()
{
    if(ui->autoPayRButton->isChecked()){
        Menu::autoPayout = true;
    }else {
        Menu::autoPayout = false;
    }

}


void Menu::on_listTableButton_clicked()
{
    Confirm::whichText = 3;
    confirmationBox();

    if(Confirm::actionConfirmed == true){
        listTable();
        Confirm::actionConfirmed = false;
        ui->listTableButton->setEnabled(false);
        ui->delistTableButton->setEnabled(true);
    }else {

        Confirm::actionConfirmed = false;
    }

}


void Menu::on_delistTableButton_clicked()
{
    Confirm::whichText = 4;
    confirmationBox();

    if(Confirm::actionConfirmed == true){
        delistTable();
        ui->delistTableButton->setEnabled(false);
        ui->listTableButton->setEnabled(true);
    }else {

        Confirm::actionConfirmed = false;
    }

}


void Menu::on_findTablesButton_clicked()
{
    ui->menuTextBrowser->setText("Public Tables:\n");
    fetchListingScData();
}

