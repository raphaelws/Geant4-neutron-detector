#include "DetectorConstruction.hh"
#include "MagneticField.hh"
#include "CellParameterisation.hh"
#include "EletronSD.hh"

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4AutoDelete.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4MaterialTable.hh"
#include "G4NistManager.hh"

#include "G4VSolid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4PVReplica.hh"
#include "G4UserLimits.hh"

#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4GenericMessenger.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal MagneticField* DetectorConstruction::fMagneticField = 0;
G4ThreadLocal G4FieldManager* DetectorConstruction::fFieldMgr = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(),
  fMessenger(0),
  fEletron1Logical(0),
  fVisAttributes()

{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
    delete fArmRotation;
    delete fMessenger;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    // Materiais
    ConstructMaterials();
    G4Material* air = G4Material::GetMaterial("G4_AIR");
    G4Material* vacuum = G4Material::GetMaterial("G4_Galactic");
    G4Material* argonGas = G4Material::GetMaterial("G4_Ar");

    //isotopo de B10
    //----------Jeito que eu defini o B10-------------------
    G4Material* el_b10  = new G4Material("B10", 5.,10.013*g/mole, 2.34*g/cm3);
    //---------------------------------------------------

    //-----------------Jeito que o Renan definiu---------------
    // G4Isotope* isoB10 = new G4Isotope("B10", 5, 10, 10.012937*g/mole);
    // G4Element* el_b10 = new G4Element("elementB10","B10",1);
    // el_b10->AddIsotope(isoB10,100*perCent);
    // G4Material* boron_layer_material = new G4Material("B10",2.34*g/cm3,1);
    // boron_layer_material->AddElement(el_b10,1);
    //------------------------------------------------------------------

    G4bool checkOverlaps = true;

    // Tamanho do mundo
    G4double world_sizeX=1.*m;
    G4double world_sizeY=1.*m;
    G4double world_sizeZ=1.*m;

    G4VSolid* worldSolid
     = new G4Box("worldBox",0.5*world_sizeX, 0.5*world_sizeY, 0.5*world_sizeZ);
   G4LogicalVolume* worldLogical
     = new G4LogicalVolume(worldSolid,air,"worldLogical");
   G4VPhysicalVolume* worldPhysical
     = new G4PVPlacement(0,G4ThreeVector(),worldLogical,"worldPhysical",0,
                         false,0,checkOverlaps);



//dimensões da placa de B10
G4double b10plate_sizeX=1.*m;
G4double b10plate_sizeY=1.*m;
G4double b10plate_sizeZ=0.0000025*m;
//Como o neutron é lançado na direção z, apenas a dimensão da placa z que importa
//Acredito que o valor usado é o de maior eficiência, porém foi passado pra mim 2 micro m e não 2,5 micro m
//outras dimensões para testes
//G4double b10plate_sizeZ=0.25*m;
//G4double b10plate_sizeZ=0.000004*m;

//Dimensões do Sensistive Detector de elétrons
G4double sd_sizeX=1.*m;
G4double sd_sizeY=1.*m;
G4double sd_sizeZ=(0.5*world_sizeZ-0.5*b10plate_sizeZ);
G4double delta=(0.5*sd_sizeZ+0.5*b10plate_sizeZ);
//não me lembro pra que serve esse delta e tenho medo de deletar
// G4double sd_sizeZ=(world_sizeZ-b10plate_sizeZ);
// G4double delta=(sd_sizeZ+b10plate_sizeZ);


//usei essa variavel para definir o ponto onde ia colocar o detector, mas não uso ela mais
G4double colocandoZ2 = b10plate_sizeZ;

//fazendo
    G4VSolid* eletron1Solid
      = new G4Box("eletron1Box",0.5*sd_sizeX,0.5*sd_sizeY,0.5*sd_sizeZ);
    fEletron1Logical
      = new G4LogicalVolume(eletron1Solid,argonGas,"eletron1Logical");
      new G4PVPlacement(0,G4ThreeVector(0.,0.,delta),fEletron1Logical,
                        "eletron1Physical",worldLogical,
                        false,0,checkOverlaps);




                        // B10 Plate



                        G4double colocandoZ = (-world_sizeZ+(5*b10plate_sizeZ));


      G4VSolid* b10Solid
      = new G4Box("b10Solid",0.5*b10plate_sizeX, 0.5*b10plate_sizeY, 0.5*b10plate_sizeZ);
      // Definido com o meu B10
      //--------------------------
      G4LogicalVolume* b10Logical
      = new G4LogicalVolume(b10Solid,el_b10,"b10Logical");
      //------------------- Comente caso for usar o do Renan
      // Definido com o B10 do Renan
      //------------------------------
      // G4LogicalVolume* b10Logical = new G4LogicalVolume(b10Solid,boron_layer_material,"b10Logical");
      // Descomente caso for usar o do Renan
      new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),b10Logical,
      "b10Physical",worldLogical,
       false,0,checkOverlaps);



    //visualização
    //Descomente o que quiser que apareça desenhado na cor especificada

    //O MUNDO
     G4VisAttributes* visAttributes = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
    //    visAttributes->SetVisibility(true);
    //    worldLogical->SetVisAttributes(visAttributes);
    //    fVisAttributes.push_back(visAttributes);

    //SD de elétrons
    visAttributes = new G4VisAttributes(G4Colour(0.8888,0.0,0.0));
    fEletron1Logical->SetVisAttributes(visAttributes);
        fVisAttributes.push_back(visAttributes);

    //Placa de B10
    visAttributes = new G4VisAttributes(G4Colour(0.9,0.9,0.0));
    visAttributes->SetVisibility(true);
    b10Logical->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);


    return worldPhysical;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 //
 void DetectorConstruction::ConstructSDandField()
	 {
	  //----------------------------sensitive detector de eletrons -----------------------------------------------------
	  G4SDManager* SDman = G4SDManager::GetSDMpointer();
	  G4String SDname;

	  G4VSensitiveDetector* eletron1 = new EletronSD(SDname="/eletron1");
	  SDman->AddNewDetector(eletron1);
	  fEletron1Logical->SetSensitiveDetector(eletron1);

	  // Register the field and its manager for deleting
	  }
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructMaterials()
{
    G4NistManager* nistManager = G4NistManager::Instance();

    //testando se o meu erro não foi definidir o B10 aqui
    //Caso de errado comente o código e pergunte como fazer na próxima reunião ************************LEMBRETE***********************************

    G4Material* el_b10  = new G4Material("B10", 5.,10.013*g/mole, 2.34*g/cm3);

    nistManager->FindOrBuildMaterial("el_b10");
    //------------------------------------------------------

    // Air
    nistManager->FindOrBuildMaterial("G4_AIR");

    // Argon gas
    nistManager->FindOrBuildMaterial("G4_Ar");
    // With a density different from the one defined in NIST
    // G4double density = 1.782e-03*g/cm3;
    // nistManager->BuildMaterialWithNewDensity("_Ar","G4_Ar",density);
    // !! cases segmentation fault


    // Vacuum "Galactic"
     nistManager->FindOrBuildMaterial("G4_Galactic");

    G4cout << G4endl << "Materiais definidos : " << G4endl << G4endl;
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}
