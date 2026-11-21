#ifndef KFPARTICLESPHENIX_KFPARTICLETRUTHANDDETTOOLS_H
#define KFPARTICLESPHENIX_KFPARTICLETRUTHANDDETTOOLS_H

#include "KFParticle_utilities.h"

#include <vector>

class PHCompositeNode;
class TTree;
class KFParticle;
class KFParticle_Tools;

class KFParticle_truthAndDetTools
{
 public:
  KFParticle_truthAndDetTools() = default;  // Constructor

  virtual ~KFParticle_truthAndDetTools() = default;

  void allPVInfo(PHCompositeNode *topNode, TTree *m_tree,
                 const KFParticle &motherParticle,
                 std::vector<KFParticle> daughters,
                 std::vector<KFParticle> intermediates);

  void clearVectors();

  void setToolsInstance(KFParticle_Tools *tools) { m_tools = tools; }

 protected:
  // The single KFParticle_Tools instance that holds the user's settings (vertex/track map names,
  // MBD vs SVTX vertex, global vertex use, ...). Set once by KFParticle_sPHENIX so that this class
  // and KFParticle_nTuple never work from a second, default-configured copy.
  KFParticle_Tools *m_tools{nullptr};

  int m_num_tracks_nTuple{0};
  int m_num_intermediate_states_nTuple{0};

  static constexpr int max_tracks{KFParticle_utilities::max_tracks};

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
