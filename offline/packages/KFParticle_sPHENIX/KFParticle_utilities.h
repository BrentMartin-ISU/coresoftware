#ifndef KFPARTICLESPHENIX_KFPARTICLEUTILITIES_H
#define KFPARTICLESPHENIX_KFPARTICLEUTILITIES_H

#include <string>

class KFParticle;
class PHCompositeNode;
class SvtxTrack;
class SvtxTrackMap;

// Stateless helpers shared across the package
class KFParticle_utilities
{
 public:
  KFParticle_utilities() = delete;

  // Size of the per-daughter branch arrays
  static constexpr int max_tracks{20};

  static SvtxTrack *getTrack(unsigned int track_id, SvtxTrackMap *trackmap);

  // Beam crossing of the vertex the candidate was built with; -100 if it can't be found
  static int getPVID(PHCompositeNode *topNode, const KFParticle &kfpvertex,
                     bool dontUseGlobalVertex, bool useMbdVertex, const std::string &vtxMapName);
};

#endif  // KFPARTICLESPHENIX_KFPARTICLEUTILITIES_H
