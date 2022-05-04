/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            accountstab.cpp
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

#include "accountstab.h"
#include "entryeditor.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QHeaderView>

AccountsTab::AccountsTab(QList<Account> &accounts,
                   QList<Item> &items,
                   QList<Category> &categories,
                   const QMap<QString, QIcon> &icons,
                   QWidget *parent)
  : QWidget(parent), accounts(accounts), items(items), categories(categories), icons(icons)
{
  setStyleSheet("QTableWidget {qproperty-iconSize: 35px; font-size: 30px;}"
                "QHeaderView {font-size: 30px;}"
                "QTableWidget QLabel {background-color: black;}");
  accountsList = new QTableWidget(this);
  accountsList->setEditTriggers(QAbstractItemView::NoEditTriggers);
  accountsList->verticalHeader()->setVisible(false);
  accountsList->setSelectionBehavior(QTableView::SelectRows);
  accountsList->setSelectionMode(QAbstractItemView::SingleSelection);
  //accountsList->setSortingEnabled(true);
  connect(accountsList, &QTableWidget::cellDoubleClicked, this, &AccountsTab::editAccount);
  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(accountsList);
  setLayout(layout);
  refreshAccounts();
}

AccountsTab::~AccountsTab()
{
}

void AccountsTab::refreshAccounts()
{
  printf("REFRESHING ACCOUNTS!\n");
  accountsList->clear();
  accountsList->setColumnCount(5);
  accountsList->setRowCount(accounts.length());

  accountsList->setHorizontalHeaderLabels({"",
      tr("Account"),
      tr("Balance"),
      tr("Bonus"),
      tr("Barcode")});
  for(int row = 0; row < accounts.length(); ++row) {

    QLabel *iconLabel = new QLabel();
    iconLabel->setAlignment(Qt::AlignHCenter);
    iconLabel->setPixmap(QIcon("graphics/account.png").pixmap(50, 50));
    accountsList->setCellWidget(row, 0, iconLabel);
    
    QTableWidgetItem *idItem = new QTableWidgetItem(accounts.at(row).id);
    idItem->setData(Qt::UserRole, accounts.at(row).barcode);
    accountsList->setItem(row, 1, idItem);

    QTableWidgetItem *balanceItem = new QTableWidgetItem();
    balanceItem->setData(Qt::DisplayRole, accounts.at(row).balance);
    balanceItem->setData(Qt::UserRole, accounts.at(row).barcode);
    accountsList->setItem(row, 2, balanceItem);

    QTableWidgetItem *bonusItem = new QTableWidgetItem();
    bonusItem->setData(Qt::DisplayRole, accounts.at(row).bonus);
    bonusItem->setData(Qt::UserRole, accounts.at(row).barcode);
    accountsList->setItem(row, 3, bonusItem);

    QTableWidgetItem *barcodeItem = new QTableWidgetItem(accounts.at(row).barcode);
    barcodeItem->setData(Qt::UserRole, accounts.at(row).barcode);
    accountsList->setItem(row, 4, barcodeItem);
  }
  accountsList->resizeColumnsToContents();
  accountsList->resizeRowsToContents();
}

void AccountsTab::editAccount(int row, int)
{
  EntryEditor entryEditor(accountsList->item(row, 1)->data(Qt::UserRole).toString(), accounts, items, categories, icons, this);
  entryEditor.exec();
  
  printf("EDITING ROW %d, BARCODE %s\n", row, qPrintable(accountsList->item(row, 1)->data(Qt::UserRole).toString()));
}
