#ifndef KFPARTICLESPHENIX_KFPARTICLETRUTHANDDETTOOLS_H
#define KFPARTICLESPHENIX_KFPARTICLETRUTHANDDETTOOLS_H

#include "KFParticle_utilities.h"

// Cluster
#include <calobase/RawClusterDefs.h>

#include <cmath>
#include <limits>
#include <string>
#include <vector>

class PHCompositeNode;
class RawCluster;
class RawClusterContainer;
class RawTower;
class RawTowerContainer;
class RawTowerGeomContainer;
class SvtxTrack;
class SvtxTrackMap;
class TowerInfoContainer;
class TTree;
class KFParticle;
class KFParticle_Tools;

class KFParticle_truthAndDetTools
{
 public:
  KFParticle_truthAndDetTools() = default;  // Constructor

  virtual ~KFParticle_truthAndDetTools() = default;

  virtual void initializeCaloBranches(TTree *m_tree, int daughter_id, const std::string &daughter_number);
  virtual void fillCaloBranch(PHCompositeNode *topNode, TTree *m_tree, const KFParticle &daughter, int daughter_id, bool &isTrackEMCalmatch, const KFParticle &vertex);
  void Get5x5CellInfo(
    RawClusterDefs::keytype key_in, 
    float &E3x3, 
    float &E5x5,
    std::vector<unsigned int> &v_emcal_5x5Cell_Phi,
    std::vector<unsigned int> &v_emcal_5x5Cell_Eta,
    std::vector<float> &v_emcal_5x5Cell_E
  );

  void allPVInfo(PHCompositeNode *topNode, TTree *m_tree,
                 const KFParticle &motherParticle,
                 std::vector<KFParticle> daughters,
                 std::vector<KFParticle> intermediates);

  void clearVectors();

  // Verbosity
  int m_track2caloVerbosity{0};
  void setVerbosityTrack2Calo(int verb){
    m_track2caloVerbosity = verb;
  }
 
  // Helper functions
  // Wrap an azimuthal difference into [-pi, pi]
  float PiRange(float deltaPhi) { return std::remainder(deltaPhi, 2 * M_PI); }

  void setToolsInstance(KFParticle_Tools *tools) { m_tools = tools; }

  // Functions to set cuts on track-emcal matching 
  void set_emcal_radius_user(float set_variable) { m_emcal_radius_user = set_variable; }
  void set_emcal_e_low_cut(float set_variable) { m_emcal_e_low_cut = set_variable; }
  void set_dphi_cut_low(float set_variable) { m_dphi_cut_low = set_variable; }
  void set_dphi_cut_high(float set_variable) { m_dphi_cut_high = set_variable; }
  void set_dz_cut_low(float set_variable) { m_dz_cut_low = set_variable; }
  void set_dz_cut_high(float set_variable) { m_dz_cut_high = set_variable; }

 protected:
  bool m_get_detailed_calorimetry{false};
  bool m_get_calo_5x5_cell_info{false};

  // The single KFParticle_Tools instance that holds the user's settings (vertex/track map names,
  // MBD vs SVTX vertex, global vertex use, ...). Set once by KFParticle_sPHENIX so that this class
  // and KFParticle_nTuple never work from a second, default-configured copy.
  KFParticle_Tools *m_tools{nullptr};

  SvtxTrackMap *dst_trackmap{nullptr};
  SvtxTrack *track{nullptr};

  int m_num_tracks_nTuple{0};
  int m_num_intermediate_states_nTuple{0};

  static constexpr int max_tracks{KFParticle_utilities::max_tracks};

  float m_emcal_radius_user{100.70};
  float m_ihcal_radius_user{117};
  float m_ohcal_radius_user{177.423};

  float m_emcal_e_low_cut{0.2};
  float m_ihcal_e_low_cut{0.01};
  float m_ohcal_e_low_cut{0.01};
  float m_dphi_cut_low{-0.15};
  float m_dphi_cut_high{0.15};
  float m_dz_cut_low{-10};
  float m_dz_cut_high{10};

