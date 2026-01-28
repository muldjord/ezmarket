/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            soundmixer.h
 *
 *  Tue Nov 23 09:36:00 CEST 2019
 *  Copyright 2019 Lars Muldjord
 *  muldjordlars@gmail.com
 ****************************************************************************/

/*
 *  This file is part of Boris.
 *
 *  Boris is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Boris is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Boris; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
 */

#pragma once

#include "SFML/Audio.hpp"

#include <QObject>
#include <QMap>

class SoundMixer : public QObject
{
  Q_OBJECT;

public:
  explicit SoundMixer();
  void setChannels(const int &channels);
  void playSound(const QString &baseName, const int &type = 0);
  void playBuffer(const sf::SoundBuffer *buffer,
                  const float &panning = 0.0f,
                  const float &pitch = 1.0f);

  // Sound effects
  QMap<QString, sf::SoundBuffer> generalSounds;
  QMap<QString, sf::SoundBuffer> ambienceSounds;
  QMap<QString, sf::SoundBuffer> soundboardSounds;

private:
  QList<sf::Sound> soundChannels;
};
