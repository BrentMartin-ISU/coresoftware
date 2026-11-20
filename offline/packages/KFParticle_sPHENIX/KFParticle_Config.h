#ifndef KFPARTICLESPHENIX_KFPARTICLECONFIG_H
#define KFPARTICLESPHENIX_KFPARTICLECONFIG_H

#include <string>

// Configuration struct for KFParticle holds bools and strings
struct KFParticle_Config
{
  bool m_write_track_container = true;
  bool m_write_particle_container = true;
  std::string m_container_name;
  std::string m_origin_track_map_node_name = "SvtxTrackMap";
};

#endif  // KFPARTICLESPHENIX_KFPARTICLECONFIG_H
