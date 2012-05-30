

#include "G4DNASecondOrderReaction.hh"
#include "G4Molecule.hh"
#include "G4DNAMolecularMaterial.hh"
#include "G4DNADamages.hh"
#include "G4UnitsTable.hh"

void G4DNASecondOrderReaction::Create()
{
    pParticleChange = &fParticleChange;
    enableAtRestDoIt    = false;
    enableAlongStepDoIt = false;
    enablePostStepDoIt  = true;

    SetProcessSubType(60);

    G4VITProcess::SetInstantiateProcessState(false);
    // meaning G4DNASecondOrderReaction contains a class inheriting from G4ProcessState

    fIsInitialized = false;
    fpMolecularConfiguration = 0;
    fpMaterial = 0;
    fReactionRate = -1.;
    fConcentration = -1.;
    fMolarMassOfMaterial = -1.;
    fProposesTimeStep = true;
    fReturnedValue = -1.;
}

G4DNASecondOrderReaction::G4DNASecondOrderReaction(const G4String &aName, G4ProcessType type) :
    G4VITProcess(aName,type),
    InitProcessState(fpSecondOrderReactionState, fpState)
{
    Create();
}

G4DNASecondOrderReaction::G4DNASecondOrderReaction(const G4DNASecondOrderReaction& rhs):
    G4VITProcess(rhs),
    InitProcessState(fpSecondOrderReactionState, fpState)
{
    Create();
}

G4DNASecondOrderReaction::~G4DNASecondOrderReaction()
{
    ;
}
G4DNASecondOrderReaction& G4DNASecondOrderReaction::operator=(const G4DNASecondOrderReaction& rhs)
{
    if (this == &rhs) return *this; // handle self assignment

    //assignment operator
    return *this;
}

G4DNASecondOrderReaction::SecondOrderReactionState::SecondOrderReactionState() : G4ProcessState()
{
    fPreviousTimeAtPreStepPoint = -1;
    fIsInGoodMaterial = false;
}

void G4DNASecondOrderReaction::BuildPhysicsTable(const G4ParticleDefinition&)
{
//    fMolarMassOfMaterial = fpMaterial->GetMaterialPropertiesTable()->GetConstProperty("Molar Mass");

    fpMoleculeDensity = G4DNAMolecularMaterial::Instance()->GetDensityTableFor(fpMaterial);
    fpMoleculeDensity2 = G4DNAMolecularMaterial::Instance()->GetNumMolPerVolTableFor(fpMaterial);
    fMolarMassOfMaterial = fpMaterial->GetMassOfMolecule()*CLHEP::Avogadro*1e3;
    fIsInitialized = true;

//    fpMoleculeConcentration = new std::vector<double>(fpMoleculeDensity->size());
}

void
G4DNASecondOrderReaction::StartTracking(G4Track* track)
{
    G4VProcess::StartTracking(track);
    G4VITProcess::fpState = new SecondOrderReactionState();
    G4VITProcess::StartTracking(track);
}

void
G4DNASecondOrderReaction::SetReaction(const G4MolecularConfiguration* molConf,
                                      const G4Material* mat, double reactionRate)
{
    if(fIsInitialized) {;}//G4Exception(...)
    fpMolecularConfiguration = molConf;
    fpMaterial = mat;
    fReactionRate = reactionRate;
}

