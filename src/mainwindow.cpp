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

MainWindow::MainWindow()
{
  setWindowTitle("EZMarket v" VERSION);
  showFullScreen();

  setStyleSheet("QLabel {font-size: 30px;}"
                "QLineEdit {font-size: 30px;}"
                "QTabWidget {font-size: 30px;}"
                "QListWidget {font-size: 30px;}"
                "QGroupBox{font-size: 35px; padding-left: 20px; padding-top: 40px; padding-bottom: 20px; padding-right: 20px;}");
  createActions();
  createMenus();
  createToolBar();
  
  loadIcons();
  loadDatabase();
  
  accountsTab = new AccountsTab(accounts, items, categories, icons, this);
  itemsTab = new ItemsTab(items, categories, icons, this);
  categoriesTab = new CategoriesTab(categories, this);
 /*
  CheckoutTab *checkoutTab = new CheckoutTab(this);
  */
  
  modeTabs = new QTabWidget(this);
  modeTabs->addTab(accountsTab, tr("Accounts"));
  modeTabs->addTab(itemsTab, tr("Items"));
  modeTabs->addTab(categoriesTab, tr("Categories"));
  /*
  modeTabs->addTab(checkoutTab, tr("Checkout"));
  */
  
  QVBoxLayout *layout = new QVBoxLayout();
  layout->addWidget(modeTabs);

  setCentralWidget(new QWidget());
  centralWidget()->setLayout(layout);

  randomTimer.setSingleShot(false);
  connect(&randomTimer, &QTimer::timeout, this, &MainWindow::initRandomSound);
  initRandomSound();

  /*
  clearTimer.setInterval(5000);
  clearTimer.setSingleShot(true);
  connect(&clearTimer, &QTimer::timeout, this, &MainWindow::clearAll);
  */
  focusTimer.setInterval(500);
  focusTimer.setSingleShot(false);
  connect(&focusTimer, &QTimer::timeout, this, &MainWindow::focusBarcodeLineEdit);
  focusTimer.start();

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
  barcodeLineEdit->setMaximumHeight(48);
  barcodeLineEdit->setMaximumWidth(300);
  barcodeLineEdit->setPlaceholderText(tr("Barcode"));
  connect(barcodeLineEdit, &QLineEdit::returnPressed, this, &MainWindow::checkBarcode);

  /*
  QPushButton *chooseWorkDirButton = new QPushButton();
  chooseWorkDirButton->setIcon(QIcon("graphics/choose_workdir.png"));
  chooseWorkDirButton->setIconSize(QSize(64, 64));
  chooseWorkDirButton->setToolTip(tr("Choose working directory"));
  */

  QToolBar *toolBar = new QToolBar(tr("Main functions"));
  toolBar->setMinimumHeight(64);
  toolBar->setStyleSheet("QToolBar {background-image: url(graphics/marquee.png); border: 0px;}");

  /*
  QPushButton *chooseWorkDirButton = new QPushButton();
  chooseWorkDirButton->setIcon(QIcon("graphics/choose_workdir.png"));
  chooseWorkDirButton->setIconSize(QSize(64, 64));
  chooseWorkDirButton->setToolTip(tr("Choose working directory"));
  
  //connect(chooseWorkDirButton, &QPushButton::clicked, this, &MainWindow::);
  
  toolBar->addWidget(chooseWorkDirButton);
  */
  toolBar->addWidget(barcodeLineEdit);
  toolBar->addSeparator();

  addToolBar(Qt::TopToolBarArea, toolBar);
}

void MainWindow::loadIcons()
{
  QDir iconsDir("graphics/icons", "*.png", QDir::Name, QDir::Files);
  QList<QFileInfo> iconInfos = iconsDir.entryInfoList();
  for(const auto &iconInfo: iconInfos) {
    icons[iconInfo.baseName()] = QIcon(iconInfo.absoluteFilePath());
  }
}

