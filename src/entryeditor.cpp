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
                         Data &data,
                         QWidget *parent)
  : QDialog(parent), barcode(barcode), data(data)
{
  setWindowTitle(tr("Barcode: ") + barcode);
  setFixedSize(550, 750);

  setStyleSheet("QLabel {font-size: 35px; qproperty-alignment: AlignCenter;}"
                "QLineEdit {font-size: 35px;}"
                "QComboBox {qproperty-iconSize: 35px; font-size: 35px;}"
                "QPushButton {qproperty-iconSize: 35px; font-size: 35px;}");
  
  QSound::play("sounds/ny_konto_eller_vare.wav");
  
  QLabel *typeLabel = new QLabel(tr("New account, category or item?"));

  accountButton = new QPushButton(tr("Account"));
  accountButton->setIcon(QIcon("graphics/account.png"));
  accountButton->setCheckable(true);
  accountButton->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);

  categoryButton = new QPushButton(tr("Category"));
  categoryButton->setIcon(QIcon("graphics/category.png"));
  categoryButton->setCheckable(true);
  categoryButton->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);

  itemButton = new QPushButton(tr("Item"));
  itemButton->setIcon(QIcon("graphics/item.png"));
  itemButton->setCheckable(true);
  itemButton->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);
  
  typeGroup = new QButtonGroup(this);
  typeGroup->addButton(accountButton);
  typeGroup->addButton(categoryButton);
  typeGroup->addButton(itemButton);
  typeGroup->setExclusive(true);
  connect(typeGroup, qOverload<QAbstractButton *>(&QButtonGroup::buttonReleased), this, &EntryEditor::typeChanged);

  QHBoxLayout *buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(accountButton);
  buttonLayout->addWidget(categoryButton);
  buttonLayout->addWidget(itemButton);

  accountWidget = new AccountWidget(barcode, data, this);
  itemWidget = new ItemWidget(barcode, data, this);
  categoryWidget = new CategoryWidget(barcode, data, this);

  typeLayout = new QStackedLayout;
  typeLayout->addWidget(new QWidget);
  typeLayout->addWidget(accountWidget);
  typeLayout->addWidget(categoryWidget);
  typeLayout->addWidget(itemWidget);
  
  QDialogButtonBox *dialogButtons = new QDialogButtonBox(QDialogButtonBox::Save |
                                                         QDialogButtonBox::Cancel);
  connect(dialogButtons, &QDialogButtonBox::accepted, this, &EntryEditor::checkSanity);
  connect(dialogButtons, &QDialogButtonBox::rejected, this, &EntryEditor::reject);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(typeLabel);
  layout->addLayout(buttonLayout);
  layout->addLayout(typeLayout);
  layout->addWidget(dialogButtons);
  
  setLayout(layout);
}

void EntryEditor::typeChanged(QAbstractButton *button)
{
  if(button == accountButton) {
    typeLayout->setCurrentWidget(accountWidget);
    accountWidget->setFocus();
  } else if(button == itemButton) {
    typeLayout->setCurrentWidget(itemWidget);
    itemWidget->setFocus();
  } else if(button == categoryButton) {
    typeLayout->setCurrentWidget(categoryWidget);
    categoryWidget->setFocus();
  }
}

void EntryEditor::checkSanity()
{
  if(typeLayout->currentWidget() == accountWidget) {
    if(!accountWidget->isSane()) {
      return;
    }
  } else if(typeLayout->currentWidget() == itemWidget) {
    if(!itemWidget->isSane()) {
      return;
    }
  } else if(typeLayout->currentWidget() == categoryWidget) {
    if(!categoryWidget->isSane()) {
      return;
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
  } else if(typeLayout->currentWidget() == categoryWidget) {
    return "category";
  }
  return "";
}

Account EntryEditor::getAccount()
{
  return accountWidget->getAccount();
}

Item EntryEditor::getItem()
{
  return itemWidget->getItem();
}

Category EntryEditor::getCategory()
{
  return categoryWidget->getCategory();
}
