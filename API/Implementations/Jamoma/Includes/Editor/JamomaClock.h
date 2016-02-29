/*!
 * \file JamomaClock.h
 *
 * \brief
 *
 * \details
 *
 * \author Théo de la Hogue
 *
 * \copyright This code is licensed under the terms of the "CeCILL-C"
 * http://www.cecill.info
 */

#pragma once

#include <ctime>
#include <ratio>
#include <chrono>
#include <thread>
#include <mutex>
#include <cmath>

#include "Editor/Clock.h"

using namespace OSSIA;
using namespace std;
using namespace std::chrono;

class JamomaClock : public virtual Clock
{

protected:

# pragma mark -
# pragma mark Implementation specific

  TimeValue         mDuration{};        /// the time (in ms) the clock will run at normal speed factor
  TimeValue         mGranularity{};     /// the minimum time between each tick (in ms)
  TimeValue         mOffset{};          /// the date (in ms) the clock will run from
  double            mSpeed{};           /// the speed factor of the clock

  Clock::DriveMode  mDriveMode{};       /// in EXTERNAL drive mode the tick() method is called from outside

  bool              mRunning{};         /// is the clock running right now ?
  bool              mPaused{};          /// is the clock paused right now ?
  TimeValue         mPosition{};        /// the progression of the clock between the beginning and the end [0. :: 1.]
  TimeValue         mDate{};            /// how many time the clock is running (without no speed factor consideration)

  thread            mThread;            /// a thread to launch the clock execution

  steady_clock::time_point mLastTime{}; /// a time reference used to compute time tick
  long long         mElapsedTime{};     /// a time reference used to know how many time are elapsed in microsecond

private:
  ExecutionCallback mCallback;          /// the callback to use for each step

public:

# pragma mark -
# pragma mark Life cycle

  JamomaClock(Clock::ExecutionCallback,
              const TimeValue& = Infinite,
              const TimeValue& = 1.,
              const TimeValue& = 0.,
              float = 1.,
              Clock::DriveMode = Clock::DriveMode::INTERNAL);

  JamomaClock(const JamomaClock *);

  ~JamomaClock();

# pragma mark -
# pragma mark Execution

  void start() override;

  void stop() override;

  void pause() override;

  void resume() override;

  bool tick() final override;

# pragma mark -
# pragma mark Accessors

  const TimeValue & getDuration() const override;
  
  Clock & setDuration(const TimeValue&) override;
  
  const TimeValue & getOffset() const final override;
  
  Clock & setOffset(const TimeValue&) override;
    
  const TimeValue & getGranularity() const final override;

  Clock & setGranularity(const TimeValue&) final override;

  float getSpeed() const final override;

  Clock & setSpeed(float) final override;

  Clock::DriveMode getDriveMode() const final override;

  Clock & setDriveMode(Clock::DriveMode) final override;

  bool getRunning() const final override;

  const TimeValue & getPosition() const final override;

  const TimeValue & getDate() const final override;

# pragma mark -
# pragma mark Implementation specific

  /*! to avoid dead lock in EXTERNAL drive mode if a TimeProcess wants to end its ParentTimeConstraint's clock */
  void request_stop();
  
protected:
  
  /*! to allow TimeConstraint to override start method */
  void do_start();
  
  /*! to allow TimeConstraint to override stop method */
  void do_stop();
  
  /*! to allow TimeConstraint to override setDuration accessor */
  void do_setDuration(const TimeValue&);
  
  /*! to allow TimeConstraint to override setOffset accessor */
  void do_setOffset(const TimeValue&);

private:

  /*! called back by the internal thread */
  void threadCallback();
};
