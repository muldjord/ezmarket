/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            itemwidget.cpp
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

#include "itemwidget.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLocale>

ItemWidget::ItemWidget(Data &data,
                       Item &item,
                       QWidget *parent)
  : QWidget(parent), data(data), item(item)
{
  QLabel *idLabel = new QLabel(tr("Item name:"));
  idLineEdit = new LineEdit(data, this);
  setFocusProxy(idLineEdit);

  QLabel *categoryLabel = new QLabel(tr("Category:"));
  categoryComboBox = new QComboBox(this);
  for(const auto &category: data.categories) {
    categoryComboBox->addItem(data.icons[category.icon], category.id, category.barcode);
  }
  
  QLabel *iconLabel = new QLabel(tr("Icon:"));
  searchLineEdit = new LineEdit(data, this);
  searchLineEdit->setPlaceholderText(tr("Icon search"));
  connect(searchLineEdit, &LineEdit::textChanged, this, &ItemWidget::searchIcons);
  iconComboBox = new QComboBox(this);
  searchIcons();
  
  QLabel *priceLabel = new QLabel(tr("Price:"));
  priceLineEdit = new LineEdit(data, this);

  QLabel *discountLabel = new QLabel(tr("Discount:"));
  discountLineEdit = new LineEdit(data, this);

  QLabel *stockLabel = new QLabel(tr("In stock:"));
  stockSpinBox = new SpinBox(this);
  
  idLineEdit->setText(item.id);
  categoryComboBox->setCurrentIndex(categoryComboBox->findData(item.category));
  iconComboBox->setCurrentIndex(iconComboBox->findData(item.icon));
  priceLineEdit->setText(QLocale().toString(item.price, 'f', 2));
  discountLineEdit->setText(QLocale().toString(item.discount, 'f', 2));
  stockSpinBox->setValue(item.stock);
  //connect(idLineEdit, &LineEdit::textChanged, this, &ItemWidget::setIconSearchText);

  QGridLayout *hLayout = new QGridLayout;
  hLayout->addWidget(priceLabel, 0, 0);
  hLayout->addWidget(priceLineEdit, 1, 0);
  hLayout->addWidget(discountLabel, 0, 1);
  hLayout->addWidget(discountLineEdit, 1, 1);
  hLayout->addWidget(stockLabel, 0, 2);
  hLayout->addWidget(stockSpinBox, 1, 2);
  hLayout->setColumnStretch(0, 1);
  hLayout->setColumnStretch(1, 1);
  hLayout->setColumnStretch(2, 1);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->addWidget(idLabel);
  layout->addWidget(idLineEdit);
  layout->addWidget(categoryLabel);
  layout->addWidget(categoryComboBox);
  layout->addWidget(iconLabel);
  layout->addWidget(searchLineEdit);
  layout->addWidget(iconComboBox);
  layout->addLayout(hLayout);
  layout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Ignored, QSizePolicy::Expanding));

  setLayout(layout);
}

ItemWidget::~ItemWidget()
{
}

bool ItemWidget::isSane()
{
  if(!idLineEdit->text().isEmpty() &&
     !categoryComboBox->currentData().toString().isEmpty() &&
     !priceLineEdit->text().isEmpty() &&
     stockSpinBox->value() >= 0) {
    return true;
  }
  return false;
}

void ItemWidget::setIconSearchText()
{
  searchLineEdit->setText(idLineEdit->text());
}

void ItemWidget::searchIcons()
{
  QList<QString> snippets = searchLineEdit->text().toLower().split(" ");
  iconComboBox->clear();
  for(const auto &key: data.icons.keys()) {
    QString tmpKey = key;
    tmpKey.replace("_", " ").replace("ae", "æ").replace("oe", "ø").replace("aa", "å");
    tmpKey = tmpKey.left(1).toUpper() + tmpKey.mid(1);
    for(const auto &snippet: snippets) {
      if(tmpKey.toLower().contains(snippet)) {
        iconComboBox->addItem(data.icons[key], tmpKey, key);
        break;
      }
    }
  }
  iconComboBox->model()->sort(0, Qt::AscendingOrder);
  iconComboBox->addItem(tr("None"), "");
}

void ItemWidget::commitItem()
{
  item.id = idLineEdit->text();
  item.category = categoryComboBox->currentData().toString();
  item.icon = iconComboBox->currentData().toString();
  item.price = QLocale().toDouble(priceLineEdit->text());
  item.discount = QLocale().toDouble(discountLineEdit->text());
  if(item.stock < stockSpinBox->value()) {
    item.age = 0;
  }
  item.stock = stockSpinBox->value();
}

void ItemWidget::removeItem()
{
  for(int a = 0; a < data.items.count(); ++a) {
    if(item.barcode == data.items.at(a).barcode) {
      data.items.removeAt(a);
      break;
    }
  }
}
