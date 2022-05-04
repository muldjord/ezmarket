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
#include <QLocale>

ItemWidget::ItemWidget(const QString &barcode,
                       const QList<Item> &items,
                       const QMap<QString, QIcon> &icons,
                       const QList<Category> &categories,
                       QWidget *parent)
  : QWidget(parent), barcode(barcode), items(items), icons(icons)
{
  QLabel *idLabel = new QLabel(tr("Item name:"));
  idLineEdit = new LineEdit(this);
  setFocusProxy(idLineEdit);

  QLabel *categoryLabel = new QLabel(tr("Category:"));
  categoryComboBox = new QComboBox(this);
  for(const auto &category: categories) {
    categoryComboBox->addItem(icons[category.icon], category.id, category.barcode);
  }
  
  QLabel *iconLabel = new QLabel(tr("Icon:"));
  searchLineEdit = new LineEdit(this);
  searchLineEdit->setPlaceholderText(tr("Icon search"));
  connect(searchLineEdit, &LineEdit::textChanged, this, &ItemWidget::searchIcons);
  iconComboBox = new QComboBox(this);
  searchIcons();
  
  QLabel *priceLabel = new QLabel(tr("Price:"));
  priceLineEdit = new LineEdit(this);
  priceLineEdit->setText("0.0");

  QLabel *discountLabel = new QLabel(tr("Discount:"));
  discountLineEdit = new LineEdit(this);
  discountLineEdit->setText("0.0");

  for(const auto &item: items) {
    if(barcode == item.barcode) {
      idLineEdit->setText(item.id);
      printf("FOUND AT %d\n", categoryComboBox->findData(item.category));
      categoryComboBox->setCurrentIndex(categoryComboBox->findData(item.category));
      iconComboBox->setCurrentIndex(iconComboBox->findData(item.icon));
      priceLineEdit->setText(QLocale().toString(item.price));
      discountLineEdit->setText(QLocale().toString(item.discount));
      break;
    }
  }
  connect(idLineEdit, &LineEdit::textChanged, this, &ItemWidget::setIconSearchText);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->addWidget(idLabel);
  layout->addWidget(idLineEdit);
  layout->addWidget(categoryLabel);
  layout->addWidget(categoryComboBox);
  layout->addWidget(iconLabel);
  layout->addWidget(searchLineEdit);
  layout->addWidget(iconComboBox);
  layout->addWidget(priceLabel);
  layout->addWidget(priceLineEdit);
  layout->addWidget(discountLabel);
  layout->addWidget(discountLineEdit);

  setLayout(layout);
}

ItemWidget::~ItemWidget()
{
}

bool ItemWidget::isSane()
{
  if(!idLineEdit->text().isEmpty() &&
     !categoryComboBox->currentData().toString().isEmpty() &&
     !priceLineEdit->text().isEmpty()) {
    return true;
  }
  return false;
}

Item ItemWidget::getItem()
{
  Item item;

  item.barcode = this->barcode;
  item.id = idLineEdit->text();
  item.category = categoryComboBox->currentData().toString();
  item.icon = iconComboBox->currentData().toString();
  item.price = QLocale().toDouble(priceLineEdit->text());
  item.discount = QLocale().toDouble(discountLineEdit->text());

  return item;
}

void ItemWidget::setIconSearchText()
{
  searchLineEdit->setText(idLineEdit->text());
}

void ItemWidget::searchIcons()
{
  QList<QString> snippets = searchLineEdit->text().toLower().split(" ");
  iconComboBox->clear();
  for(const auto &key: icons.keys()) {
    QString tmpKey = key;
    tmpKey.replace("_", " ").replace("ae", "æ").replace("oe", "ø").replace("aa", "å");
    tmpKey = tmpKey.left(1).toUpper() + tmpKey.mid(1);
    for(const auto &snippet: snippets) {
      if(tmpKey.toLower().contains(snippet)) {
        iconComboBox->addItem(icons[key], tmpKey, key);
        break;
      }
    }
  }
  iconComboBox->model()->sort(0, Qt::AscendingOrder);
  iconComboBox->addItem(tr("None"), "");
}
