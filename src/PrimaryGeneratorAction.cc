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
// $Id: PrimaryGeneratorAction.cc 77781 2013-11-28 07:54:07Z gcosmo $
//
/// \file PrimaryGeneratorAction.cc
/// \brief Implementation of the PrimaryGeneratorAction class

#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0)
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  //LINHA PARA TESTAR A PRODUÇÃO DIRETA DE ELETRONS POR ALPHAS
  // G4ParticleDefinition* particle
  //   = particleTable->FindParticle(particleName="alpha");
  G4ParticleDefinition* particle
    = particleTable->FindParticle(particleName="neutron");
  fParticleGun->SetParticleDefinition(particle);

  // particle direction: +Z
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));

  // G4double posZ = -0.*m;
  // G4double posY = -0.*m;
  // G4double posX = -0.*m;
  // fParticleGun->SetParticlePosition(G4ThreeVector(posX,posY,0.5*m));

     fParticleGun->SetParticleEnergy(0.04180*eV);
    //fParticleGun->SetParticleEnergy(50.*eV);
}

//-----------------------------------------------------------------------

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

//-----------------------------------------------------------------------

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // G4double posZ = -0.*m;
  G4double posY = -0.*m;
  G4double posX = -0.*m;
  // fParticleGun->SetParticlePosition(G4ThreeVector(posX,posY,-0.002*m));
    fParticleGun->SetParticlePosition(G4ThreeVector(posX,posY,-0.5*m));
  //this function is called at the begining of each event
  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//-----------------------------------------------------------------------
