#include "PrimaryGeneratorAction.hh"
#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(), fParticleGun(0)
{
  fParticleGun = new G4GeneralParticleSource();
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // Define the particle and its energy
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* neutron = particleTable->FindParticle("neutron");
  G4double energy = GenerateEnergy();

  // Set the particle properties and direction
  fParticleGun->SetParticleDefinition(neutron);
  fParticleGun->SetParticleEnergy(energy);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));

  // Generate the primary particle and add it to the event
  fParticleGun->GeneratePrimaryVertex(anEvent);
}

G4double PrimaryGeneratorAction::GenerateEnergy()
{
  // Generate neutron energy using Watt fission spectrum formula
  G4double a = 1.18*MeV;
  G4double b = 1.03419*MeV^-1;
  G4double E;
  do {
    E = G4RandExponential::shoot(a);
  } while (G4UniformRand() > (b/a)*sinh(sqrt(b*E)/a)/E);
  
  return E;
}
