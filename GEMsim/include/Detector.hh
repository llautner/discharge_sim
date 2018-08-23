#ifndef DETECTOR_HH_
#define DETECTOR_HH_

#include "InputHandler.hh"
#include "SensitiveDetector.hh"
#include "G4PVPlacement.hh"
#include "G4LogicalVolume.hh"
#include "G4SubtractionSolid.hh"
#include "G4Sphere.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4EllipticalTube.hh"
#include "SensitiveDetector.hh"
#include "G4UserLimits.hh"
#include "G4SystemOfUnits.hh"
#include "CLHEP/Units/SystemOfUnits.h"
#include "G4NistManager.hh"

#include "G4EqMagElectricField.hh"
#include "G4UniformElectricField.hh"

#include "G4MagneticField.hh"
#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4ChordFinder.hh"


class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;

class Detector {
public:
        Detector(G4LogicalVolume*, InputHandler*);
        virtual ~Detector();

        void Create(G4LogicalVolume*);
private:
        G4LogicalVolume* world;
        InputHandler* fInput;

        G4Box *ActiveVolumeBox;
        G4LogicalVolume *ActiveVolumeLog;
        G4Box *PCBBox;
        G4Box *MeshBox;
        G4Tubs *cylinder;
	G4EllipticalTube *ellipse;
        G4LogicalVolume *cylinderLog;
	G4LogicalVolume *ellipseLog;
        G4SubtractionSolid *subtract;
        G4LogicalVolume *PCBLog;
        G4LogicalVolume *MeshLog;
        SensitiveDetector* sensitiveDetector;
        
        G4UserLimits* fStepLimit;
        
        
        G4ElectricField*        fEMfield;
        G4EqMagElectricField*   fEquation;
        G4MagIntegratorStepper* fStepper;
        G4double                fMinStep ;
        G4ChordFinder*          fChordFinder ;
        G4FieldManager*         fFieldManager;
        G4MagInt_Driver*        fIntgrDriver;
};

#endif /* DETECTOR_HH_ */
