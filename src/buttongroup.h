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

#ifndef __BUTTONGROUP_H__
#define __BUTTONGROUP_H__

#include "datatypes.h"
#include "data.h"
#include "itemwidget.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>

class ButtonGroup : public QHBoxLayout
{
Q_OBJECT

public:
  ButtonGroup(QWidget *parent = nullptr);
  QMessageBox::ButtonRole getResult();                              
                              
public slots:

signals:
  void clicked();
    
private slots:
  void buttonClicked(QAbstractButton *button);

private:
  QMessageBox::ButtonRole result;
  QPushButton *deleteButton = nullptr;
  QPushButton *saveButton = nullptr;
  QPushButton *cancelButton = nullptr;
};


#endif // __BUTTONGROUP_H__