G4double G4DNASecondOrderReaction::PostStepGetPhysicalInteractionLength(const G4Track& track,
                                                                        G4double   /*previousStepSize*/,
                                                                        G4ForceCondition* pForceCond)
{
//    G4cout << "G4DNASecondOrderReaction::PostStepGetPhysicalInteractionLength" << G4endl;

    //_______________________________________________________________________
    // Check whether the track is in the good material (maybe composite material)
    const G4Material* material = track.GetMaterial();

    fpMaterialIsComposite = false;

    G4Molecule* mol = GetMolecule(track);
    if(!mol) return DBL_MAX;
    if(mol->GetMolecularConfiguration() != fpMolecularConfiguration)
    {
//        G4cout <<"mol->GetMolecularConfiguration() != fpMolecularConfiguration" << G4endl;
        return DBL_MAX;
    }

    G4double molDensity = (*fpMoleculeDensity)[material->GetIndex()];
    G4double molDensity2 = (*fpMoleculeDensity2)[material->GetIndex()];

    if(molDensity == 0.0) // ie : not found
    {
        if(fpSecondOrderReactionState->fIsInGoodMaterial)
        {
            ResetNumberOfInteractionLengthLeft();
            fpSecondOrderReactionState->fIsInGoodMaterial = false;
        }

//        G4cout << " Material " << fpMaterial->GetName() << " not found "
//               <<" | name of current material : " << material->GetName()
//               << G4endl;

        return DBL_MAX; // Becareful return here !!
    }

//    G4cout << " Va calculer le temps d'interaction " << G4endl;

    fpSecondOrderReactionState->fIsInGoodMaterial = true;

    fConcentration = molDensity/fMolarMassOfMaterial;
    fConcentration2 = molDensity2/CLHEP::Avogadro;

//    if(fConcentration != fConcentration2)
//    {
//        G4cout << "fConcentration != fConcentration2" << G4endl;
//        G4cout << "fConcentration = " << fConcentration << G4endl;
//        G4cout << "fConcentration2 = " << fConcentration2 << G4endl;
//    }

//    G4cout << "Concentration : " << fConcentration / (g/mole)<< G4endl;

    //_______________________________________________________________________
    // Either initialize the lapse of time left
        // meaning => the track enters for the first time in the material
    // or substract the previous time step to the previously calculated lapse of time left
        // meaning => the track has not left this material since the previous call

    G4double previousTimeStep(-1.);

    if(track.GetCurrentStepNumber() > 0)
        previousTimeStep = track.GetGlobalTime() - fpSecondOrderReactionState->fPreviousTimeAtPreStepPoint ;

    fpSecondOrderReactionState->fPreviousTimeAtPreStepPoint = track.GetGlobalTime();

    if ( (previousTimeStep < 0.0) || (fpState->theNumberOfInteractionLengthLeft<=0.0)) {
        // beggining of tracking (or just after DoIt of this process)
        ResetNumberOfInteractionLengthLeft();
    } else if ( previousTimeStep  > 0.0) {
        // subtract NumberOfInteractionLengthLeft
        SubtractNumberOfInteractionLengthLeft(previousTimeStep );
    } else {
        // zero step
        //  DO NOTHING
    }

    // condition is set to "Not Forced"
    *pForceCond = NotForced;

    // get mean free path
    fpState->currentInteractionLength = 1/(fReactionRate*fConcentration2);

    G4double value;
    if (fpState->currentInteractionLength <DBL_MAX) {
        value = fpState->theNumberOfInteractionLengthLeft * (fpState->currentInteractionLength);
    } else {
        value = DBL_MAX;
    }
#ifdef G4VERBOSE
    if (verboseLevel>1){
        G4cout << "G4VITRestDiscreteProcess::PostStepGetPhysicalInteractionLength ";
        G4cout << "[ " << GetProcessName() << "]" <<G4endl;
        track.GetDynamicParticle()->DumpInfo();
        G4cout << " in Material  " <<  track.GetMaterial()->GetName() <<G4endl;
        G4cout << "InteractionLength= " << value/cm <<"[cm] " <<G4endl;
    }
#endif

//    G4cout << "currentInteractionLength : " << fpState->currentInteractionLength << G4endl;
//    G4cout << "Returned time : " << G4BestUnit(value,"Time") << G4endl;

    if(value < fReturnedValue)
        fReturnedValue  = value;

    return value*-1;
    // multiple by -1 to indicate to the tracking system that we are returning a time
}

#include "G4ITStepManager.hh"

G4VParticleChange* G4DNASecondOrderReaction::PostStepDoIt(const G4Track& track,const G4Step& /*step*/)
{
    G4Molecule* molecule = GetMolecule(track);
    G4cout << "___________" << G4endl;
    G4cout << "Returned value : " << G4BestUnit(fReturnedValue,"Time") << G4endl;
    G4cout << "Time Step : " << G4BestUnit(G4ITStepManager::Instance()->GetTimeStep(),"Time") << G4endl;
    G4cout << "Reaction : " << molecule->GetName() << " + " << fpMaterial->GetName() << G4endl;
    fReturnedValue  = DBL_MAX;
    fParticleChange.Initialize(track);
    fParticleChange.ProposeTrackStatus(fStopAndKill);
    G4DNADamages::Instance()->AddIndirectDamage(fpMaterial->GetName(),molecule,track.GetPosition(),track.GetGlobalTime());
    return &fParticleChange;
}
