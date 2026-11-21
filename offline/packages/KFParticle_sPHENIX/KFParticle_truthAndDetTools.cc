#include "KFParticle_truthAndDetTools.h"
#include "KFParticle_Tools.h"  // for KFParticle_Tools

#include <KFParticle.h>  // for KFParticle

#include <vector>

void KFParticle_truthAndDetTools::allPVInfo(PHCompositeNode *topNode,
                                            TTree * /*m_tree*/,
                                            const KFParticle &motherParticle,
                                            std::vector<KFParticle> daughters,
                                            std::vector<KFParticle> intermediates)
{
  std::vector<KFParticle> primaryVertices = m_tools->makeAllPrimaryVertices(topNode, m_tools->get_vtx_map_node_name());

  for (auto &primaryVertice : primaryVertices)
  {
    allPV_x.push_back(primaryVertice.GetX());
    allPV_y.push_back(primaryVertice.GetY());
    allPV_z.push_back(primaryVertice.GetZ());

    allPV_mother_PV_DCA.push_back(motherParticle.GetDistanceFromVertex(primaryVertice));
    allPV_mother_PV_DCA_StdDev.push_back(motherParticle.GetDeviationFromVertex(primaryVertice));

    for (unsigned int j = 0; j < daughters.size(); ++j)
    {
      allPV_daughter_PV_DCA[j].push_back(daughters[j].GetDistanceFromVertex(primaryVertice));
      allPV_daughter_PV_DCA_StdDev[j].push_back(daughters[j].GetDeviationFromVertex(primaryVertice));
    }

    for (unsigned int j = 0; j < intermediates.size(); ++j)
    {
      allPV_intermediates_PV_DCA[j].push_back(intermediates[j].GetDistanceFromVertex(primaryVertice));
      allPV_intermediates_PV_DCA_StdDev[j].push_back(intermediates[j].GetDeviationFromVertex(primaryVertice));
    }
  }
}

void KFParticle_truthAndDetTools::clearVectors()
{
  for (int i = 0; i < m_num_tracks_nTuple; ++i)
  {
    // PV vectors
    allPV_daughter_PV_DCA[i].clear();
    allPV_daughter_PV_DCA_StdDev[i].clear();
  }

  allPV_x.clear();
  allPV_y.clear();
  allPV_z.clear();

  allPV_mother_PV_DCA.clear();
  allPV_mother_PV_DCA_StdDev.clear();

  for (int i = 0; i < m_num_intermediate_states_nTuple; ++i)
  {
    allPV_intermediates_PV_DCA[i].clear();
    allPV_intermediates_PV_DCA_StdDev[i].clear();
  }
}
