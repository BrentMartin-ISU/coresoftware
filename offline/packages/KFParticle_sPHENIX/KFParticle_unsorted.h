#ifndef KFPARTICLESPHENIX_KFPARTICLEUNSORTED_H
#define KFPARTICLESPHENIX_KFPARTICLEUNSORTED_H

#include "KFParticle_Config.h"

#include <string>

class KFParticle;
class PHCompositeNode;

// TEMPORARY. Holds the code taken out of KFParticle_Tools that KFParticle_eventReconstruction does not use,
// while it is redistributed during the Tools rework. Nothing new should be added here, and this file
// should be gone by the end of the rework.
class KFParticle_unsorted
{
 public:
  // Binds the configuration only (the owner's instance is not constructed yet)
  explicit KFParticle_unsorted(const KFParticle_Config &config)
    : m_config(config)
  {
  }

  // Event-level counts for the nTuple
  void getTracksFromBC(PHCompositeNode *topNode, const int &bunch_crossing, const std::string &vertexMapName, int &nTracks, int &nPVs) const;
  int getTracksFromVertex(PHCompositeNode *topNode, const KFParticle &vertex, const std::string &vertexMapName) const;
  static int getNchargedSiSeedMultiplicity(PHCompositeNode *topNode, const int &bunch_crossing, int verbosity);

  // Stateless
  static float calculateJT(const KFParticle &mother, const KFParticle &daughter);
  static double calculateEllipsoidRadius(int posOrNeg, double sigma_ii, double sigma_jj, double sigma_ij);  // no callers
  static bool findParticle(const std::string &particle);
  static void identify(const KFParticle &particle);

 private:
  const KFParticle_Config &m_config;
};

#endif  // KFPARTICLESPHENIX_KFPARTICLEUNSORTED_H
