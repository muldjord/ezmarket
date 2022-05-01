/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            newentry.cpp
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

#include "newentry.h"

#include <QPushButton>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QLabel>

NewEntry::NewEntry(const QString &barcode,
                   QList<Account> &accounts,
                   QList<Item> &items, const QList<Category> &categories,
                   QWidget *parent)
  : QDialog(parent), accounts(accounts), items(items), categories(categories)
{
  setWindowTitle(tr("Barcode: ") + barcode);
  setFixedSize(450, 500);

  setStyleSheet("QLabel {font-size: 40px; qproperty-alignment: AlignCenter;}"
                "QComboBox {font-size: 25px;}"
                "QPushButton {qproperty-iconSize: 40px; font-size: 40px;}");
  
  QLabel *typeLabel = new QLabel(tr("New account or item?"));

  QPushButton *accountButton = new QPushButton(tr("Account"));
  accountButton->setIcon(QIcon("graphics/account.png"));
  accountButton->setCheckable(true);
  accountButton->setObjectName("account");
  accountButton->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);

  QPushButton *itemButton = new QPushButton(tr("Item"));
  itemButton->setIcon(QIcon("graphics/item.png"));
  itemButton->setCheckable(true);
  itemButton->setObjectName("item");
  itemButton->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);

  typeGroup = new QButtonGroup(this);
  typeGroup->addButton(accountButton);
  typeGroup->addButton(itemButton);
  typeGroup->setExclusive(true);
  connect(typeGroup, qOverload<QAbstractButton *>(&QButtonGroup::buttonReleased), this, &NewEntry::typeChanged);

  QHBoxLayout *buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(accountButton);
  buttonLayout->addWidget(itemButton);

  accountWidget = new AccountWidget(barcode, this);
  itemWidget = new ItemWidget(barcode, categories, this);

  typeLayout = new QStackedLayout;
  typeLayout->addWidget(new QWidget);
  typeLayout->addWidget(accountWidget);
  typeLayout->addWidget(itemWidget);

  QDialogButtonBox *dialogButtons = new QDialogButtonBox(QDialogButtonBox::Save |
                                                         QDialogButtonBox::Cancel);
  connect(dialogButtons, &QDialogButtonBox::accepted, this, &NewEntry::checkSanity);
  connect(dialogButtons, &QDialogButtonBox::rejected, this, &NewEntry::reject);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(typeLabel);
  layout->addLayout(buttonLayout);
  layout->addLayout(typeLayout);
  layout->addWidget(dialogButtons);
  
  setLayout(layout);
}

void NewEntry::typeChanged(QAbstractButton *button)
{
  if(button->objectName() == "account") {
    typeLayout->setCurrentWidget(accountWidget);
    accountWidget->setFocus();
  } else if(button->objectName() == "item") {
    typeLayout->setCurrentWidget(itemWidget);
    itemWidget->setFocus();
  }
}

void NewEntry::checkSanity()
{
  if(typeLayout->currentWidget() == accountWidget) {
    if(accountWidget->isSane()) {
      accounts.append(accountWidget->getAccount());
    } else {
      return;
    }
  } else if(typeLayout->currentWidget() == itemWidget) {
    if(itemWidget->isSane()) {
      items.append(itemWidget->getItem());
    } else {
      return;
    }
  }
  accept();
}
