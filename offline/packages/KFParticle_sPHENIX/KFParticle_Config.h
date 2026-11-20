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
};

#endif  // KFPARTICLESPHENIX_KFPARTICLECONFIG_H
