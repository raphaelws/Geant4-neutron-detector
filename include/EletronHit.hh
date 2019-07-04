#ifndef EletronHit_h
#define EletronHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"

class G4AttDef;
class G4AttValue;

class EletronHit : public G4VHit
 {
 public:
  EletronHit(G4int i,G4double t);
  virtual ~EletronHit() {}

  inline void *operator new(size_t);
  inline void operator delete(void*aHit);

  void Print();

  G4int GetID() const { return fId; }

  void SetTime(G4double val) { fTime = val; }
  G4double GetTime() const { return fTime; }

 private:
  G4int fId;
  G4double fTime;
};

 typedef G4THitsCollection<EletronHit> EletronHitsCollection;

 extern G4ThreadLocal G4Allocator<EletronHit>* EletronHitAllocator;

 inline void* EletronHit::operator new(size_t)
 {
    if (!EletronHitAllocator)
      EletronHitAllocator = new G4Allocator<EletronHit>;
    return (void*)EletronHitAllocator->MallocSingle();
 }

 inline void EletronHit::operator delete(void*aHit)
 {
    EletronHitAllocator->FreeSingle((EletronHit*) aHit);
 }


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
