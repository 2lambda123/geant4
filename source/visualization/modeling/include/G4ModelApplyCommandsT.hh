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
// $Id: G4ModelApplyCommandsT.hh,v 1.1 2006-05-02 20:47:40 tinslay Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Abstract model messenges. Derived classes should implement
// the "Apply" method
//
// Jane Tinslay April 2006
//
#ifndef G4APPLYCOMMANDST_HH
#define G4APPLYCOMMANDST_HH

#include "G4Colour.hh"
#include "G4String.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcommand.hh"
#include "G4VModelCommand.hh"
#include "globals.hh"
#include <sstream>

///////////////////////////////////////////////////////////////////////////
// ApplyStringColour command
template <typename M>
class G4ModelCmdApplyStringColour : public G4VModelCommand<M> {

public: // With description

  G4ModelCmdApplyStringColour(M* model, const G4String& placement, const G4String& cmdName);

  virtual ~G4ModelCmdApplyStringColour();

  void SetNewValue(G4UIcommand* command, G4String newValue);

protected:

  // Implement in derived class
  virtual void Apply(const G4String&, const G4Colour&) = 0;

  G4UIcommand* StringCommand() {return fpStringCmd;}
  G4UIcommand* ComponentCommand() {return fpComponentCmd;}

private:

  G4UIcommand* fpStringCmd;
  G4UIcommand* fpComponentCmd;

};

template <typename M>
G4ModelCmdApplyStringColour<M>::G4ModelCmdApplyStringColour(M* model, const G4String& placement, const G4String& cmdName)
  :G4VModelCommand<M>(model)
{
  //Set variable colour through a string
  G4String dir = placement+"/"+model->Name()+"/"+cmdName;
  G4UIparameter* param(0);

  fpStringCmd = new G4UIcommand(dir, this);
  fpStringCmd->SetGuidance("Set variable colour through a string");   
  
  param = new G4UIparameter("Variable", 's', false);
  fpStringCmd->SetParameter(param);

  param = new G4UIparameter("Value", 's', false);
  fpStringCmd->SetParameter(param);

  //Set variable colour through RGBA components
  G4String componentDir = dir+"RGBA";
  
  fpComponentCmd = new G4UIcommand(componentDir, this);
  fpComponentCmd->SetGuidance("Set variable colour through red, green, blue and alpha components");   
  param = new G4UIparameter("Variable", 's', false);
  fpComponentCmd->SetParameter(param);

  param = new G4UIparameter("Red component", 'd', false);
  fpComponentCmd->SetParameter(param);

  param = new G4UIparameter("Green component", 'd', false);
  fpComponentCmd->SetParameter(param);

  param = new G4UIparameter("Blue component", 'd', false);
  fpComponentCmd->SetParameter(param);

  param = new G4UIparameter("Alpha component", 'd', false);
  fpComponentCmd->SetParameter(param);
} 

template <typename M>
G4ModelCmdApplyStringColour<M>::~G4ModelCmdApplyStringColour()
{ 
  delete fpStringCmd;
  delete fpComponentCmd;
}

template <typename M>
void G4ModelCmdApplyStringColour<M>::SetNewValue(G4UIcommand* cmd, G4String newValue)
{
  G4Colour myColour;
  G4String parameter;
  
  if (cmd == fpStringCmd) {
    G4String colour;
    std::istringstream is (newValue);
    is >> parameter >> colour;
    
    // Colour key should exist
    if (!G4Colour::GetColour(colour, myColour)) {
      std::ostringstream o;
      o << "G4Colour with key "<<colour<<" does not exist ";
      G4Exception
	("G4ModelCmdApplyStringColour<M>::SetNewValue",
	 "NonExistentColour", JustWarning, o.str().c_str());
      return;
    }
  }
  
  if (cmd == fpComponentCmd) {
    G4double red(0), green(0), blue(0), alpha(0);
    std::istringstream is (newValue);
    is >> parameter >> red >> green >> blue >> alpha;
    
    G4Colour colour(red, green, blue, alpha);
    myColour = colour;
  }

  Apply(parameter, myColour);
}

///////////////////////////////////////////////////////////////////////////
//ApplyColour command
template <typename M>
class G4ModelCmdApplyColour : public G4VModelCommand<M> {

public: // With description

  G4ModelCmdApplyColour(M* model, const G4String& placement, const G4String& cmdName);

  virtual ~G4ModelCmdApplyColour();

  void SetNewValue(G4UIcommand* command, G4String newValue);

protected:

  // Implement in derived class
  virtual void Apply(const G4Colour&) = 0;

