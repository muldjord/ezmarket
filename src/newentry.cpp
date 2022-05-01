/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            newentry.cpp
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

#include "newentry.h"

#include <QPushButton>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QDialogButtonBox>

NewEntry::NewEntry(QString barcode, QWidget *parent): QDialog(parent)
{
  setWindowTitle(tr("Barcode: ") + barcode);
  setFixedSize(450, 350);

  setStyleSheet("QLabel {font-size: 40px; qproperty-alignment: AlignCenter;}"
                "QPushButton {qproperty-iconSize: 40px; font-size: 40px;}"
                "QLineEdit {font-size: 40px;}");
  
  QLabel *typeLabel = new QLabel(tr("New person or book?"));

  QPushButton *personButton = new QPushButton(tr("Person"));
  personButton->setIcon(QIcon("graphics/person.png"));
  personButton->setCheckable(true);
  personButton->setObjectName("person");
  personButton->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);

  QPushButton *bookButton = new QPushButton(tr("Book"));
  bookButton->setIcon(QIcon("graphics/book.png"));
  bookButton->setCheckable(true);
  bookButton->setObjectName("book");
  bookButton->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);

  typeGroup = new QButtonGroup(this);
  typeGroup->addButton(personButton);
  typeGroup->addButton(bookButton);
  typeGroup->setExclusive(true);
  connect(typeGroup, qOverload<QAbstractButton *>(&QButtonGroup::buttonReleased), this, &NewEntry::typeChanged);

  QHBoxLayout *buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(personButton);
  buttonLayout->addWidget(bookButton);

  nameTitleLabel = new QLabel(this);
  nameTitleLineEdit = new QLineEdit(this);

  QDialogButtonBox *dialogButtons = new QDialogButtonBox(QDialogButtonBox::Save |
                                                         QDialogButtonBox::Cancel);
  connect(dialogButtons, &QDialogButtonBox::accepted, this, &NewEntry::checkNameTitle);
  connect(dialogButtons, &QDialogButtonBox::rejected, this, &NewEntry::reject);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(typeLabel);
  layout->addLayout(buttonLayout);
  layout->addWidget(nameTitleLabel);
  layout->addWidget(nameTitleLineEdit);
  layout->addWidget(dialogButtons);
  
  setLayout(layout);
}

void NewEntry::typeChanged(QAbstractButton *button)
{
  if(button->objectName() == "person") {
    nameTitleLabel->setText(tr("Name:"));
  } else if(button->objectName() == "book") {
    nameTitleLabel->setText(tr("Title:"));
  }
  nameTitleLineEdit->setFocus();
}

QString NewEntry::getType()
{
  return typeGroup->checkedButton()->objectName();
}

QString NewEntry::getNameTitle()
{
  return nameTitleLineEdit->text();
}

void NewEntry::checkNameTitle()
{
  if(typeGroup->checkedButton() == nullptr) {
    printf("NO ENTRY TYPE SELECTED!\n");
    return;
  }
  if(nameTitleLineEdit->text().trimmed().isEmpty()) {
    printf("MISSING NAME OR TITLE!\n");
    return;
  }
  accept();
}
