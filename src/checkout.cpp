/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            checkout.cpp
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

#include "checkout.h"
#include "itemeditor.h"

#include <QVBoxLayout>
#include <QButtonGroup>
#include <QPushButton>
#include <QSound>
#include <QDir>

Checkout::Checkout(Data &data, QWidget *parent)
  : QWidget(parent), data(data)
{
  setStyleSheet("QListWidget {font-size: " + QString::number(data.fontSize) + "px; qproperty-iconSize: " + QString::number(data.iconSize) + "px;}");

  checkoutList = new QListWidget(this);
  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(checkoutList);
  setLayout(layout);
  clearItems();
}

Checkout::~Checkout()
{
}

void Checkout::payBy(const QString &barcode)
{
  double total = 0.0;
  for(const auto &item: checkoutItems) {
    total += item.price - item.discount;
  }
  for(auto &account: data.accounts) {
    if(account.barcode == barcode) {
      account.balance -= total;
    }
  }
  QSound::play("sounds/betaling_modtaget-tak_fordi_du_handlede_i_butikken.wav");
  clearItems();
}

void Checkout::addItem(const QString &barcode)
{
  checkoutList->clear();
  for(const auto &item: data.items) {
    if(item.barcode == barcode) {
      checkoutItems.append(item);
    }
  }
  double total = 0.0;
  for(const auto &item: checkoutItems) {
    total += item.price - item.discount;
    checkoutList->addItem(new QListWidgetItem(QIcon(data.icons[item.icon]), item.id + ": " + QString::number(item.price) + tr("$")));
    if(item.discount > 0.0) {
      checkoutList->addItem(new QListWidgetItem("\t-" + QString::number(item.discount) + tr("$") + tr(" discount")));
    }
  }
  checkoutList->addItem(new QListWidgetItem("-------------------"));
  checkoutList->addItem(new QListWidgetItem(tr("Total: ") + QString::number(total) + tr("$")));
}

void Checkout::clearItems()
{
  checkoutItems.clear();
  checkoutList->clear();
  checkoutList->addItem(new QListWidgetItem(tr("Please scan the first item...")));
}