  G4UIcommand* StringCommand() {return fpStringCmd;}
  G4UIcommand* ComponentCommand() {return fpComponentCmd;}

private:

  G4UIcommand* fpStringCmd;
  G4UIcommand* fpComponentCmd;

};

template <typename M>
G4ModelCmdApplyColour<M>::G4ModelCmdApplyColour(M* model, const G4String& placement, const G4String& cmdName)
  :G4VModelCommand<M>(model)
{
  //Set colour through a string
  G4String dir = placement+"/"+model->Name()+"/"+cmdName;
  G4UIparameter* param(0);

  fpStringCmd = new G4UIcommand(dir, this);
  fpStringCmd->SetGuidance("Set colour through a string");   
  
  param = new G4UIparameter("Variable", 's', false);
  fpStringCmd->SetParameter(param);

  //Set colour through RGBA components
  G4String componentDir = dir+"RGBA";
  
  fpComponentCmd = new G4UIcommand(componentDir, this);
  fpComponentCmd->SetGuidance("Set colour through red, green, blue and alpha components");   
  fpComponentCmd->SetGuidance("Four inputs are expected.");

  param = new G4UIparameter("Red component", 'd', false);
  fpComponentCmd->SetParameter(param);

  param = new G4UIparameter("Green component", 'd', false);
  fpComponentCmd->SetParameter(param);

  param = new G4UIparameter("Blue component", 'd', false);
  fpComponentCmd->SetParameter(param);

  param = new G4UIparameter("Alpha component", 'd', false);
  fpComponentCmd->SetParameter(param);
} 

template <typename M>
G4ModelCmdApplyColour<M>::~G4ModelCmdApplyColour()
{ 
  delete fpStringCmd;
  delete fpComponentCmd;
}

template <typename M>
void G4ModelCmdApplyColour<M>::SetNewValue(G4UIcommand* cmd, G4String newValue)
{
  G4Colour myColour;

  if (cmd == fpStringCmd) {
    G4String colour;
    std::istringstream is (newValue);
    is >> colour;
    
    // Colour key should exist
    if (!G4Colour::GetColour(colour, myColour)) {
      std::ostringstream o;
      o << "G4Colour with key "<<colour<<" does not exist ";
      G4Exception
	("G4ModelCmdApplyColour<M>::SetNewValue",
	 "NonExistentColour", JustWarning, o.str().c_str());
      return;
    }
  }

  if (cmd == fpComponentCmd) {
    G4double red(0), green(0), blue(0), alpha(0);
    std::istringstream is (newValue);
    is >> red >> green >> blue >> alpha;
    
    G4Colour colour(red, green, blue, alpha);
    myColour = colour;
  }

  Apply(myColour);
}

///////////////////////////////////////////////////////////////////////////
//ApplyBool command
template <typename M>
class G4ModelCmdApplyBool : public G4VModelCommand<M> {

public: // With description

  G4ModelCmdApplyBool(M* model, const G4String& placement, const G4String& cmdName);
  virtual ~G4ModelCmdApplyBool();

  void SetNewValue(G4UIcommand* command, G4String newValue);

protected:

  // Implement in derived class
  virtual void Apply(const G4bool&) = 0;

  G4UIcmdWithABool* Command() {return fpCmd;}

private:

  G4UIcmdWithABool* fpCmd;

};

template <typename M>
G4ModelCmdApplyBool<M>::G4ModelCmdApplyBool(M* model, const G4String& placement, const G4String& cmdName)
  :G4VModelCommand<M>(model)
{
  G4String dir = placement+"/"+model->Name()+"/"+cmdName;
  fpCmd = new G4UIcmdWithABool(dir, this);

  fpCmd->SetParameterName("Bool", false);
}
 
template <typename M>
G4ModelCmdApplyBool<M>::~G4ModelCmdApplyBool()
{  
  delete fpCmd;
}

template <typename M>
void G4ModelCmdApplyBool<M>::SetNewValue(G4UIcommand* cmd, G4String newValue)
{
  Apply(fpCmd->GetNewBoolValue(newValue));
}

///////////////////////////////////////////////////////////////////////////
//ApplyNull command
template <typename M>
class G4ModelCmdApplyNull : public G4VModelCommand<M> {

public: // With description

  G4ModelCmdApplyNull(M* model, const G4String& placement, const G4String& cmdName);

  virtual ~G4ModelCmdApplyNull();

  void SetNewValue(G4UIcommand* command, G4String newValue);

protected:

  // Implement in derived class
  virtual void Apply() = 0;

  G4UIcommand* Command() {return fpCmd;}
  
private:

  G4UIcommand* fpCmd;

};

