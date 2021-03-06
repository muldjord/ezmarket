/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            datatypes.h
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

#ifndef __DATATYPES_H__
#define __DATATYPES_H__

#include <QString>

struct Account {
  QString barcode = "";
  QString id = "";
  double balance = 0.0;
  int bonus = 0;
};

struct Item {
  QString icon = ""; // PNG file basename
  QString id = "";
  QString category = ""; // Category barcode.
  double price = 0.0;
  double discount = 0.0; // Subtracted from price.
  int stock = 0;
  int age = 0; // Seconds. Reset whenever one or more is added to stock emulating renewal.
  QString barcode = "";
};

struct Category {
  QString barcode = "";
  QString id = "";
  QString icon = ""; // PNG file basename
  int lifespan = 20;
};

#endif // __DATATYPES_H__
