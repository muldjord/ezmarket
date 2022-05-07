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
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QLabel>
#include <QSound>

ItemEditor::ItemEditor(const QString &barcode,
                       QList<Item> &items,
                       const QList<Category> &categories,
                       const QMap<QString, QIcon> &icons,
                       QWidget *parent)
  : QDialog(parent), barcode(barcode), items(items), categories(categories)
{
  setWindowTitle(tr("Barcode: ") + barcode);
  setFixedSize(450, 700);

  setStyleSheet("QLabel {font-size: 35px; qproperty-alignment: AlignCenter;}"
                "QLineEdit {font-size: 35px;}"
                "QComboBox {qproperty-iconSize: 35px; font-size: 35px;}"
                "QPushButton {qproperty-iconSize: 35px; font-size: 35px;}");
  
  //QSound::play("sounds/ny_konto_eller_vare.wav");

  for(auto &item: items) {
    if(barcode == item.barcode) {
      itemWidget = new ItemWidget(barcode, item, categories, icons, this);
      break;
    }
  }

  QDialogButtonBox *dialogButtons = new QDialogButtonBox(QDialogButtonBox::Save |
                                                         QDialogButtonBox::Cancel);
  connect(dialogButtons, &QDialogButtonBox::accepted, this, &ItemEditor::checkSanity);
  connect(dialogButtons, &QDialogButtonBox::rejected, this, &ItemEditor::reject);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(itemWidget);
  layout->addWidget(dialogButtons);
  
  setLayout(layout);
}

void ItemEditor::checkSanity()
{
  if(itemWidget->isSane()) {
    accept();
  }
}