#include "DetectorConstruction.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"
#include "G4PSPassageCellFlux.hh"

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction()
{
}

DetectorConstruction::~DetectorConstruction()
{
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Define materials
  G4NistManager* nistManager = G4NistManager::Instance();
  G4Material* Fe = nistManager->FindOrBuildMaterial("G4_Fe");
  G4Material* S = nistManager->FindOrBuildMaterial("G4_S");
  G4Material* B = nistManager->FindOrBuildMaterial("G4_B");
  G4Material* vacuum = nistManager->FindOrBuildMaterial("G4_Galactic");

  // Define dimensions
  G4double plateThickness = 2.*cm;
  G4double plateSizeX = 20.*cm;
  G4double plateSizeY = 20.*cm;
  G4double detectorRadius = 5.*cm;
  G4double detectorDistance = 20.*cm;

  // Define positions
  G4ThreeVector platePos = G4ThreeVector(0., 0., 0.);
  G4ThreeVector detectorPos = G4ThreeVector(0., 0., -detectorDistance);

  // Define shapes
  G4Box* plateSolid = new G4Box("plateSolid", plateSizeX/2., plateSizeY/2., plateThickness/2.);
  G4Tubs* detectorSolid = new G4Tubs("detectorSolid", 0., detectorRadius, detectorThickness/2., 0., 360.*deg);

  // Define logical volumes
  G4LogicalVolume* plateLV = new G4LogicalVolume(plateSolid, Fe, "plateLV");
  G4LogicalVolume* detectorLV = new G4LogicalVolume(detectorSolid, vacuum, "detectorLV");

  // Place the plate in the center of the world
  G4VPhysicalVolume* platePV = new G4PVPlacement(0, platePos, plateLV, "platePV", 0, false, 0);

  // Place the detector above the plate
  G4RotationMatrix* detectorRot = new G4RotationMatrix();
  detectorRot->rotateX(90.*deg);
  G4VPhysicalVolume* detectorPV = new G4PVPlacement(detectorRot, detectorPos, detectorLV, "detectorPV", plateLV, false, 0);

  // Set the detector as a sensitive detector and calculate the neutron flux
  G4VSensitiveDetector* detectorSD = new G4PSPassageCellFlux("detectorSD", 0);
  detectorLV->SetSensitiveDetector(detectorSD);
  G4SDManager::GetSDMpointer()->AddNewDetector(detectorSD);

  // Return the physical volume of the plate
  return platePV;
}
