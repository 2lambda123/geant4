//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// $Id: G4Timer.hh,v 1.11 2001-07-11 10:00:52 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// ----------------------------------------------------------------------
// Class G4Timer
//
// Class description:
//
// Class for timer objects, able to measure elasped user/system process time.
//
// Note: Uses <sys/times.h> & <unistd.h> - POSIX.1 defined
//       If used, this header must be included in the source (.cc) file and it
//       must be the first header file to be included!
//
// Member functions:
//
// G4Timer()
//   Construct a timer object
// Start()
//   Start timing
// Stop()
//   Stop timing
// G4bool IsValid()
//   Return true if have a valid time (ie start() and stop() called)
// G4double GetRealElapsed()
//   Return the elapsed real time between last calling start() and stop()
// G4double GetSystemElapsed()
//   Return the elapsed system time between last calling start() and stop()
// G4double GetUserElapsed()
//   Return the elapsed user time between last calling start() and stop()
//
// Operators:
//
// G4std::ostream& operator << (G4std::ostream& os, const G4Timer& t);
//   Print the elapsed real,system and usertimes on os. Prints **s for times
//   if !IsValid
//
// Member data:
//
// G4bool fValidTimes
//   True after start and stop have both been called more than once and
//   an equal number of times
// clock_t fStartRealTime,fEndRealTime
//   Real times (arbitrary time 0)
// tms fStartTimes,fEndTimes
//   Timing structures (see times(2)) for start and end times

// History:
// 23.08.96 P.Kent Updated to also computed real elapsed time
// 21.08.95 P.Kent
// 29.04.97 G.Cosmo Added timings for Windows/NT

#ifndef G4TIMER_HH
#define G4TIMER_HH

#ifndef WIN32
#  include <unistd.h>
#  include <sys/times.h>
#else
#  include <time.h>
#  define _SC_CLK_TCK    1

   extern "C" {
          int sysconf(int);
   };

   // Structure returned by times()
  
   struct tms {
      clock_t tms_utime;           /* user time */
      clock_t tms_stime;           /* system time */
      clock_t tms_cutime;          /* user time, children */
      clock_t tms_cstime;          /* system time, children */
   };

   extern "C" {
      extern clock_t times(struct tms *);
   };
#endif  /* WIN32 */

#include "G4Types.hh"
#include "G4ios.hh"

class G4Timer
{
public:
    G4Timer() : fValidTimes(false) {;}

    inline void Start();
    inline void Stop();
    inline G4bool IsValid() const;
    G4double GetRealElapsed() const;
    G4double GetSystemElapsed() const;
    G4double GetUserElapsed() const;
private:
    G4bool fValidTimes;
    clock_t fStartRealTime,fEndRealTime;
    tms fStartTimes,fEndTimes;
};

G4std::ostream& operator << (G4std::ostream& os, const G4Timer& t);

// Inline functions:

inline void G4Timer::Start()
{
    fValidTimes=false;
    fStartRealTime=times(&fStartTimes);
}

inline void G4Timer::Stop()
{
    fEndRealTime=times(&fEndTimes);
    fValidTimes=true;
}

inline G4bool G4Timer::IsValid() const
{
    return fValidTimes;
}

#define times ostimes

#endif

