/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            soundboard.cpp
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

#include "soundboard.h"
#include "itemeditor.h"

#include <QVBoxLayout>
#include <QButtonGroup>
#include <QPushButton>
#include <QSound>
#include <QDir>

Soundboard::Soundboard(Data &data, QWidget *parent)
  : QWidget(parent)
{
  setStyleSheet("QLabel {font-size: " + QString::number(data.fontSize) + "px; qproperty-alignment: AlignCenter;}"
                "QLineEdit {font-size: " + QString::number(data.fontSize) + "px;}"
                "QComboBox {qproperty-iconSize: " + QString::number(data.iconSizeSmall) + "px; font-size: " + QString::number(data.fontSize) + "px;}"
                "QPushButton {border-image: url(graphics/soundbutton.png); qproperty-iconSize: " + QString::number(data.iconSizeSmall) + "px; font-size: " + QString::number(data.fontSize) + "px;}"
                "QPushButton:pressed {border-image: url(graphics/soundbutton_pressed.png); qproperty-iconSize: " + QString::number(data.iconSizeSmall) + "px; font-size: " + QString::number(data.fontSize) + "px;}");

  QDir soundsDir("sounds/soundboard", "*.wav", QDir::Name, QDir::Files);
  QList<QFileInfo> soundInfos = soundsDir.entryInfoList();
  QButtonGroup *soundButtons = new QButtonGroup(this);
  connect(soundButtons, qOverload<QAbstractButton *>(&QButtonGroup::buttonPressed), this, &Soundboard::playSound);

  int rowSounds = 0;

  QVBoxLayout *layout = new QVBoxLayout;
  QHBoxLayout *hLayout = new QHBoxLayout;

  for(const auto &soundInfo: soundInfos) {
    QString title = soundInfo.baseName();
    title.replace("_", " ").replace("-", " ").replace("ae", "æ").replace("oe", "ø").replace("aa", "å");
    title = title.left(1).toUpper() + title.mid(1);
    QList<QString> words = title.split(" ");
    title = "";
    QString line = "";
    while(words.length()) {
      if(line.length() < 15) {
        line += words.takeAt(0) + " ";
      } else {
        title += line + "\n";
        line = "";
      }
    }
    title += line;
    QPushButton *soundButton = new QPushButton(QIcon(QPixmap("graphics/sound.png").scaled(data.iconSize, data.iconSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)), title, this);
    soundButton->setFocusPolicy(Qt::NoFocus);
    soundButton->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);
    soundButton->setObjectName(soundInfo.absoluteFilePath());
    soundButtons->addButton(soundButton);
    hLayout->addWidget(soundButton);
    if(rowSounds >= 3) {
      layout->addLayout(hLayout);
      hLayout = new QHBoxLayout;
      rowSounds = 0;
    } else {
      rowSounds++;
    }
  }
  layout->addLayout(hLayout);
  setLayout(layout);
}

Soundboard::~Soundboard()
{
}

void Soundboard::playSound(QAbstractButton *button)
{
  QSound::play(button->objectName());
}
