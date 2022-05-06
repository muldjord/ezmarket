/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            entryeditor.cpp
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

#include "entryeditor.h"

#include <QPushButton>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QLabel>
#include <QSound>

EntryEditor::EntryEditor(const QString &barcode,
                         QList<Account> &accounts,
                         QList<Item> &items,
                         QList<Category> &categories,
                         const QMap<QString, QIcon> &icons,
                         QWidget *parent)
  : QDialog(parent), barcode(barcode), accounts(accounts), items(items), categories(categories)
{
  setWindowTitle(tr("Barcode: ") + barcode);
  setFixedSize(450, 700);

  setStyleSheet("QLabel {font-size: 35px; qproperty-alignment: AlignCenter;}"
                "QLineEdit {font-size: 35px;}"
                "QComboBox {qproperty-iconSize: 35px; font-size: 35px;}"
                "QPushButton {qproperty-iconSize: 35px; font-size: 35px;}");
  
  for(const auto &account: accounts) {
    if(barcode == account.barcode) {
      entryType = "account";
      break;
    }
  }
  for(const auto &item: items) {
    if(barcode == item.barcode) {
      entryType = "item";
      break;
    }
  }
  for(const auto &category: categories) {
    if(barcode == category.barcode) {
      entryType = "category";
      break;
    }
  }
  if(entryType.isEmpty()) {
    QSound::play("sounds/ny_konto_eller_vare.wav");
  }
  
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
  connect(typeGroup, qOverload<QAbstractButton *>(&QButtonGroup::buttonReleased), this, &EntryEditor::typeChanged);

  QHBoxLayout *buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(accountButton);
  buttonLayout->addWidget(itemButton);

  accountWidget = new AccountWidget(barcode, accounts, this);
  itemWidget = new ItemWidget(barcode, items, icons, categories, this);

  typeLayout = new QStackedLayout;
  if(entryType.isEmpty()) {
    typeLayout->addWidget(new QWidget);
    typeLayout->addWidget(accountWidget);
    typeLayout->addWidget(itemWidget);
  } else if(entryType == "account") {
    typeLayout->addWidget(accountWidget);
  } else if(entryType == "item") {
    typeLayout->addWidget(itemWidget);
  } else if(entryType == "category") {
    typeLayout->addWidget(itemWidget);
  }

  QDialogButtonBox *dialogButtons = new QDialogButtonBox(QDialogButtonBox::Save |
                                                         QDialogButtonBox::Cancel);
  connect(dialogButtons, &QDialogButtonBox::accepted, this, &EntryEditor::checkSanity);
  connect(dialogButtons, &QDialogButtonBox::rejected, this, &EntryEditor::reject);

  QVBoxLayout *layout = new QVBoxLayout;
  if(entryType.isEmpty()) {
    layout->addWidget(typeLabel);
    layout->addLayout(buttonLayout);
  }
  layout->addLayout(typeLayout);
  layout->addWidget(dialogButtons);
  
  setLayout(layout);
}

void EntryEditor::typeChanged(QAbstractButton *button)
{
  if(button->objectName() == "account") {
    typeLayout->setCurrentWidget(accountWidget);
    accountWidget->setFocus();
  } else if(button->objectName() == "item") {
    typeLayout->setCurrentWidget(itemWidget);
    itemWidget->setFocus();
  }
}

void EntryEditor::checkSanity()
{
  if(entryType.isEmpty()) {
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
  } else {
    if(entryType == "account") {
      if(accountWidget->isSane()) {
        for(auto &account: accounts) {
          if(barcode == account.barcode) {
            account = accountWidget->getAccount();
            break;
          }
        }
      } else {
        return;
      }
    } else if(entryType == "item") {
      if(itemWidget->isSane()) {
        for(auto &item: items) {
          if(barcode == item.barcode) {
            item = itemWidget->getItem();
            break;
          }
        }
      } else {
        return;
      }
    }
  }
  accept();
}

QString EntryEditor::getType()
{
  if(typeLayout->currentWidget() == accountWidget) {
    return "account";
  } else if(typeLayout->currentWidget() == itemWidget) {
    return "item";
  }
  return "";
}