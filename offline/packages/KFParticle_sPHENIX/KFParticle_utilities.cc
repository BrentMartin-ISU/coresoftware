#include "KFParticle_utilities.h"

#include <globalvertex/GlobalVertex.h>
#include <globalvertex/GlobalVertexMap.h>
#include <globalvertex/MbdVertex.h>
#include <globalvertex/MbdVertexMap.h>
#include <globalvertex/SvtxVertex.h>
#include <globalvertex/SvtxVertexMap.h>

#include <trackbase_historic/SvtxTrackMap.h>

#include <phool/getClass.h>

#include <KFParticle.h>

#include <iostream>

SvtxTrack *KFParticle_utilities::getTrack(unsigned int track_id, SvtxTrackMap *trackmap)
{
  // Track ids are the keys of the map, so look the track up directly instead of scanning the whole map
  auto iter = trackmap->find(track_id);
  if (iter == trackmap->end())
  {
    return nullptr;
  }

  return iter->second;
}

int KFParticle_utilities::getPVID(PHCompositeNode *topNode, const KFParticle &kfpvertex,
                                  bool dontUseGlobalVertex, bool useMbdVertex, const std::string &vtxMapName)
{
  if (dontUseGlobalVertex)
  {
    if (useMbdVertex)
    {
      auto *dst_mbdvertexmap = findNode::getClass<MbdVertexMap>(topNode, "MbdVertexMap");
      if (dst_mbdvertexmap)
      {
        MbdVertex *m_dst_vertex = dst_mbdvertexmap->get(kfpvertex.Id());
        return m_dst_vertex->get_beam_crossing();
      }

      std::cout << "KFParticle vertex matching: " << vtxMapName << " does not exist" << std::endl;
    }
    else
    {
      auto *dst_vertexmap = findNode::getClass<SvtxVertexMap>(topNode, vtxMapName);
      if (dst_vertexmap)
      {
        SvtxVertex *m_dst_vertex = dst_vertexmap->get(kfpvertex.Id());
        return m_dst_vertex->get_beam_crossing();
      }

      std::cout << "KFParticle vertex matching: " << vtxMapName << " does not exist" << std::endl;
    }
  }
  else
  {
    auto *globalvertexmap = findNode::getClass<GlobalVertexMap>(topNode, "GlobalVertexMap");
    if (!globalvertexmap)
    {
      return -100;
    }

    GlobalVertex *gvertex = globalvertexmap->get(kfpvertex.Id());
    return gvertex->get_beam_crossing();
  }

  return -100;
}
