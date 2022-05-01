/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            lineedit.cpp
 *
 *  Thu Mar 28 12:17:00 UTC+1 2019
 *  Copyright 2019 Lars Bisballe
 *  larsbisballe@gmail.com
 ****************************************************************************/

/*
 *  This file is part of Momaku.
 *
 *  Momaku is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Momaku is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Momaku; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
 */

#define DEBUG

#include <stdio.h>
#include <QSettings>

#include "lineedit.h"

extern QSettings *settings;

LineEdit::LineEdit(QString group, QString name, QString stdValue)
{
  this->group = group;
  this->name = name;
  this->defaultValue = stdValue;
  
  if(group != "General")
    settings->beginGroup(group);
  if(!settings->contains(name)) {
    settings->setValue(name, stdValue);
  }
  setText(settings->value(name, stdValue).toString());
  if(group != "General") {
    settings->endGroup();
  }

  connect(this, &LineEdit::editingFinished, this, &LineEdit::saveToConfig);
  connect(this, &LineEdit::textChanged, this, &LineEdit::saveToConfig);
}

LineEdit::~LineEdit()
{
}

void LineEdit::resetToDefault()
{
  setText(defaultValue);
}

void LineEdit::saveToConfig()
{
  if(text().contains(",")) {
    setText(text().replace(",", "."));
  }

  if(group != "General")
    settings->beginGroup(group);
  settings->setValue(name, this->text());
  if(group != "General")
    settings->endGroup();
  qDebug("Key '%s' saved to config with value '%s'\n", name.toStdString().c_str(), this->text().toStdString().c_str());
}
