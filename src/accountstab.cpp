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

#include <stdio.h>

#include "accountstab.h"

AccountsTab::AccountsTab(const QList<Account> &accounts, QWidget *parent)
  : QListWidget(parent), accounts(accounts)
{
  refreshTimer.setInterval(1000);
  refreshTimer.setSingleShot(true);
  connect(&refreshTimer, &QTimer::timeout, this, &AccountsTab::refreshAccounts);
  refreshTimer.start();

  connect(this, &AccountsTab::itemPressed, this, &AccountsTab::accountSelected);
}

AccountsTab::~AccountsTab()
{
}

void AccountsTab::refreshAccounts()
{
  printf("REFRESHING ACCOUNTS!\n");
  clear();

  for(auto &account: accounts) {
    /*
    if(!account.loanedBy.isEmpty()) {
      account.loanedTimer += elapsedTime.elapsed();
      printf("Account: %s, loaned time: %d\n", qPrintable(account.title), account.loanedTimer);
    } else {
      account.loanedTimer = 0.0;
    }
    */
    QListWidgetItem *item = new QListWidgetItem;
    item->setText(account.id + " (" + account.barcode + ")");
    /*
    if(account.loanedTimer > 240 * 1000) {
      item->setForeground(Qt::red);
    } else if(!account.reservedBy.isEmpty()) {
      item->setForeground(Qt::blue);
    } else if(!account.loanedBy.isEmpty()) {
      item->setForeground(Qt::gray);
    }
    */
    item->setData(Qt::UserRole, account.barcode);
    addItem(item);
  }
  //elapsedTime.restart();
  refreshTimer.start();
}

void AccountsTab::accountSelected(QListWidgetItem *item)
{
  //emit focusAccount(item->data(Qt::UserRole).toString());
  refreshTimer.start();
}
