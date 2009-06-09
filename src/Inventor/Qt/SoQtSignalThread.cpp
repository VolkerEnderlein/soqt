/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2009 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

#include "SoQtSignalThread.h"
#include <Inventor/Qt/moc_SoQtSignalThread.icc>

SoQtSignalThread::SoQtSignalThread(void)
  : isstopped(false)
{
}

SoQtSignalThread::~SoQtSignalThread()
{
}

void
SoQtSignalThread::trigger(void)
{
#ifdef SOQT_SIGNAL_THREAD_ACTIVE
  // lock first to make sure the QThread is actually waiting for a signal
  this->mutex.lock();
  this->waitcond.wakeOne();
  this->mutex.unlock();
#endif // SOQT_SIGNAL_THREAD_ACTIVE
}

void
SoQtSignalThread::stopThread(void)
{
#ifdef SOQT_SIGNAL_THREAD_ACTIVE
  this->mutex.lock();
  this->isstopped = true;
  this->waitcond.wakeOne();
  this->mutex.unlock();
#endif // SOQT_SIGNAL_THREAD_ACTIVE
}


void
SoQtSignalThread::run(void)
{
#ifdef SOQT_SIGNAL_THREAD_ACTIVE
  this->mutex.lock();
  while (!this->isstopped) {
    // just wait, and trigger every time we receive a signal
    this->waitcond.wait(&this->mutex);
    if (!this->isstopped) {
      emit triggerSignal();
    }
  }
  this->mutex.unlock();
#endif // SOQT_SIGNAL_THREAD_ACTIVE
}