  // Vectors for calo info
  float detector_emcal_deltaphi[max_tracks]{std::numeric_limits<float>::quiet_NaN()};
  float detector_emcal_deltaeta[max_tracks]{std::numeric_limits<float>::quiet_NaN()};
  float detector_emcal_deltaz[max_tracks]{std::numeric_limits<float>::quiet_NaN()};
  float detector_emcal_energy_3x3[max_tracks]{std::numeric_limits<float>::quiet_NaN()};
  float detector_emcal_energy_5x5[max_tracks]{std::numeric_limits<float>::quiet_NaN()};
  float detector_emcal_cluster_energy[max_tracks]{std::numeric_limits<float>::quiet_NaN()};
  float detector_ihcal_deltaphi[max_tracks]{std::numeric_limits<float>::quiet_NaN()};
  float detector_ihcal_deltaeta[max_tracks]{std::numeric_limits<float>::quiet_NaN()};
  float detector_ihcal_energy_3x3[max_tracks]{std::numeric_limits<float>::quiet_NaN()};
  float detector_ihcal_energy_5x5[max_tracks]{std::numeric_limits<float>::quiet_NaN()};
  float detector_ihcal_cluster_energy[max_tracks]{std::numeric_limits<float>::quiet_NaN()};
  float detector_ohcal_deltaphi[max_tracks]{std::numeric_limits<float>::quiet_NaN()};
  float detector_ohcal_deltaeta[max_tracks]{std::numeric_limits<float>::quiet_NaN()};
  float detector_ohcal_energy_3x3[max_tracks]{std::numeric_limits<float>::quiet_NaN()};
  float detector_ohcal_energy_5x5[max_tracks]{std::numeric_limits<float>::quiet_NaN()};
  float detector_ohcal_cluster_energy[max_tracks]{std::numeric_limits<float>::quiet_NaN()};
  // Detailed Calo Info
  float detector_emcal_projection_z[max_tracks]{std::numeric_limits<float>::quiet_NaN()};
  float detector_emcal_projection_eta[max_tracks]{std::numeric_limits<float>::quiet_NaN()};
  float detector_emcal_projection_phi[max_tracks]{std::numeric_limits<float>::quiet_NaN()};
  float detector_emcal_cluster_z[max_tracks]{std::numeric_limits<float>::quiet_NaN()};
  float detector_emcal_cluster_eta[max_tracks]{std::numeric_limits<float>::quiet_NaN()};
  float detector_emcal_cluster_phi[max_tracks]{std::numeric_limits<float>::quiet_NaN()};
  float detector_emcal_chi2[max_tracks]{std::numeric_limits<float>::quiet_NaN()};
  unsigned int detector_emcal_ntowers[max_tracks]{std::numeric_limits<unsigned int>::quiet_NaN()};
  // 5x5 grid
  std::vector<unsigned int> detector_emcal_5x5Cell_Phi[max_tracks];
  std::vector<unsigned int> detector_emcal_5x5Cell_Eta[max_tracks];
  std::vector<float> detector_emcal_5x5Cell_E[max_tracks];

  // Containers for calo info
  RawTowerGeomContainer *EMCalGeo{nullptr};
  RawClusterContainer *clustersEM{nullptr};
  TowerInfoContainer *_towersEM{nullptr};
  RawTowerGeomContainer *IHCalGeo{nullptr};
  RawClusterContainer *clustersIH{nullptr};
  RawTowerContainer *_towersIH{nullptr};
  RawTowerGeomContainer *OHCalGeo{nullptr};
  RawClusterContainer *clustersOH{nullptr};
  RawTowerContainer *_towersOH{nullptr};

  std::vector<float> allPV_x;
  std::vector<float> allPV_y;
  std::vector<float> allPV_z;
  std::vector<float> allPV_mother_PV_DCA;
  std::vector<float> allPV_mother_PV_DCA_StdDev;
  std::vector<float> allPV_daughter_PV_DCA[max_tracks];
  std::vector<float> allPV_daughter_PV_DCA_StdDev[max_tracks];
  std::vector<float> allPV_intermediates_PV_DCA[max_tracks];
  std::vector<float> allPV_intermediates_PV_DCA_StdDev[max_tracks];
};

#endif  // KFPARTICLESPHENIX_KFPARTICLETRUTHANDDETTOOLS_H
