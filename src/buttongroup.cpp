/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            buttongroup.cpp
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

#include "buttongroup.h"

#include <QButtonGroup>

ButtonGroup::ButtonGroup(QWidget *parent)
  : QHBoxLayout(parent)
{
  deleteButton = new QPushButton(tr("Delete"));

  saveButton = new QPushButton(tr("Save changes"));
  cancelButton = new QPushButton(tr("Cancel"));

  QButtonGroup *buttonGroup = new QButtonGroup;
  buttonGroup->addButton(deleteButton);
  buttonGroup->addButton(saveButton);
  buttonGroup->addButton(cancelButton);
  connect(buttonGroup, qOverload<QAbstractButton *>(&QButtonGroup::buttonClicked), this, &ButtonGroup::buttonClicked);
  
  addWidget(deleteButton);
  addStretch(1);
  addWidget(saveButton);
  addWidget(cancelButton);
}

void ButtonGroup::buttonClicked(QAbstractButton *button)
{
  if(button == deleteButton) {
    result = QMessageBox::ActionRole;
  } else if(button == saveButton) {
    result = QMessageBox::AcceptRole;
  } else if(button == cancelButton) {
    result = QMessageBox::RejectRole;
  }
  emit clicked();
}

QMessageBox::ButtonRole ButtonGroup::getResult()
{
  return result;
}