template <typename M>
G4ModelCmdApplyNull<M>::G4ModelCmdApplyNull(M* model, const G4String& placement, const G4String& cmdName)
  :G4VModelCommand<M>(model)
{
  G4String dir = placement+"/"+model->Name()+"/"+cmdName;
  fpCmd = new G4UIcommand(dir, this);
}

template <typename M>
G4ModelCmdApplyNull<M>::~G4ModelCmdApplyNull() 
{
  delete fpCmd;
}

template <typename M>
void G4ModelCmdApplyNull<M>::SetNewValue(G4UIcommand*, G4String)
{
  Apply();
}

///////////////////////////////////////////////////////////////////////////
//ApplyDouble command
template <typename M>
class G4ModelCmdApplyDouble : public G4VModelCommand<M> {

public: // With description

  G4ModelCmdApplyDouble(M* model, const G4String& placement, const G4String& cmdName);

  virtual ~G4ModelCmdApplyDouble();

  void SetNewValue(G4UIcommand* command, G4String newValue);

protected:

  // Implement in derived class
  virtual void Apply(const G4double&) = 0;

  G4UIcmdWithADouble* Command() {return fpCmd;}

private:

  G4UIcmdWithADouble* fpCmd;

};

template <typename M>
G4ModelCmdApplyDouble<M>::G4ModelCmdApplyDouble(M* model, const G4String& placement, const G4String& cmdName)
  :G4VModelCommand<M>(model)
{
  G4String dir = placement+"/"+model->Name()+"/"+cmdName;

  fpCmd = new G4UIcmdWithADouble(dir, this);
  fpCmd->SetParameterName("Double", false);
}

template <typename M>
G4ModelCmdApplyDouble<M>::~G4ModelCmdApplyDouble()
{  
  delete fpCmd;
}

template <typename M>
void G4ModelCmdApplyDouble<M>::SetNewValue(G4UIcommand* cmd, G4String newValue)
{
  Apply(fpCmd->GetNewDoubleValue(newValue));
}

///////////////////////////////////////////////////////////////////////////
// ApplyInteger command
template <typename M>
class G4ModelCmdApplyInteger : public G4VModelCommand<M> {

public: // With description

  G4ModelCmdApplyInteger(M* model, const G4String& placement, const G4String& cmdName);

  virtual ~G4ModelCmdApplyInteger();

  void SetNewValue(G4UIcommand* command, G4String newValue);

protected:

  // Implement in derived class
  virtual void Apply(const G4int&) = 0;

  G4UIcmdWithAnInteger* Command() {return fpCmd;}

private:

  G4UIcmdWithAnInteger* fpCmd;
};

template <typename M>
G4ModelCmdApplyInteger<M>::G4ModelCmdApplyInteger(M* model, const G4String& placement, const G4String& cmdName)
  :G4VModelCommand<M>(model)
{
  G4String dir = placement+"/"+model->Name()+"/"+cmdName;

  fpCmd = new G4UIcmdWithAnInteger(dir, this);
  fpCmd->SetParameterName("Integer", false);
}

template <typename M>
G4ModelCmdApplyInteger<M>::~G4ModelCmdApplyInteger()
{  
  delete fpCmd;
}

template <typename M>
void G4ModelCmdApplyInteger<M>::SetNewValue(G4UIcommand* cmd, G4String newValue)
{
  Apply(fpCmd->GetNewIntValue(newValue));
}

///////////////////////////////////////////////////////////////////////////
// ApplyString command
template <typename M>
class G4ModelCmdApplyString : public G4VModelCommand<M> {

public: // With description

  G4ModelCmdApplyString(M* model, const G4String& placement, const G4String& cmdName);

  virtual ~G4ModelCmdApplyString();

  void SetNewValue(G4UIcommand* command, G4String newValue);

protected:

  // Implement in derived class
  virtual void Apply(const G4String&) = 0;

  G4UIcmdWithAString* Command() {return fpCmd;}

private:

  G4UIcmdWithAString* fpCmd;

};

template <typename M>
G4ModelCmdApplyString<M>::G4ModelCmdApplyString(M* model, const G4String& placement, const G4String& cmdName)
  :G4VModelCommand<M>(model)
{
  G4String dir = placement+"/"+model->Name()+"/"+cmdName;

  fpCmd = new G4UIcmdWithAString(dir, this);
}

template <typename M>
G4ModelCmdApplyString<M>::~G4ModelCmdApplyString()
{  
  delete fpCmd;
}

template <typename M>
void G4ModelCmdApplyString<M>::SetNewValue(G4UIcommand*, G4String newValue)
{
  Apply(newValue);
}

#endif
