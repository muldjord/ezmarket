/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            mainwindow.cpp
 *
 *  Sat Apr 30 09:03:00 CEST 2022
 *  Copyright 2022 Lars Muldjord
 *  muldjordlars@gmail.com
 ****************************************************************************/
/*
 *  This file is part of EZMarket.
 *
 *  EZMarket is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  EZMarket is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with EZMarket; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
 */

#include "mainwindow.h"
#include "aboutbox.h"
#include "entryeditor.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QApplication>
#include <QFile>
#include <QSound>
#include <QRandomGenerator>
#include <QDir>
#include <QToolBar>
#include <QLocale>
#include <QKeyEvent>
#include <QDateTime>

MainWindow::MainWindow()
{
  setWindowTitle("EZMarket v" VERSION);
  showFullScreen();

  setStyleSheet("QLabel {font-size: " + QString::number(data.fontSizeSmall) + "px;}"
                "QLineEdit {font-size: " + QString::number(data.fontSizeSmall) + "px;}"
                "QSpinBox {font-size: " + QString::number(data.fontSize) + "px;}"
                "QTabWidget {qproperty-iconSize: " + QString::number(data.iconSizeSmall) + "px; font-size: " + QString::number(data.fontSizeSmall) + "px;}"
                "QListWidget {font-size: " + QString::number(data.fontSizeSmall) + "px;}"
                "QGroupBox{font-size: " + QString::number(data.fontSize) + "px; padding-left: 20px; padding-top: 40px; padding-bottom: 20px; padding-right: 20px;}");
  createActions();
  createMenus();
  createToolBar();
  
  loadIcons();
  loadDatabase();
  
  accountsTab = new AccountsTab(data, this);
  itemsTab = new ItemsTab(data, this);
  categoriesTab = new CategoriesTab(data, this);
  soundboardTab = new Soundboard(data, this);
  checkoutTab = new Checkout(data, this);
  
  modeTabs = new QTabWidget(this);
  modeTabs->addTab(accountsTab, QIcon(QPixmap("graphics/account.png").scaled(data.iconSize, data.iconSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)), tr("Accounts"));
  modeTabs->addTab(categoriesTab, QIcon(QPixmap("graphics/category.png").scaled(data.iconSize, data.iconSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)), tr("Categories"));
  modeTabs->addTab(itemsTab, QIcon(QPixmap("graphics/item.png").scaled(data.iconSize, data.iconSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)), tr("Items"));
  modeTabs->addTab(soundboardTab, QIcon(QPixmap("graphics/sound.png").scaled(data.iconSize, data.iconSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)), tr("Announcements"));
  modeTabs->addTab(checkoutTab, QIcon(QPixmap("graphics/checkout.png").scaled(data.iconSize, data.iconSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)), tr("Checkout"));

  QVBoxLayout *layout = new QVBoxLayout();
  layout->addWidget(modeTabs);

  setCentralWidget(new QWidget());
  centralWidget()->setLayout(layout);

  randomTimer.setSingleShot(false);
  connect(&randomTimer, &QTimer::timeout, this, &MainWindow::initRandomSound);
  initRandomSound();

  focusTimer.setInterval(1000);
  focusTimer.setSingleShot(false);
  connect(&focusTimer, &QTimer::timeout, this, &MainWindow::focusBarcodeLineEdit);
  focusTimer.start();

  installEventFilter(this);

  show();
}

MainWindow::~MainWindow()
{
  saveDatabase();
}

void MainWindow::createActions()
{
  quitAct = new QAction("&Quit", this);
  quitAct->setIcon(QIcon("graphics/quit.png"));
  connect(quitAct, &QAction::triggered, qApp, &QApplication::quit);

  aboutAct = new QAction(tr("&About..."), this);
  aboutAct->setIcon(QIcon("graphics/about.png"));
  connect(aboutAct, &QAction::triggered, this, &MainWindow::showAbout);
}

void MainWindow::createMenus()
{
  fileMenu = new QMenu(tr("&File"), this);
  fileMenu->addAction(quitAct);

  helpMenu = new QMenu(tr("&Help"), this);
  helpMenu->addAction(aboutAct);

  menuBar = new QMenuBar();
  menuBar->addMenu(fileMenu);
  menuBar->addMenu(helpMenu);

  setMenuBar(menuBar);
}

