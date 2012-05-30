#include "G4MoleculeCounter.hh"
#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"

G4MoleculeCounter::G4MoleculeCounter()
{
    fUse = FALSE;
    fVerbose = 0 ;
}

G4MoleculeCounter* G4MoleculeCounter::fpInstance = 0;

G4MoleculeCounter* G4MoleculeCounter::GetMoleculeCounter()
{
    if(!fpInstance)
        fpInstance = new G4MoleculeCounter();

    return fpInstance;
}

void G4MoleculeCounter::AddAMoleculeAtTime(const G4Molecule& molecule, G4double time)
{
    if(fVerbose > 1)
    {
        G4cout<<"G4MoleculeCounter::AddAMoleculeAtTime : "<< molecule.GetName()
              << " at time : " << G4BestUnit(time, "Time") <<G4endl;
    }

    if(fDontRegister[molecule.GetDefinition()]) return ;

    // G4double pstime = time/picosecond;
    //
    // G4double fractpart=-1, intpart=-1;
    // fractpart = modf (pstime , &intpart);
    //
    // if(pstime<10.1)
    // {
    // fractpart *= 10 ;
    // fractpart = floor(fractpart)/10;
    // pstime = intpart+fractpart;
    // }
    //
    // else
    // {
    // pstime = intpart;
    // }
    // time = pstime*picosecond ;

    if(fVerbose)
    {
        G4cout<<"-------------------------"<<G4endl ;
        G4cout << "G4MoleculeCounter::AddAMoleculeAtTime " << G4endl;
        G4cout<<"!! Molecule = " << molecule.GetName() << G4endl;
        G4cout<<"!! At Time = "<< G4BestUnit(time, "Time") <<G4endl;
    }

    CounterMapType::iterator counterMap_i = fCounterMap.find(molecule) ;

    if(counterMap_i == fCounterMap.end())
    {
        if(fVerbose)	G4cout << " !! ***** Map is empty " << G4endl;
        fCounterMap[molecule][time] = 1;
    }
    else if(counterMap_i->second.empty())
    {
        if(fVerbose)	G4cout << " !! ***** Map is empty " << G4endl;
        counterMap_i->second[time] = 1;
    }
    else
    {
        NbMoleculeAgainstTime::iterator end = counterMap_i->second.end();
        end--;

        if(fVerbose)
            G4cout<<"!! End Time = "<< G4BestUnit(end->first, "Time") <<G4endl;

        if(end->first <= time)
        {
            counterMap_i->second[time]=end->second + 1;
        }
        else
        {
            NbMoleculeAgainstTime::iterator it = counterMap_i->second.lower_bound(time);

            while(it->first > time && it!=counterMap_i->second.begin())
            {
                if(fVerbose)
                    G4cout<<"!!  ********** Is going back!!!!"<<G4endl;
                it--;
            }

            if(it==counterMap_i->second.begin() && it->first > time)
            {
                if(fVerbose)
                    G4cout<<"!!  ********** Illegal !!!!"<<G4endl;
                return ;
            }

            if(fVerbose)
            {
                G4cout<<"!! PREVIOUS NB = "<< it->second <<G4endl;
                G4cout<<"!! PREVIOUS TIME = "<< G4BestUnit(it->first,"Time") <<G4endl;
            }
            counterMap_i->second[time]=it->second + 1;
        }
    }

    if(fVerbose)
        G4cout<<"!! NB = "<< fCounterMap[molecule][time]<<G4endl;
}

void G4MoleculeCounter::RemoveAMoleculeAtTime(const G4Molecule& molecule, G4double time)
{
    if(fVerbose > 1)
    {
        G4cout<<"G4MoleculeCounter::RemoveAMoleculeAtTime : "<< molecule.GetName()
             << " at time : " << G4BestUnit(time,"Time") <<G4endl;
    }

    if(fDontRegister[molecule.GetDefinition()]) return ;

    if(fCounterMap[molecule].empty())
    {
        molecule.PrintState();
        G4String errMsg = "You are trying to remove molecule "
                          + molecule.GetName()
                          +" from the counter while this kind of molecules has not been registered yet";
        G4Exception("G4MoleculeCounter::RemoveAMoleculeAtTime","",FatalErrorInArgument, errMsg);

        return;
    }
    else
    {
        NbMoleculeAgainstTime::iterator it ;

        if(fCounterMap[molecule].size() == 1)
        {
            it = fCounterMap[molecule].begin() ;
        }
        else
        {
            it = fCounterMap[molecule].lower_bound(time);
        }


        if(fVerbose)
            G4cout<<"-------------------------"<<G4endl ;

        if(it==fCounterMap[molecule].end())
        {
            if(fVerbose)
                G4cout << " ********** NO ITERATOR !!!!!!!!! " << G4endl;
            it++;
            if(time<it->first)
            {
                G4String errMsg = "There was no "+ molecule.GetName()
                        + " record at the time or even before the time asked";
                G4Exception("G4MoleculeCounter::RemoveAMoleculeAtTime","",FatalErrorInArgument, errMsg);
            }
        }

        if(fVerbose)
        {
            G4cout << "G4MoleculeCounter::RemoveAMoleculeAtTime " << G4endl;
            G4cout<<"!! Molecule = " << molecule.GetName() << G4endl;
            G4cout<<"!! At Time = "<< G4BestUnit(time,"Time") <<G4endl;
            G4cout<<"!! PREVIOUS TIME = "<< G4BestUnit(it->first,"Time")<<G4endl;
            G4cout<<"!! PREVIOUS Nb = "<< it->second <<G4endl;
        }

        // If valgrind problem on the line below, it means that the pointer "it"
        // points nowhere
        if(fCounterMap[molecule].value_comp()(*it, *fCounterMap[molecule].begin()))
        {
            if(fVerbose)
                G4cout<<"!! ***** In value_comp ... " << G4endl;
            it++;
            if(time<it->first)
            {
                G4String errMsg = "There was no "+ molecule.GetName()
                        + " record at the time or even before the time asked";
                G4Exception("G4MoleculeCounter::RemoveAMoleculeAtTime","",FatalErrorInArgument, errMsg);
            }
        }

        while(it->first > time && it!=fCounterMap[molecule].begin())
        {
            if(fVerbose)
            {
                G4cout<<"!! ***** Is going back!!!!"<<G4endl;
                G4cout<<"!! PREVIOUS TIME = "<< G4BestUnit(it-> first,"Time") <<G4endl;
            }
            it--;
        }

        if(it==fCounterMap[molecule].begin() && it->first > time)
        {
            if(fVerbose)
                G4cout<<"!!  ********** Illegal !!!!"<<G4endl;
            return ;
        }

        if(fVerbose)
        {
            G4cout<<"!! PREVIOUS NB = "<< (*it).second <<G4endl;
            G4cout<<"!! PREVIOUS TIME = "<< G4BestUnit(it->first,"Time")<<G4endl;
        }
        fCounterMap[molecule][time]=it->second - 1;
    }
    if(fVerbose)
    {
        G4cout<<"!! NB = "<< fCounterMap[molecule][time]<<G4endl;
    }
}

vector<G4Molecule> G4MoleculeCounter::RecordMolecules()
{
    if(fVerbose > 1)
    {
        G4cout<<"Entering in G4MoleculeCounter::RecordMolecules"<<G4endl;
    }

    CounterMapType::iterator it;
    vector<G4Molecule> output;

    for(it = fCounterMap.begin() ; it != fCounterMap.end() ; it++)
    {
        output.push_back((*it).first);
    }
    return output;
}

