
#pragma once

#include "OpenAlContext.hpp"

#include "geronimo/system/file-utils/FileLoadFileCallback.hpp"
#include "geronimo/system/file-utils/FileManager.hpp"

#include "geronimo/helpers/GLMath.hpp"

#include <string>
#include <unordered_map>
#include <vector>

namespace gero {
namespace audio {

class OpenALSoundManager {
private:
  bool _enabled = true;

  glm::vec3 _listenerPos = {0, 0, 0};

  OpenAlContext::Context _context;

  struct SoundSource {
    uint32_t id;
    glm::vec3 position = {0,0,0};
    bool playing = false;
    bool absolute = false;
    uint32_t priority = 0;
    int32_t userAlias = 0;
  };

  std::vector<SoundSource> _sources;

  struct SoundBuffer {
    uint32_t id;
  };

  std::unordered_map<uint32_t, SoundBuffer> _bufferSoundsMap;

public:
  OpenALSoundManager();
  ~OpenALSoundManager();

public:
  void setEnabled(bool enabled);
  bool isEnabled() const;

public:
  void loadOggFromFile(uint32_t alias, const std::string& filename);
  void loadOggFromFile(uint32_t alias, const std::string& filename, fileUtils::FileManager& fileManager);
  void loadOggFromFile(uint32_t alias, const std::string& filename, fileUtils::LoadCallback loadFileCallback);
  void loadOggFromMemory(uint32_t alias, const std::string& content);

public:
  void playSound(uint32_t inAlias, const glm::vec3& inPosition, float inVolume, float inPitch, uint32_t inPriority = 0, int32_t inUserAlias = 0, int32_t inMaxAlias = 1000);
  void playAbsoluteSound(uint32_t inAlias, float inVolume, float inPitch, uint32_t inPriority = 0, int32_t inUserAlias = 0, int32_t inMaxAlias = 1000);

private:
  void _playSound(uint32_t inAlias, const glm::vec3& inPosition, float inVolume, float inPitch, bool inAbsolute, uint32_t inPriority, int32_t inUserAlias, int32_t inMaxAlias);

public:
  void setVolume(float level);
  void setListener(const glm::vec3& pos, const glm::vec3& front, const glm::vec3& up);
};

} // namespace audio
} // namespace gero
