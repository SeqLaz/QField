/***************************************************************************
 tracker.cpp - Tracker
  ---------------------
 begin                : 20.02.2020
 copyright            : (C) 2020 by David Signer
 email                : david (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "rubberbandmodel.h"
#include "tracker.h"

#include <QTimer>
#include <qgsdistancearea.h>
#include <qgsproject.h>
#include <qgssensormanager.h>

Tracker::Tracker( QgsVectorLayer *layer, bool visible )
  : mLayer( layer ), mVisible( visible )
{
}

RubberbandModel *Tracker::model() const
{
  return mRubberbandModel;
}

void Tracker::setModel( RubberbandModel *model )
{
  if ( mRubberbandModel == model )
    return;
  mRubberbandModel = model;
}

void Tracker::trackPosition()
{
  if ( !model() || std::isnan( model()->currentCoordinate().x() ) || std::isnan( model()->currentCoordinate().y() ) )
  {
    return;
  }

  model()->addVertex();

  mTimeIntervalFulfilled = false;
  mMinimumDistanceFulfilled = false;
  mSensorCaptureFulfilled = false;
}

void Tracker::positionReceived()
{
  if ( !qgsDoubleNear( mMinimumDistance, 0.0 ) )
  {
    QVector<QgsPointXY> points = mRubberbandModel->flatPointSequence( QgsProject::instance()->crs() );

    auto pointIt = points.constEnd() - 1;

    QVector<QgsPointXY> flatPoints;

    flatPoints << *pointIt;
    pointIt--;
    flatPoints << *pointIt;

    QgsDistanceArea distanceArea;
    distanceArea.setEllipsoid( QgsProject::instance()->ellipsoid() );
    distanceArea.setSourceCrs( QgsProject::instance()->crs(), QgsProject::instance()->transformContext() );

    if ( distanceArea.measureLine( flatPoints ) > mMinimumDistance )
    {
      mMinimumDistanceFulfilled = true;
    }
  }
  else
  {
    mMinimumDistanceFulfilled = true;
  }

  if ( !mConjunction || ( mTimeIntervalFulfilled && mSensorCaptureFulfilled ) )
  {
    trackPosition();
  }
}

void Tracker::timeReceived()
{
  mTimeIntervalFulfilled = true;

  if ( !mConjunction || ( mMinimumDistanceFulfilled && mSensorCaptureFulfilled ) )
  {
    trackPosition();
  }
}

void Tracker::sensorDataReceived()
{
  mSensorCaptureFulfilled = true;

  if ( !mConjunction || ( mMinimumDistanceFulfilled && mSensorCaptureFulfilled ) )
  {
    trackPosition();
  }
}

void Tracker::start()
{
  if ( mTimeInterval > 0 )
  {
    connect( &mTimer, &QTimer::timeout, this, &Tracker::timeReceived );
    mTimer.start( mTimeInterval * 1000 );
  }
  else
  {
    mTimeIntervalFulfilled = true;
  }
  if ( mMinimumDistance > 0 || qgsDoubleNear( mTimeInterval, 0.0 ) )
  {
    connect( mRubberbandModel, &RubberbandModel::currentCoordinateChanged, this, &Tracker::positionReceived );
  }
  else
  {
    mMinimumDistanceFulfilled = true;
  }
  if ( mSensorCapture )
  {
    connect( QgsProject::instance()->sensorManager(), &QgsSensorManager::sensorDataCaptured, this, &Tracker::sensorDataReceived );
  }
  else
  {
    mSensorCaptureFulfilled = true;
    if ( mTimeInterval > 0 || mSensorCapture )
    {
      // Other constraints will guide verdex addition
      return;
    }
  }

  //set the start time
  setStartPositionTimestamp( QDateTime::currentDateTime() );

  if ( mMeasureType == Tracker::SecondsSinceStart )
  {
    model()->setMeasureValue( 0 );
  }

  //track first position
  trackPosition();
}

void Tracker::stop()
{
  //track last position
  trackPosition();

  if ( mTimeInterval > 0 )
  {
    mTimer.stop();
    disconnect( &mTimer, &QTimer::timeout, this, &Tracker::trackPosition );
  }
  if ( mMinimumDistance > 0 )
  {
    disconnect( mRubberbandModel, &RubberbandModel::currentCoordinateChanged, this, &Tracker::positionReceived );
  }
  if ( mSensorCapture )
  {
    disconnect( QgsProject::instance()->sensorManager(), &QgsSensorManager::sensorDataCaptured, this, &Tracker::sensorDataReceived );
  }
}
