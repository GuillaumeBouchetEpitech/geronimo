
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
    // glm::vec3 position;
    bool playing = false;
    bool absolute = false;
  };

  std::vector<SoundSource> _sources;
  uint32_t _currentSource = 0;

  std::unordered_map<uint32_t, uint32_t> _bufferSoundsMap;

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
  void playSound(uint32_t alias, const glm::vec3& pos, float volume, float pitch);
  void playAbsoluteSound(uint32_t alias, float volume, float pitch);

public:
  void setVolume(float level);
  void setListener(const glm::vec3& pos, const glm::vec3& front, const glm::vec3& up);
};

} // namespace audio
} // namespace gero
