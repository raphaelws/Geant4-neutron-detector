#ifndef CellParameterisation_H
#define CellParameterisation_H 1

#include "globals.hh"
#include "G4VPVParameterisation.hh"
class G4VPhysicalVolume;

/// EM Calorimeter cell parameterisation

class CellParameterisation : public G4VPVParameterisation
{
public:
    CellParameterisation();
    virtual ~CellParameterisation();

    virtual void ComputeTransformation(
                   const G4int copyNo,G4VPhysicalVolume *physVol) const;

private:
    G4double fXCell[80];
    G4double fYCell[80];
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