void MainWindow::loadDatabase()
{
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
  
  accounts.append(account);
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

  items.append(item);
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
  printf("Category:\n  barcode=%s\n  id=%s\n  icon=%s\n",
         qPrintable(category.barcode),
         qPrintable(category.id),
         qPrintable(category.icon));
  
  categories.append(category);
}

void MainWindow::saveDatabase()
{
  if(accounts.isEmpty() && items.isEmpty()) {
    return;
  }

  if(QFile::exists("database.dat")) {
    QString baseName = "database0000";
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
    QFile::rename("database.dat", baseName + ".dat");
  }
  QFile database("database.dat");
  if(database.open(QIODevice::WriteOnly)) {
    database.write("accounts:\n");
    for(const auto &account: accounts) {
      database.write(QString("barcode=" + account.barcode + ";id=" + account.id + ";balance=" + QString::number(account.balance) + ";bonus=" + QString::number(account.bonus) + "\n").toUtf8());
    }
    database.write("\n");
    database.write("items:\n");
    for(const auto &item: items) {
      database.write(QString("barcode=" + item.barcode + ";id=" + item.id + ";category=" + item.category + ";price=" + QString::number(item.price) + ";discount=" + QString::number(item.discount) + ";stock=" + QString::number(item.stock) + ";age=" + QString::number(item.age) + ";icon=" + item.icon + "\n").toUtf8());
    }
    database.write("\n");
    database.write("categories:\n");
    for(const auto &category: categories) {
      database.write(QString("barcode=" + category.barcode + ";id=" + category.id + ";icon=" + category.icon + "\n").toUtf8());
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
  for(const auto &account: accounts) {
    if(barcode == account.barcode) {
      printf("THIS IS AN ACCOUNT!\n");
      type = "account";
      barcodeFound = true;
    }
  }
  for(const auto &item: items) {
    if(barcode == item.barcode) {
      printf("THIS IS AN ITEM!\n");
      type = "item";
      barcodeFound = true;
    }
  }

  if(!barcodeFound) {
    printf("NEW BARCODE!\n");
    EntryEditor entryEditor(barcode, accounts, items, categories, icons, this);
    entryEditor.exec();
    type = entryEditor.getType();
    if(type == "account") {
      std::sort(accounts.begin(), accounts.end(), [](const Account a, const Account b) -> bool { return a.id.toLower() < b.id.toLower(); });
      accountsTab->refreshAccounts();
    } else if(type == "item") {
      std::sort(items.begin(), items.end(), [](const Item a, const Item b) -> bool { return a.id.toLower() < b.id.toLower(); });
      //itemsTab->refreshItems();
    }
  }

  if(modeTabs->currentWidget() == accountsTab) {
    printf("ACCOUNTS TAB ACTIVE!\n");
    if(type == "account") {
    } else if(type == "item") {
      modeTabs->setCurrentWidget(itemsTab);
    }
  } else if(modeTabs->currentWidget() == itemsTab) {
    printf("ITEMS TAB ACTIVE!\n");
    if(type == "account") {
      modeTabs->setCurrentWidget(accountsTab);
    } else if(type == "item") {
    }
  } else if(modeTabs->currentWidget() == categoriesTab) {
    printf("CATEGORIES TAB ACTIVE!\n");
  }
  
  /*  
  for(const auto &account: accounts) {
    if(barcode == account.barcode) {
      printf("THIS IS A ACCOUNT!\n");
      barcodeFound = true;
      accountBarcodeLineEdit->setText(account.barcode);
      accountNameLineEdit->setText(account.id);
      accountOnLoanList->clear();
      accountReservedList->clear();
      clearItem();
      for(const auto &item: items) {
        if(item.loanedBy == account.barcode) {
          accountOnLoanList->addItem(item.id);
        } else if(item.reservedBy == account.barcode) {
          accountReservedList->addItem(item.id);
        }
      }
    }
  }
  for(auto &item: items) {
    if(barcode == item.barcode) {
      printf("THIS IS A ITEM!\n");
      barcodeFound = true;
      itemBarcodeLineEdit->setText(item.barcode);
      itemIdLineEdit->setText(item.id);
      itemLoanedByLineEdit->setText(getAccountFromBarcode(item.loanedBy)); 
      itemReservedByLineEdit->setText(getAccountFromBarcode(item.reservedBy)); 
      if(item.id.isEmpty()) {
        printf("MISSING ID, NEW ITEM?\n");
        break;
      }
      if(!accountBarcodeLineEdit->text().isEmpty()) {
        for(const auto &account: accounts) {
          if(accountBarcodeLineEdit->text() == account.barcode) {

            if(item.reservedBy.isEmpty() && item.loanedBy.isEmpty()) {
              QSound::play("sounds/laant.wav");
              printf("ITEM LOANED!\n");
              item.loanedBy = account.barcode;

            } else if(!item.reservedBy.isEmpty() && item.loanedBy.isEmpty()) {
              if(account.barcode == item.reservedBy) {
                QSound::play("sounds/laant.wav");
                printf("ITEM LOANED! HAD IT RESERVED!\n");
                item.reservedBy.clear();
                item.loanedBy = account.barcode;
              } else {
                QSound::play("sounds/reserveret_til_anden.wav");
                printf("RESERVED BY OTHER!\n");
              }

            } else if(!item.reservedBy.isEmpty() && !item.loanedBy.isEmpty()) {
              if(account.barcode == item.loanedBy) {
                QSound::play("sounds/afleveret-reserveret_til_anden.wav");
                printf("ITEM RETURNED! RESERVED BY OTHER!\n");
                item.loanedBy.clear();
              } else if(account.barcode == item.reservedBy) {
                QSound::play("sounds/udlaant-allerede_reserveret_til_dig.wav");
                printf("ON LOAN, ALREADY RESERVED!\n");
              }

            } else if(item.reservedBy.isEmpty() && !item.loanedBy.isEmpty()) {
              if(item.loanedBy != account.barcode) {
                QSound::play("sounds/udlaant-reserveret_til_dig.wav");
                printf("ITEM RESERVED!\n");
                item.reservedBy = account.barcode;
              } else if(item.loanedBy == account.barcode) {
                QSound::play("sounds/afleveret.wav");
                printf("ITEM RETURNED!\n");
                item.loanedBy.clear();
              }
            }

          }
        }
      }
    }
  }
  if(!barcodeFound) {
    QSound::play("sounds/ny_account_eller_bog.wav");
    printf("NEW BARCODE!\n");
    EntryEditor entryEditor(barcode, this);
    entryEditor.exec();
    if(entryEditor.result() == QDialog::Accepted) {
      if(entryEditor.getType() == "account") {
        QSound::play("sounds/account_registreret.wav");
        printf("ADDING NEW ACCOUNT!\n");
        Account account;
        account.barcode = barcode;
        account.id = entryEditor.getNameTitle();
        accounts.append(account);
      } else if(entryEditor.getType() == "item") {
        QSound::play("sounds/bog_registreret.wav");
        printf("ADDING NEW ITEM!\n");
        Item item;
        item.barcode = barcode;
        item.id = entryEditor.getNameTitle();
        items.append(item);
        std::sort(items.begin(), items.end(), [](const Item a, const Item b) -> bool { return a.id.toLower() < b.id.toLower(); });

      }
      clearAll();
      barcodeLineEdit->setText(barcode);
      checkBarcode();
    }
  } else {
    clearTimer.start();
  }
  */
}

QString MainWindow::getAccountFromBarcode(const QString &barcode)
{
  for(const auto &account: accounts) {
    if(account.barcode == barcode) {
      return account.id;
    }
  }
  return "";
}

QString MainWindow::getItemFromBarcode(const QString &barcode)
{
  for(const auto &item: items) {
    if(item.barcode == barcode) {
      return item.id;
    }
  }
  return "";
}

QString MainWindow::getCategoryFromBarcode(const QString &barcode)
{
  for(const auto &category: categories) {
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
