#include "EletronSD.hh"
#include "EletronHit.hh"

#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EletronSD::EletronSD(G4String name)
: G4VSensitiveDetector(name), fHitsCollection(0), fHCID(-1)
{
    G4String HCname = "eletronColl";
    collectionName.insert(HCname);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EletronSD::~EletronSD()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EletronSD::Initialize(G4HCofThisEvent* hce)
{
    fHitsCollection = new EletronHitsCollection
    (SensitiveDetectorName,collectionName[0]);
    if (fHCID<0)
    { fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection); }
    hce->AddHitsCollection(fHCID,fHitsCollection);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool EletronSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{


//ATENÇÃO
//QUANDO VOCÊ EXECUTE O ./B10eletrons toda a informação que estava lá é apagada
//Para modificar isso comente a linha indicada no arquivo ActionInitialization.cc

//arquivo onde as informaões dos elétrons são salvas
std::ofstream eFile ("e-out.txt",std::ofstream::app);


//informações das secundárias
const std::vector<const G4Track*>* secondary
                                    = step->GetSecondaryInCurrentStep();

//o nome da partícula principal que gerou essas secundárias
G4String nome = step->GetTrack()->GetParticleDefinition()->GetParticleName();

//aqui perorremos a lista de secundárias e pegamos as informações de momento, nome e posição
//name= NOME DA SECUNDÁRIA , nome = NOME DA "PRINCIPAL"
for (size_t lp=0; lp<(*secondary).size(); lp++) {
        G4String name = (*secondary)[lp]->GetDefinition()->GetParticleName();
        G4ThreeVector momento =(*secondary)[lp]->GetMomentum();
        G4ThreeVector worldPos = (*secondary)[lp]->GetPosition();
        //Linha para ver no arquivo se eletrons não estavam sendo produzidos por outros eletrons
        eFile << name << " " << nome << "\n";
        //no momento estou pegando TODOS os elétrons apenas para testar
        //caso necessário, é fácil pedir pra ver se o parent track é uma alpha ou um lítio
        if (name=="e-" && nome!= "e-")
        {eFile << worldPos << " " << momento << "\n";

      }
       if (nome=="e-") {step->GetTrack()->SetTrackStatus(fStopAndKill);}

}


//Confirmei que está funcionando da seguinte forma:
//-Alterei a aprticula lançada para uma alpha
//- descomentei a linha 63 e verifiquei no arquivo de saída que apenas os eletrons produzidos por alpha estavam sendo coletados
//-confirmei se os elétrons estavam realmente sendo "mortos"

//----------------------------------

    // algumas coisas que eu fiz e não quero apagar caso precise depois

       G4StepPoint* preStepPoint = step->GetPreStepPoint();
     // G4ThreeVector worldPos = preStepPoint->GetPosition();
      // G4String nome = step->GetTrack()->GetParticleDefinition()->GetParticleName();
     // G4ThreeVector momento = step->GetTrack()->GetMomentum();

     // if (name=="alpha")
     // {eFile << worldPos << " " << momento << "\n";
     //  step->GetTrack()->SetTrackStatus(fStopAndKill);}

    G4TouchableHistory* touchable = (G4TouchableHistory*)(preStepPoint->GetTouchable());
    G4int copyNo = touchable->GetVolume()->GetCopyNo();
    G4double hitTime = preStepPoint->GetGlobalTime();

    // {
      EletronHit* hit = new EletronHit(copyNo,hitTime);
      fHitsCollection->insert(hit);
    // }

    return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
