/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            lineedit.h
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

#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QLineEdit>

class LineEdit : public QLineEdit
{
  Q_OBJECT
    
public:
  LineEdit(QString group, QString name, QString stdValue);
  ~LineEdit();
  
public slots:
  void resetToDefault();

protected:
  
private slots:
  void saveToConfig();

private:
  QString name;
  QString group;
  QString defaultValue;
};

#endif
