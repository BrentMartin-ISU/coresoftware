// TEMPORARY: see KFParticle_unsorted.h. The function bodies are moved from KFParticle_Tools.cc unchanged,
// except that getTracksFromBC and getTracksFromVertex use local node pointers instead of writing the
// KFParticle_Tools m_dst_* members (nothing read those values after these calls).

#include "KFParticle_unsorted.h"

#include <globalvertex/GlobalVertex.h>
#include <globalvertex/GlobalVertexMap.h>
#include <globalvertex/SvtxVertex.h>
#include <globalvertex/SvtxVertexMap.h>

#include <trackbase/TrkrDefs.h>
#include <trackbase_historic/TrackAnalysisUtils.h>
#include <trackbase_historic/TrackSeed.h>
#include <trackbase_historic/TrackSeedContainer.h>

#include <phool/getClass.h>
#include <phool/phool.h>  // for PHWHERE

#include <KFParticle.h>

#include <TDatabasePDG.h>

#include <Eigen/Dense>

#include <cmath>     // for sqrt, pow
#include <cstdlib>   // for abs
#include <iostream>  // for operator<<, basic_ostream

void KFParticle_unsorted::getTracksFromBC(PHCompositeNode *topNode, const int &bunch_crossing, const std::string &vertexMapName, int &nTracks, int &nPVs) const
{
  if (m_config.useMbdVertex)  // If you're using the MBD vertex then there is no way to know which tracks are associated to it
  {
    return;
  }

  const std::string &vtxMN = vertexMapName.empty() ? m_config.vertexMapNodeName : vertexMapName;

  nTracks = 0;
  nPVs = 0;
  auto *dst_vertexmap = findNode::getClass<SvtxVertexMap>(topNode, vtxMN);
  for (SvtxVertexMap::ConstIter iter = dst_vertexmap->begin(); iter != dst_vertexmap->end(); ++iter)
  {
    SvtxVertex *dst_vertex = iter->second;
    if ((int) dst_vertex->get_beam_crossing() == bunch_crossing)
    {
      nTracks += dst_vertex->size_tracks();
      ++nPVs;
    }
  }
}

int KFParticle_unsorted::getTracksFromVertex(PHCompositeNode *topNode, const KFParticle &vertex, const std::string &vertexMapName) const
{
  if (m_config.useMbdVertex)  // If you're using the MBD vertex then there is no way to know which tracks are associated to it
  {
    return 0;
  }

  const std::string &vtxMN = vertexMapName.empty() ? m_config.vertexMapNodeName : vertexMapName;

  SvtxVertex *associated_vertex = nullptr;
  if (m_config.dontUseGlobalVertex)
  {
    auto *dst_vertexmap = findNode::getClass<SvtxVertexMap>(topNode, vtxMN);
    associated_vertex = dst_vertexmap ? dst_vertexmap->get(vertex.Id()) : nullptr;
  }
  else
  {
    auto *dst_globalvertexmap = findNode::getClass<GlobalVertexMap>(topNode, "GlobalVertexMap");
    auto *dst_vertexmap = findNode::getClass<SvtxVertexMap>(topNode, vtxMN);
    if (!dst_globalvertexmap || !dst_vertexmap)
    {
      std::cout << "Can't continue in KFParticle_Tools::getTracksFromVertex" << std::endl;
      return 0;
    }
    auto *associated_gvertex = dst_globalvertexmap->get(vertex.Id());
    if (!associated_gvertex)
    {
      return 0;
    }

    auto svtxiter = associated_gvertex->find_vertexes(GlobalVertex::SVTX);
    if (svtxiter == associated_gvertex->end_vertexes())
    {
      return 0;
    }
    const auto &svtxvertexvector = svtxiter->second;

    for (const auto &gvertex : svtxvertexvector)
    {
      auto vtxiter = dst_vertexmap->find(gvertex->get_id());
      if (vtxiter != dst_vertexmap->end())
      {
        associated_vertex = vtxiter->second;
      }
    }
  }
  if (associated_vertex)
  {
    return associated_vertex->size_tracks();
  }

  return 0;
}

