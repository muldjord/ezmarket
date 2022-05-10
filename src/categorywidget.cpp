/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            categorywidget.cpp
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

#include "categorywidget.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QLocale>

CategoryWidget::CategoryWidget(Data &data,
                               Category &category,
                               QWidget *parent)
  : QWidget(parent), data(data), category(category)
{
  QLabel *idLabel = new QLabel(tr("Category name:"));
  idLineEdit = new LineEdit(this);
  setFocusProxy(idLineEdit);

  QLabel *iconLabel = new QLabel(tr("Icon:"));
  searchLineEdit = new LineEdit(this);
  searchLineEdit->setPlaceholderText(tr("Icon search"));
  connect(searchLineEdit, &LineEdit::textChanged, this, &CategoryWidget::searchIcons);
  iconComboBox = new QComboBox(this);
  searchIcons();
  
  QLabel *lifespanLabel = new QLabel(tr("Lifespan:"));
  lifespanLineEdit = new LineEdit(this);
  lifespanLineEdit->setText("0");

  idLineEdit->setText(category.id);
  iconComboBox->setCurrentIndex(iconComboBox->findData(category.icon));
  lifespanLineEdit->setText(QLocale().toString(category.lifespan));

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->addWidget(idLabel);
  layout->addWidget(idLineEdit);
  layout->addWidget(iconLabel);
  layout->addWidget(searchLineEdit);
  layout->addWidget(iconComboBox);
  layout->addWidget(lifespanLabel);
  layout->addWidget(lifespanLineEdit);
  layout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Ignored, QSizePolicy::Expanding));

  setLayout(layout);
}

CategoryWidget::~CategoryWidget()
{
}

bool CategoryWidget::isSane()
{
  if(!idLineEdit->text().isEmpty() &&
     !lifespanLineEdit->text().isEmpty()) {
    return true;
  }
  return false;
}

void CategoryWidget::setIconSearchText()
{
  searchLineEdit->setText(idLineEdit->text());
}

void CategoryWidget::searchIcons()
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

void CategoryWidget::commitCategory()
{
  category.id = idLineEdit->text();
  category.icon = iconComboBox->currentData().toString();
  category.lifespan = QLocale().toInt(lifespanLineEdit->text());
}
