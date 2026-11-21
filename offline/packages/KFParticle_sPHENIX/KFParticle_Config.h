#ifndef KFPARTICLESPHENIX_KFPARTICLECONFIG_H
#define KFPARTICLESPHENIX_KFPARTICLECONFIG_H

#include <string>

// Configuration for KFParticle, as plain read-only data.
// KFParticle_sPHENIX owns the only writable instance (a private member, set through its setters).
// Every other class holds a private const reference to it, so there the fields can be read but not written.
struct KFParticle_Config
{
  // DST output
  bool saveDST = false;
  bool writeTrackContainer = true;
  bool writeParticleContainer = true;
  std::string containerName;
  std::string originTrackMapNodeName = "SvtxTrackMap";

  // PID (dE/dx)
  bool usePID = false;
  bool useLocalPIDFile = false;
  std::string localPIDFilename;
  float dEdxBandWidth = 0.2;  // accepted fraction around the expected dE/dx

  // nTuple output
  bool saveOutput = true;
  std::string outputName = "outputData.root";
  bool lightweightNTuple = false;      // reduced set of branches
  bool lightweightNTupleToQA = false;  // reduced nTuple registered with the QA histogram manager instead of a file
  bool truthMatching = false;
  bool triggerInfo = false;
  bool detectorInfo = false;
  bool detailedTracking = true;        // residuals, layers and sensor IDs, when detectorInfo is on
  bool caloInfo = false;
  bool detailedCalorimetry = false;    // EMCal projection and cluster position, chi2, nTowers, when caloInfo is on
  bool calo5x5CellInfo = false;        // towers in the 5x5 cell around the matched EMCal cluster, when caloInfo is on
  bool requireTrackEMCalMatch = false; // only fill candidates whose tracks all match an EMCal cluster
  bool allPVInfo = false;
  bool useCentrality = false;
  bool dEdxInfo = false;

  // Vertexing and candidate selection
  bool constrainToPrimaryVertex = false;
  bool selectMotherByMassError = true;  // best candidate = smallest mass error; false = closest to its PV (only with the PV constraint, mother only)

  // Magnetic field
  std::string magFieldFile = "FIELDMAP_TRACKING";  // file path or CDB tag; getField() replaces a tag with the resolved path

  // Track-EMCal matching
  int track2CaloVerbosity = 0;
  float emcalRadius = 100.70;      // radius of the track state used for the match
  float emcalEnergyLowCut = 0.2;   // minimum cluster energy
  float emcalDPhiCutLow = -0.15;
  float emcalDPhiCutHigh = 0.15;
  float emcalDZCutLow = -10;
  float emcalDZCutHigh = 10;
};

#endif  // KFPARTICLESPHENIX_KFPARTICLECONFIG_H
