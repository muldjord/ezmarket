/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            accountwidget.cpp
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

#include "accountwidget.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QLocale>

AccountWidget::AccountWidget(const QString &barcode, QWidget *parent)
  : QWidget(parent), barcode(barcode)
{
  QLabel *idLabel = new QLabel(tr("Account holder:"));
  idLineEdit = new LineEdit(this);
  setFocusProxy(idLineEdit);

  QLabel *balanceLabel = new QLabel(tr("Balance:"));
  balanceLineEdit = new LineEdit(this);
  balanceLineEdit->setText("100.0");

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->addWidget(idLabel);
  layout->addWidget(idLineEdit);
  layout->addWidget(balanceLabel);
  layout->addWidget(balanceLineEdit);
  layout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Ignored, QSizePolicy::Expanding));

  setLayout(layout);
}

AccountWidget::~AccountWidget()
{
}

bool AccountWidget::isSane()
{
  if(!idLineEdit->text().isEmpty() &&
     !balanceLineEdit->text().isEmpty()) {
    return true;
  }
  return false;
}

Account AccountWidget::getAccount()
{
  Account account;

  account.barcode = this->barcode;
  account.id = idLineEdit->text();
  account.balance = QLocale().toDouble(balanceLineEdit->text());

  return account;
}
