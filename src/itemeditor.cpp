/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            itemeditor.cpp
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

#include "itemeditor.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QSound>
#include <QMessageBox>

ItemEditor::ItemEditor(const QString &barcode,
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

  QLabel *itemLabel = new QLabel(this);
  itemLabel->setPixmap(QPixmap("graphics/item.png").scaled(data.iconSize, data.iconSize));

  for(auto &item: data.items) {
    if(barcode == item.barcode) {
      itemWidget = new ItemWidget(data, item, this);
      break;
    }
  }

  buttonGroup = new ButtonGroup;
  connect(buttonGroup, &ButtonGroup::clicked, this, &ItemEditor::checkButton);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(itemLabel);
  layout->addWidget(itemWidget);
  layout->addLayout(buttonGroup);
  
  setLayout(layout);
}

void ItemEditor::checkButton()
{
  if(buttonGroup->getResult() == QMessageBox::ActionRole) {
    if(QMessageBox::question(this, tr("Delete item?"), tr("Are you sure you want to delete this item?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes) {;
      itemWidget->removeItem();
      accept();
    }
  } else if(buttonGroup->getResult() == QMessageBox::AcceptRole) {
    if(itemWidget->isSane()) {
      itemWidget->commitItem();
      accept();
    }
  } else if(buttonGroup->getResult() == QMessageBox::RejectRole) {
    reject();
  }
}