void MainWindow::createToolBar()
{
  barcodeLineEdit = new QLineEdit(this);
  barcodeLineEdit->setStyleSheet("QLineEdit {background-image: url(graphics/barcode_background.png);}");
  barcodeLineEdit->setMaximumHeight(data.iconSize);
  barcodeLineEdit->setMaximumWidth(400);
  barcodeLineEdit->setPlaceholderText(tr("Barcode"));
  connect(barcodeLineEdit, &QLineEdit::returnPressed, this, &MainWindow::checkBarcode);

  openCloseButton = new QPushButton(tr("The store is closed"), this);
  openCloseButton->setIcon(QIcon("graphics/quit.png"));
  openCloseButton->setFocusPolicy(Qt::NoFocus);
  openCloseButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  openCloseButton->setCheckable(true);
  connect(openCloseButton, &QPushButton::pressed, this, &MainWindow::openCloseStore);

  QToolBar *toolBar = new QToolBar(tr("Main functions"));
  toolBar->setMinimumHeight(data.iconSize + 20);
  toolBar->setStyleSheet("QToolBar {background-image: url(graphics/marquee.png); border: 0px;}");

  /*
  QPushButton *chooseWorkDirButton = new QPushButton();
  chooseWorkDirButton->setIcon(QIcon("graphics/choose_workdir.png"));
  chooseWorkDirButton->setIconSize(QSize(64, 64));
  chooseWorkDirButton->setToolTip(tr("Choose working directory"));
  
  //connect(chooseWorkDirButton, &QPushButton::clicked, this, &MainWindow::);
  
  toolBar->addWidget(chooseWorkDirButton);
  */
  auto spacerLeft = new QWidget(this);
  spacerLeft->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  auto spacerRight = new QWidget(this);
  spacerRight->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  toolBar->addWidget(spacerLeft);
  toolBar->addWidget(barcodeLineEdit);
  toolBar->addWidget(openCloseButton);
  toolBar->addWidget(spacerRight);
  //toolBar->addSeparator();

  addToolBar(Qt::TopToolBarArea, toolBar);
}

