//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// Author: Mathieu Karamitros (kara@cenbg.in2p3.fr)
//
// WARNING : This class is released as a prototype.
// It might strongly evolve or even disapear in the next releases.
//
// History:
// -----------
// 10 Oct 2011 M.Karamitros created
//
// -------------------------------------------------------------------

#ifndef G4MOLECULARREACTION_H
#define G4MOLECULARREACTION_H

#include <G4VITReactionProcess.hh>

class G4DNAMolecularReactionTable;
class G4VDNAReactionModel;

class G4DNAMolecularReaction : public G4VITReactionProcess
{
    public:
        /** Default constructor */
        G4DNAMolecularReaction();
        /** Default destructor */
        virtual ~G4DNAMolecularReaction();
        /** Copy constructor
         *  \param other Object to copy from
         */
        G4DNAMolecularReaction(const G4DNAMolecularReaction& other);
        /** Assignment operator
         *  \param other Object to assign from
         *  \return A reference to this
         */
        G4DNAMolecularReaction& operator=(const G4DNAMolecularReaction& other);

        AddClone(G4VITReactionProcess, G4DNAMolecularReaction)

        /** Given two tracks, it tells you whether they can react
         */
        virtual G4bool TestReactibility(const G4Track&,
                                        const G4Track&,
                                        const double stepTime,
                                        const bool userStepTimeLimit) /*const*/ ;

        /** Will generate the products of the two given tracks
         */
        virtual G4ITReactionChange* MakeReaction(const G4Track&, const G4Track&) ;

        inline void SetReactionModel(G4VDNAReactionModel*);
        inline void SetReactionTable(const G4DNAMolecularReactionTable*);

        inline void SetVerbose(int);
        // 1 : only when make reaction is called
        // 2 : both make reaction + test reactibility are called

    protected:
        const G4DNAMolecularReactionTable*& fMolReactionTable;
        G4VDNAReactionModel* fReactionModel;
        G4int fVerbose;
        G4double fReactionRadius ;
        G4double fDistance;
    private:
};

inline void G4DNAMolecularReaction::SetReactionModel(G4VDNAReactionModel* model)
{
    fReactionModel = model;
}

inline void G4DNAMolecularReaction::SetVerbose(int verb)
{
    fVerbose = verb;
}

#endif // G4MOLECULARREACTION_H
