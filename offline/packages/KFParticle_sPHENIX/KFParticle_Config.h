#ifndef KFPARTICLESPHENIX_KFPARTICLECONFIG_H
#define KFPARTICLESPHENIX_KFPARTICLECONFIG_H

#include <string>

class KFParticle_sPHENIX;

// Configuration for KFParticle: set by KFParticle_sPHENIX (the module), read-only everywhere else
class KFParticle_Config
{
 public:
  bool writeTrackContainer() const { return m_write_track_container; }
  bool writeParticleContainer() const { return m_write_particle_container; }
  const std::string &containerName() const { return m_container_name; }
  const std::string &originTrackMapNodeName() const { return m_origin_track_map_node_name; }

 private:
  friend class KFParticle_sPHENIX;  // the only class that can write

  bool m_write_track_container = true;
  bool m_write_particle_container = true;
  std::string m_container_name;
  std::string m_origin_track_map_node_name = "SvtxTrackMap";
};

#endif  // KFPARTICLESPHENIX_KFPARTICLECONFIG_H
