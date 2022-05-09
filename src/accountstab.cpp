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
#include "accounteditor.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QHeaderView>

AccountsTab::AccountsTab(Data &data,
                   QWidget *parent)
  : QWidget(parent), data(data)
{
  setStyleSheet("QTableView {font-size: 30px;}"
                "QHeaderView {font-size: 30px;}");
  accountsView = new QTableView(this);
  accountsModel = new AccountsModel(data, this);

  proxyModel = new QSortFilterProxyModel(this);
  proxyModel->setSourceModel(accountsModel);

  accountsView->setModel(proxyModel);
  accountsView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  accountsView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
  accountsView->verticalHeader()->setMinimumSectionSize(data.iconSize + 6);
  accountsView->verticalHeader()->setMaximumSectionSize(data.iconSize + 6);
  accountsView->setSelectionBehavior(QTableView::SelectRows);
  accountsView->setSelectionMode(QAbstractItemView::SingleSelection);
  accountsView->verticalHeader()->setVisible(false);
  accountsView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  accountsView->setSortingEnabled(true);
  connect(accountsView, &QTableView::doubleClicked, this, &AccountsTab::editAccount);
  /*
  QPusbButton *addButton = new QPushButton(tr("Add account"));
  connect(addButton, &QPushButton::clicked, this, &AccountsTab::addAccount);

  QHBoxLayout *functionLayout = new QHBoxLayout;
  functionsLayout->addWidget(addButton);
  */
  
  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(accountsView);
  setLayout(layout);
}

AccountsTab::~AccountsTab()
{
}

void AccountsTab::editAccount(const QModelIndex &index)
{
  AccountEditor accountEditor(data.accounts.at(proxyModel->mapToSource(index).row()).barcode, data, this);
  accountEditor.exec();
}

/*
void AccountsTab::addAccount()
{
  AccountEditor accountEditor(data.accounts.at(proxyModel->mapToSource(index).row()).barcode, data, this);
  accountEditor.exec();
}
*/
