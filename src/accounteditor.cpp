/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            accounteditor.cpp
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

#include "accounteditor.h"

#include <QPushButton>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QLabel>
#include <QSound>

AccountEditor::AccountEditor(const QString &barcode,
                             Data &data,
                             QWidget *parent)
  : QDialog(parent), barcode(barcode), data(data)
{
  setWindowTitle(tr("Barcode: ") + barcode);
  setFixedSize(data.editorWidth, data.editorWidth);

  setStyleSheet("QLabel {font-size: " + QString::number(data.fontSize) + "px; qproperty-alignment: AlignCenter;}"
                "QLineEdit {font-size: " + QString::number(data.fontSize) + "px;}"
                "QComboBox {qproperty-iconSize: " + QString::number(data.iconSizeSmall) + "px; font-size: " + QString::number(data.fontSize) + "px;}"
                "QPushButton {qproperty-iconSize: " + QString::number(data.iconSizeSmall) + "px; font-size: " + QString::number(data.fontSize) + "px;}");
  
  QLabel *accountLabel = new QLabel(this);
  accountLabel->setPixmap(QPixmap("graphics/account.png").scaled(data.iconSize, data.iconSize));

  for(auto &account: data.accounts) {
    if(barcode == account.barcode) {
      accountWidget = new AccountWidget(data, account, this);
      break;
    }
  }

  buttonGroup = new ButtonGroup;
  connect(buttonGroup, &ButtonGroup::clicked, this, &AccountEditor::checkButton);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(accountLabel);
  layout->addWidget(accountWidget);
  layout->addLayout(buttonGroup);
  
  setLayout(layout);
}

void AccountEditor::checkButton()
{
  if(buttonGroup->getResult() == QMessageBox::ActionRole) {
    if(QMessageBox::question(this, tr("Delete account?"), tr("Are you sure you want to delete this account?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes) {;
      accountWidget->removeAccount();
      accept();
    }
  } else if(buttonGroup->getResult() == QMessageBox::AcceptRole) {
    if(accountWidget->isSane()) {
      accountWidget->commitAccount();
      accept();
    }
  } else if(buttonGroup->getResult() == QMessageBox::RejectRole) {
    reject();
  }
}
