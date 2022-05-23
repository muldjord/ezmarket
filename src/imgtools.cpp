/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            imgtools.cpp
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

#include "imgtools.h"

#include <QPainter>

QPixmap ImgTools::getPreparedIcon(const QPixmap &icon, const int &iconSize,
                                  const QPixmap &iconBack)
{
  QImage image(iconSize + 2, iconSize + 2, QImage::Format_ARGB32);
  image.fill(Qt::transparent);
  QPainter painter(&image);
  if(!iconBack.isNull()) {
    painter.drawPixmap(0, 0, iconBack);
  }
  painter.drawPixmap(1, 1, icon);
  painter.end();
  /*
  QImage image(96, 96, QImage::Format_ARGB32);
  image.fill(Qt::transparent);
  QPainter painter(&image);
  QBrush brush(Qt::SolidPattern);
  brush.setColor(Qt::black);
  painter.setBrush(brush);
  painter.drawEllipse(0, 0, 95, 95);
  painter.end();
  image = image.scaled(data.iconSize8, data.iconSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  painter.begin(&image);
  painter.drawPixmap(0, 0, icon.pixmap(data.iconSize, data.iconSize));
  painter.end();
  */

  return QPixmap::fromImage(image);
}