int KFParticle_unsorted::getNchargedSiSeedMultiplicity(PHCompositeNode *topNode, const int &bunch_crossing, int verbosity)
{
  auto *m_siliconSeeds = findNode::getClass<TrackSeedContainer>(topNode, "SiliconTrackSeedContainer");
  //auto clustermap = findNode::getClass<TrkrClusterContainer>(topNode, "TRKR_CLUSTER_SEED");
  if (!m_siliconSeeds)
  {
    if (verbosity > 0)
    {
      std::cout << PHWHERE << " ERROR: Can't find SiliconTrackSeedContainer " << std::endl;
    }
    return -1;
  }
  //if (!clustermap)
  //{
  //  std::cout << " ERROR: Can't find TRKR_CLUSTER " << std::endl;
  //  return -1;
  //}

  TrackSeed *_tracklet_si;
  int ncharged_multiplicity = 0;
  const int nMapsCut = 1;
  const int nInttCut = 1;
  for (unsigned int phtrk_iter_si = 0;
         phtrk_iter_si < m_siliconSeeds->size();
         ++phtrk_iter_si)
  {
    _tracklet_si = m_siliconSeeds->get(phtrk_iter_si);
    if (!_tracklet_si)
    {
      continue;
    }
    if (_tracklet_si->get_crossing() == bunch_crossing)
    {
      if (TrackAnalysisUtils::get_cluster_count(_tracklet_si,TrkrDefs::mvtxId) >= nMapsCut &&
          TrackAnalysisUtils::get_cluster_count(_tracklet_si,TrkrDefs::inttId) >= nInttCut)
      {
        ncharged_multiplicity++;
      }
    }
  }

  return ncharged_multiplicity;
}

float KFParticle_unsorted::calculateJT(const KFParticle &mother, const KFParticle &daughter)
{
  Eigen::Vector3f motherP = Eigen::Vector3f(mother.GetPx(), mother.GetPy(), mother.GetPz());
  Eigen::Vector3f daughterP = Eigen::Vector3f(daughter.GetPx(), daughter.GetPy(), daughter.GetPz());

  Eigen::Vector3f motherP_X_daughterP = motherP.cross(daughterP);

  float jT = (motherP_X_daughterP.norm()) / motherP.norm();

  return jT;
}

double KFParticle_unsorted::calculateEllipsoidRadius(int posOrNeg, double sigma_ii, double sigma_jj, double sigma_ij)
{  // Note - Only works for a 2D ellipsoid OR rotated nD ellipsoid to avoid projections
  if (std::abs(posOrNeg) != 1)
  {
    std::cout << "You have set posOrNeg to " << posOrNeg << ". This value must be  +/- 1! Skipping" << std::endl;
    return 0;
  }

  double r_ij = sqrt((sigma_ii + sigma_jj) / 2 + posOrNeg * (sqrt(pow((sigma_ii - sigma_jj) / 2, 2) + pow(sigma_ij, 2))));

  return r_ij;
}

bool KFParticle_unsorted::findParticle(const std::string &particle)
{
  bool particleFound = true;
  if (!TDatabasePDG::Instance()->GetParticle(particle.c_str()))
  {
    std::cout << "The particle, " << particle << " is not recognized" << std::endl;
    std::cout << "Check TDatabasePDG for a list of available particles" << std::endl;
    particleFound = false;
  }

  return particleFound;
}

void KFParticle_unsorted::identify(const KFParticle &particle)
{
  std::cout << "Track ID: " << particle.Id() << std::endl;
  std::cout << "PDG ID: " << particle.GetPDG() << ", charge: " << (int) particle.GetQ() << ", mass: " << particle.GetMass() << " GeV" << std::endl;
  std::cout << "(px,py,pz) = (" << particle.GetPx() << " +/- " << std::sqrt(particle.GetCovariance(3, 3)) << ", ";
  std::cout << particle.GetPy() << " +/- " << std::sqrt(particle.GetCovariance(4, 4)) << ", ";
  std::cout << particle.GetPz() << " +/- " << std::sqrt(particle.GetCovariance(5, 5)) << ") GeV" << std::endl;
  std::cout << "(x,y,z) = (" << particle.GetX() << " +/- " << std::sqrt(particle.GetCovariance(0, 0)) << ", ";
  std::cout << particle.GetY() << " +/- " << std::sqrt(particle.GetCovariance(1, 1)) << ", ";
  std::cout << particle.GetZ() << " +/- " << std::sqrt(particle.GetCovariance(2, 2)) << ") cm\n" << std::endl;
}
