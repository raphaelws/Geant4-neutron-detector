#ifndef EletronSD_h
#define EletronSD_h 1

#include "G4VSensitiveDetector.hh"
#include "EletronHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

/// Hodoscope sensitive detector

class EletronSD : public G4VSensitiveDetector
{
public:
    EletronSD(G4String name);
    virtual ~EletronSD();

    virtual void Initialize(G4HCofThisEvent*HCE);
    virtual G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);

private:
    EletronHitsCollection* fHitsCollection;
    G4int fHCID;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
