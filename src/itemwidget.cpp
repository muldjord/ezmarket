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

ItemWidget::ItemWidget(const QString &barcode, const QList<Category> &categories, QWidget *parent)
  : QWidget(parent), barcode(barcode)
{
  QLabel *idLabel = new QLabel(tr("Item name:"));
  idLineEdit = new LineEdit(this);
  setFocusProxy(idLineEdit);

  QLabel *categoryLabel = new QLabel(tr("Category:"));
  categoryComboBox = new QComboBox(this);
  for(const auto &category: categories) {
    categoryComboBox->addItem(category.id, category.barcode);
  }

  QLabel *priceLabel = new QLabel(tr("Price:"));
  priceLineEdit = new LineEdit(this);
  priceLineEdit->setText("0.0");

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->addWidget(idLabel);
  layout->addWidget(idLineEdit);
  layout->addWidget(categoryLabel);
  layout->addWidget(categoryComboBox);
  layout->addWidget(priceLabel);
  layout->addWidget(priceLineEdit);

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
  item.price = QLocale().toDouble(priceLineEdit->text());

  return item;
}
