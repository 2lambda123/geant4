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
// $Id: G4VInteractorManager.hh,v 1.8 2003-06-16 16:56:06 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// G.Barrand

#ifndef G4VINTERACTORMANAGER_HH
#define G4VINTERACTORMANAGER_HH

#include "globals.hh"
#include <vector>

typedef void*  G4Interactor;
typedef G4bool (*G4DispatchFunction)(void*);
typedef void   (*G4SecondaryLoopAction)();

// Class description :
//
//  G4VInteractorManager : a base class to isolate common things
// to various GUI "toolkits" like WIndows, Xt.
//  The word "interactor" is for "piece of user interface" or
// "widget" (which means nothing). Then a GUI "toolkit" could be 
// defined as a manager of interactors.
//
// Class description - end :

class G4VInteractorManager {
public:
                 G4VInteractorManager            ();                     
  virtual       ~G4VInteractorManager            ();                     
  void           SetArguments                    (int,char**);
  char**         GetArguments                    (int*);
  void           SetMainInteractor               (G4Interactor);
  G4Interactor   GetMainInteractor               ();
  void           AddDispatcher                   (G4DispatchFunction);
  void           RemoveDispatcher                (G4DispatchFunction);
  void           AddSecondaryLoopPreAction       (G4SecondaryLoopAction);
  void           AddSecondaryLoopPostAction      (G4SecondaryLoopAction);
  void           AddShell                        (G4Interactor);
  void           RemoveShell                     (G4Interactor);
  void           EnableSecondaryLoop             ();
  void           DisableSecondaryLoop            ();
  void           SecondaryLoopPreActions         ();
  void           SecondaryLoopPostActions        ();
  void           RequireExitSecondaryLoop        (int); 
  void           DispatchEvent                   (void*);
  void           SecondaryLoop                   (); 
  int            GetExitSecondaryLoopCode        ();
  void           PutStringInResourceDatabase     (char*);
  virtual G4bool Inited                          () = 0;
  virtual void*  GetEvent                        () = 0;
  virtual void   FlushAndWaitExecution           () = 0;
  void           SetParentInteractor             (G4Interactor);
  G4Interactor   GetParentInteractor             ();
  void           SetCreatedInteractor            (G4Interactor);
  G4Interactor   GetCreatedInteractor            ();
  void           SetCreationString               (char*);
  char*          GetCreationString               ();
private:
  int                    argc;
  char**                 argv;
  G4Interactor           mainInteractor;
  std::vector<G4DispatchFunction> dispatchers;
  std::vector<G4SecondaryLoopAction> preActions;
  std::vector<G4SecondaryLoopAction> postActions;
  std::vector<G4Interactor> shells;
  G4bool                 secondaryLoopEnabled;
  G4bool                 alreadyInSecondaryLoop;
  int                    exitSecondaryLoop;
  G4Interactor           parentInteractor;
  G4Interactor           createdInteractor;
  char*                  creationString;
};

#define OGL_EXIT_CODE 1
#define OIV_EXIT_CODE 2
#define XO_EXIT_CODE  3

#endif
