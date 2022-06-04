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

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QPushButton>
#include <QSound>
#include <QTimer>

Checkout::Checkout(Data &data, QWidget *parent)
  : QWidget(parent), data(data)
{
  setStyleSheet("QListWidget {padding: 50px; border-image: url(graphics/checkout_background.png); font-size: " + QString::number(data.fontSizeSmall) + "px; qproperty-iconSize: " + QString::number(data.iconSize) + "px;}");

  checkoutList = new QListWidget(this);
  checkoutList->setMinimumWidth(800);
  checkoutList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  QFont font = checkoutList->font();
  font.setFamily("courier");
  checkoutList->setFont(font);
  QHBoxLayout *hLayout = new QHBoxLayout;
  hLayout->addStretch();
  hLayout->addWidget(checkoutList);
  hLayout->addStretch();
  QVBoxLayout *layout = new QVBoxLayout;
  layout->addLayout(hLayout);
  setLayout(layout);
  clearItemList();
}

Checkout::~Checkout()
{
}

void Checkout::payBy(const QString &barcode)
{
  if(checkoutItems.length() <= 0) {
    return;
  }
  double total = 0.0;
  for(const auto &item: checkoutItems) {
    total += item.price - item.discount;
  }
  for(auto &account: data.accounts) {
    if(account.barcode == barcode) {
      account.balance -= total;
      int bonus = total / 100;
      account.bonus += bonus;
      checkoutList->addItem(new QListWidgetItem("-------------------"));
      checkoutList->addItem(new QListWidgetItem(tr("Paid by '") + account.id + "'"));
      if(bonus > 0) {
        checkoutList->addItem(new QListWidgetItem(tr("Bonus for this purchase: ") + QLocale().toString(bonus)));
      }
      checkoutList->addItem(new QListWidgetItem(tr("New balance: ") + QLocale().toString(account.balance, 'f', 2)));
      if(account.balance < 0) {
        QSound::play("sounds/betaling_modtaget-advarsel_konto_i_minus.wav");
      } else {
        QSound::play("sounds/betaling_modtaget-tak_fordi_du_handlede_i_butikken.wav");
      }
    }
  }
  checkoutItems.clear();
  checkoutList->scrollToBottom();
  QTimer::singleShot(10000, this, &Checkout::clearItemList);
}

void Checkout::addItem(const QString &barcode)
{
  for(auto &item: data.items) {
    if(item.barcode == barcode) {
      int lifespan = -1;
      for(const auto &category: data.categories) {
        if(category.barcode == item.category) {
          lifespan = category.lifespan;
        }
      }
      if(item.stock <= 0) {
        QSound::play("sounds/varen_er_udsolgt.wav");
      } else if(item.age > lifespan) {
        QSound::play("sounds/varen_er_for_gammel.wav");
      } else {
        checkoutItems.append(item);
        item.stock -= 1;
        if(item.stock <= 0) {
          QSound::play("sounds/varen_er_nu_udsolgt.wav");
        }
      }
    }
  }

  if(checkoutItems.length() <= 0) {
    return;
  }
  checkoutList->clear();

  double subTotal = 0.0;
  double discount = 0.0;
  double total = 0.0;
  for(const auto &item: checkoutItems) {
    subTotal += item.price;
    discount -= item.discount;
    total += item.price - item.discount;
    checkoutList->addItem(new QListWidgetItem(QIcon(data.icons[item.icon]), item.id + ": " + QLocale().toString(item.price, 'f', 2) + tr("$")));
    if(item.discount > 0.0) {
      checkoutList->addItem(new QListWidgetItem("\t-" + QLocale().toString(item.discount, 'f', 2) + tr("$") + tr(" discount")));
    }
  }
  checkoutList->addItem(new QListWidgetItem("-------------------"));
  checkoutList->addItem(new QListWidgetItem(tr("Subtotal : ") + QLocale().toString(subTotal, 'f', 2) + tr("$")));
  checkoutList->addItem(new QListWidgetItem(tr("Discount : ") + QLocale().toString(discount, 'f', 2) + tr("$")));
  checkoutList->addItem(new QListWidgetItem(tr("To pay   : ") + QLocale().toString(total, 'f', 2) + tr("$")));
  checkoutList->scrollToBottom();
}

void Checkout::clearItemList()
{
  checkoutList->clear();
  checkoutList->addItem(new QListWidgetItem(tr("Scan item...")));
}