void MainWindow::loadIcons()
{
  QDir iconsDir("graphics/icons", "*.png", QDir::Name, QDir::Files);
  QList<QFileInfo> iconInfos = iconsDir.entryInfoList();
  for(const auto &iconInfo: iconInfos) {
    data.icons[iconInfo.baseName()] = QPixmap(iconInfo.absoluteFilePath()).scaled(data.iconSize, data.iconSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  }
}

void MainWindow::loadDatabase()
{
  if(!QFile::exists("database.dat") && QFile::exists("database.dat.example")) {
    printf("No database found, copying 'database.dat.example' to 'database.dat'\n");
    QFile::copy("database.dat.example", "database.dat");
  }
  QFile database("database.dat");
  int state = -1;
  if(database.open(QIODevice::ReadOnly)) {
    while(!database.atEnd()) {
      QString string = QString::fromUtf8(database.readLine().trimmed());
      if(string.isEmpty() ||
         string.left(1) == "#") {
        continue;
      } else if(string == "accounts:") {
        state = 0;
        continue;
      } else if(string == "items:") {
        state = 1;
        continue;
      } else if(string == "categories:") {
        state = 2;
        continue;
      }
      if(state == 0) {
        parseAccount(string);
      } else if(state == 1) {
        parseItem(string);
      } else if(state == 2) {
        parseCategory(string);
      }
    }
    database.close();
  }
}

void MainWindow::parseAccount(const QString &string)
{
  Account account;
  QMap<QString, QString> vars;
  for(const auto &snippet: string.split(";")) {
    if(snippet.contains("=")) {
      vars[snippet.split("=").first()] = snippet.split("=").last();
    }
  }
  if(vars.contains("barcode")) {
    account.barcode = vars["barcode"];
  }
  if(vars.contains("id")) {
    account.id = vars["id"];
  }
  if(vars.contains("balance")) {
    account.balance = vars["balance"].toDouble();
  }
  if(vars.contains("bonus")) {
    account.bonus = vars["bonus"].toInt();
  }

  printf("Account:\n  barcode=%s\n  id=%s\n  balance=%f\n  bonus=%d\n",
         qPrintable(account.barcode),
         qPrintable(account.id),
         account.balance,
         account.bonus);
  
  data.accounts.append(account);
}

void MainWindow::parseItem(const QString &string)
{
  Item item;
  QMap<QString, QString> vars;
  for(const auto &snippet: string.split(";")) {
    if(snippet.contains("=")) {
      vars[snippet.split("=").first()] = snippet.split("=").last();
    }
  }
  if(vars.contains("barcode")) {
    item.barcode = vars["barcode"];
  }
  if(vars.contains("id")) {
    item.id = vars["id"];
  }
  if(vars.contains("category")) {
    item.category = vars["category"];
  }
  if(vars.contains("price")) {
    item.price = vars["price"].toDouble();
  }
  if(vars.contains("discount")) {
    item.discount = vars["discount"].toDouble();
  }
  if(vars.contains("stock")) {
    item.stock = vars["stock"].toInt();
  }
  if(vars.contains("age")) {
    item.age = vars["age"].toInt();
  }
  if(vars.contains("icon")) {
    item.icon = vars["icon"];
  }

  printf("Item:\n  barcode=%s\n  id=%s\n  category=%s\n  price=%f\n  discount=%f\n  stock=%d\n  age=%d\n  icon=%s\n",
         qPrintable(item.barcode),
         qPrintable(item.id),
         qPrintable(item.category),
         item.price,
         item.discount,
         item.stock,
         item.age,
         qPrintable(item.icon));

  data.items.append(item);
}

void MainWindow::parseCategory(const QString &string)
{
  Category category;
  QMap<QString, QString> vars;
  for(const auto &snippet: string.split(";")) {
    if(snippet.contains("=")) {
      vars[snippet.split("=").first()] = snippet.split("=").last();
    }
  }
  if(vars.contains("barcode")) {
    category.barcode = vars["barcode"];
  }
  if(vars.contains("id")) {
    category.id = vars["id"];
  }
  if(vars.contains("icon")) {
    category.icon = vars["icon"];
  }
  if(vars.contains("lifespan")) {
    category.lifespan = vars["lifespan"].toInt();
  }
  printf("Category:\n  barcode=%s\n  id=%s\n  icon=%s\n  lifespan=%d\n",
         qPrintable(category.barcode),
         qPrintable(category.id),
         qPrintable(category.icon),
         category.lifespan);
  
  data.categories.append(category);
}

void MainWindow::saveDatabase()
{
  if(data.accounts.isEmpty() &&
     data.categories.isEmpty() &&
     data.items.isEmpty()) {
    return;
  }

  if(QFile::exists("database.dat")) {
    /*
    QString baseName = "backup/database0000";
    int serial = -1;
    do {
      serial++;
      baseName = baseName.left(baseName.length() - 4);
      QString serialString = QString::number(serial);
      while(serialString.length() < 4) {
        serialString.prepend("0");
      }
      baseName.append(serialString);
    } while(QFile::exists(baseName + ".dat"));
    */
    QDir::current().mkpath("backup");
    QFile::rename("database.dat", "backup/database" + QDateTime::currentDateTime().toString("yyyyMMdd-HHmmss") + ".dat");
  }
  QFile database("database.dat");
  if(database.open(QIODevice::WriteOnly)) {
    database.write("accounts:\n");
    for(const auto &account: data.accounts) {
      database.write(QString("barcode=" + account.barcode + ";id=" + account.id + ";balance=" + QString::number(account.balance) + ";bonus=" + QString::number(account.bonus) + "\n").toUtf8());
    }
    database.write("\n");
    database.write("items:\n");
    for(const auto &item: data.items) {
      database.write(QString("barcode=" + item.barcode + ";id=" + item.id + ";category=" + item.category + ";price=" + QString::number(item.price) + ";discount=" + QString::number(item.discount) + ";stock=" + QString::number(item.stock) + ";age=" + QString::number(item.age) + ";icon=" + item.icon + "\n").toUtf8());
    }
    database.write("\n");
    database.write("categories:\n");
    for(const auto &category: data.categories) {
      database.write(QString("barcode=" + category.barcode + ";id=" + category.id + ";icon=" + category.icon + ";lifespan=" + QString::number(category.lifespan) + "\n").toUtf8());
    }
    database.close();
  }
}

void MainWindow::showAbout()
{
  // Spawn about window
  AboutBox aboutBox(this);
  aboutBox.exec();
}

void MainWindow::checkBarcode()
{
  QString barcode = barcodeLineEdit->text().toLower();
  QString type = "";
  barcodeLineEdit->setText("");
  if(barcode.isEmpty()) {
    return;
  }

  printf("CHECKING BARCODE: '%s'\n", qPrintable(barcode));
  bool barcodeFound = false;
  for(const auto &account: data.accounts) {
    if(barcode == account.barcode) {
      printf("THIS IS AN ACCOUNT!\n");
      type = "account";
      barcodeFound = true;
    }
  }
  for(const auto &item: data.items) {
    if(barcode == item.barcode) {
      printf("THIS IS AN ITEM!\n");
      type = "item";
      barcodeFound = true;
    }
  }
  for(const auto &category: data.categories) {
    if(barcode == category.barcode) {
      printf("THIS IS A CATEGORY!\n");
      type = "category";
      barcodeFound = true;
    }
  }

  if(!barcodeFound) {
    printf("NEW BARCODE!\n");
    EntryEditor entryEditor(barcode, data, this);
    entryEditor.exec();
    if(entryEditor.result() == QDialog::Accepted) {
      if(entryEditor.getType() == "account") {
        accountsTab->accountsModel->beginInsertRow(data.accounts.length());
        entryEditor.addAccount();
        QSound::play("sounds/kundekort_registreret.wav");
        accountsTab->accountsModel->insertRows(data.accounts.length(), 1);
      } else if(entryEditor.getType() == "category") {
        categoriesTab->categoriesModel->beginInsertRow(data.categories.length());
        entryEditor.addCategory();
        QSound::play("sounds/kategori_registreret.wav");
        categoriesTab->categoriesModel->insertRows(data.categories.length(), 1);
      } else if(entryEditor.getType() == "item") {
        itemsTab->itemsModel->beginInsertRow(data.items.length());
        entryEditor.addItem();
        QSound::play("sounds/vare_registreret.wav");
        itemsTab->itemsModel->insertRows(data.items.length(), 1);
      }
    }
  }

  // Select relevant tab unless we are on checkout tab
  if(modeTabs->currentWidget() != checkoutTab) {
    if(type == "account") {
      modeTabs->setCurrentWidget(accountsTab);
    } else if(type == "category") {
      modeTabs->setCurrentWidget(categoriesTab);
    } else if(type == "item") {
      modeTabs->setCurrentWidget(itemsTab);
    }
    if(type == "account") {
      accountsTab->focusRow(barcode);
    } else if(type == "category") {
      categoriesTab->focusRow(barcode);
    } else if(type == "item") {
      itemsTab->addStock(barcode);
      itemsTab->focusRow(barcode);
    }
  } else {
    if(type == "account") {
      checkoutTab->payBy(barcode);
    } else if(type == "item") {
      checkoutTab->addItem(barcode);
    }
  }

  /*
  for(int a = 0; a < data.categories.length(); ++a) {
  if(data.categories.at(a).barcode == barcode) {
  categoriesTab->categoriesModel->refreshAll();
  }
  }
  */
}

QString MainWindow::getAccountFromBarcode(const QString &barcode)
{
  for(const auto &account: data.accounts) {
    if(account.barcode == barcode) {
      return account.id;
    }
  }
  return "";
}

QString MainWindow::getItemFromBarcode(const QString &barcode)
{
  for(const auto &item: data.items) {
    if(item.barcode == barcode) {
      return item.id;
    }
  }
  return "";
}

QString MainWindow::getCategoryFromBarcode(const QString &barcode)
{
  for(const auto &category: data.categories) {
    if(category.barcode == barcode) {
      return category.id;
    }
  }
  return "";
}

void MainWindow::initRandomSound()
{
  int fixedInterval = 6 * 60 * 1000;
  int randomInterval = 4 * 60 * 1000;
  randomTimer.setInterval(fixedInterval + QRandomGenerator::global()->bounded(randomInterval));
  if(!randomTimer.isActive()) {
    randomTimer.start();
    return;
  }
  playRandomSound();
}

void MainWindow::playRandomSound()
{
  QDir randomsDir("sounds/randoms", "*.wav", QDir::Name, QDir::Files);
  QList<QFileInfo> soundInfos = randomsDir.entryInfoList();
  if(soundInfos.isEmpty()) {
    return;
  }
  QSound::play(soundInfos.at(QRandomGenerator::global()->bounded(soundInfos.length())).absoluteFilePath());
}

/*
void MainWindow::focusItem(const QString &barcode)
{
  clearAll();
  barcodeLineEdit->setText(barcode);
  checkBarcode();
}
*/

void MainWindow::focusBarcodeLineEdit()
{
  barcodeLineEdit->setFocus();
}

void MainWindow::openCloseStore()
{
  // Checked state is opposite since we check it on 'pressed' to make barcode focus work
  if(openCloseButton->isChecked()) {
    openCloseButton->setText(tr("The store is closed"));
    openCloseButton->setToolTip(tr("Click to open the store"));
    if(!itemsTab->ageItems()) {
      QSound::play("sounds/butikken_er_nu_lukket.wav");
    } else {
      QSound::play("sounds/en_vare_er_for_gammel.wav");
    }
    printf("The store is now closed!\n");
  } else {
    openCloseButton->setText(tr("The store is open"));
    openCloseButton->setToolTip(tr("Click to close the store"));
    QSound::play("sounds/butikken_er_nu_aaben.wav");
    printf("The store is now open!\n");
  }
  focusTimer.start();
}

/* This DOES NOT work. The tabs get focus and eats the events even if I disable focus for them
bool MainWindow::eventFilter(QObject *, QEvent *event)
{
  if(event->type() == QEvent::KeyPress) {
    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
    if(keyEvent->modifiers() != 0) {
      return false;
    }
    barcodeLineEdit->setText(keyEvent->text());
    barcodeLineEdit->setFocus();    
    return true;
  }
  return false;
}
*/
