#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4SDManager.hh"
#include "G4Sphere.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

#include "DetectorConstruction.hh"
#include "NeutronSD.hh"

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction()
{ }

DetectorConstruction::~DetectorConstruction()
{ }

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Define materials
  G4NistManager* nist = G4NistManager::Instance();
  G4Material* Fe = nist->FindOrBuildMaterial("G4_Fe");
  G4Material* S = nist->FindOrBuildMaterial("G4_S");
  G4Material* B = nist->FindOrBuildMaterial("G4_B");

  // Define dimensions of the plate
  G4double plateX = 20*cm;
  G4double plateY = 20*cm;
  G4double plateZ = 2*cm;

  // Create box shape for the plate
  G4Box* plateBox = new G4Box("plateBox", plateX/2, plateY/2, plateZ/2);

  // Create logical volume for the plate and add the materials
  G4LogicalVolume* plateLV = new G4LogicalVolume(plateBox, Fe, "plateLV");
  plateLV->SetMaterialComposition(new G4ElementRatio(Fe, 78), new G4ElementRatio(S, 9), new G4ElementRatio(B, 13));
  
  // Define dimensions of the detector
  G4double detectorRadius = 5*cm;
  G4double detectorThickness = 1*mm;

  // Create sphere shape for the detector
  G4Sphere* detectorSphere = new G4Sphere("detectorSphere", 0, detectorRadius, 0, 360*deg, 0, 180*deg);

  // Create logical volume for the detector and set its material
  G4LogicalVolume* detectorLV = new G4LogicalVolume(detectorSphere, nist->FindOrBuildMaterial("G4_Galactic"), "detectorLV");

  // Create a sensitive detector for neutrons
  G4String neutronSDname = "neutronSD";
  NeutronSD* neutronSD = new NeutronSD(neutronSDname, "neutronHitsCollection");
  G4SDManager::GetSDMpointer()->AddNewDetector(neutronSD);

  // Assign the sensitive detector to the logical volume of the detector
  detectorLV->SetSensitiveDetector(neutronSD);

  // Set visualization attributes for the detector
  G4VisAttributes* visAttr = new G4VisAttributes(G4Colour::Yellow());
  visAttr->SetVisibility(true);
  detectorLV->SetVisAttributes(visAttr);

  // Place the plate in the center of the world
  G4VPhysicalVolume* platePV = new G4PVPlacement(0, G4ThreeVector(), plateLV, "platePV", 0, false, 0);

  // Place the detector on the thin side of the plate, 20cm away from it
  G4RotationMatrix* detectorRot = new G4RotationMatrix();
  detectorRot->rotateX(90*deg);
  G4ThreeVector detectorPos(0, 0, plateZ/2 + detectorRadius + 20*cm + detectorThickness/2);
  G4VPhysicalVolume* detectorPV = new G4PVPlacement(detectorRot, detectorPos, detectorLV, "detectorPV", plateLV, false, 0);

  // Return the physical volume of the plate
  return platePV;
}
