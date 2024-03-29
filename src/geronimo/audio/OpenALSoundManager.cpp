
#include "OpenALSoundManager.hpp"

#include "decoders/ogg.hpp"

#include "geronimo/system/ErrorHandler.hpp"
#include "geronimo/system/file-utils/FileLoadFileCallback.hpp"
#include "geronimo/system/file-utils/getFileContent.hpp"
#include "geronimo/system/math/clamp.hpp"

#include <algorithm>

namespace gero {
namespace audio {

OpenALSoundManager::OpenALSoundManager() {

  _context = OpenAlContext::initialize();

  { // listener

    setListener(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1));
    OpenAlContext::setListenerVolume(1.0f);

  } // listener

  { // sources

    constexpr uint32_t totalSources = 30;
    std::array<uint32_t, totalSources> allSources;

    OpenAlContext::genSources(allSources.size(), allSources.data());

    _sources.reserve(totalSources); // pre-allocate
    for (std::size_t ii = 0; ii < allSources.size(); ++ii) {

      const uint32_t currSource = allSources.at(ii);

      OpenAlContext::setSourcePitch(currSource, 1.0f);
      // OpenAlContext::setSourcePosition(currSource, 0.0f, 0.0f, 0.0f);
      OpenAlContext::setSourceVolume(currSource, 1.0f);
      OpenAlContext::setSourceMinVolume(currSource, 0.0f);
      OpenAlContext::setSourceMaxVolume(currSource, 1.0f);
      OpenAlContext::setSourceDirection(currSource, 0.0f, 0.0f, 0.0f);
      OpenAlContext::setSourceVelocity(currSource, 0.0f, 0.0f, 0.0f);
      OpenAlContext::setSourceLooping(currSource, false);

      _sources.push_back({currSource});
    }

  } // sources
}

OpenALSoundManager::~OpenALSoundManager() { OpenAlContext::close(_context); }

void OpenALSoundManager::setEnabled(bool enabled) {
  if (_enabled == enabled)
    return;

  _enabled = enabled;

  OpenAlContext::setListenerVolume(0.0f);
}

bool OpenALSoundManager::isEnabled() const { return _enabled; }

void OpenALSoundManager::loadOggFromFile(uint32_t alias, const std::string& filename) {
  loadOggFromFile(alias, filename, gero::fileUtils::getDefaulCallback());
}

void OpenALSoundManager::loadOggFromFile(uint32_t alias,
                                         const std::string& filename,
                                         gero::fileUtils::FileManager& fileManager) {
  loadOggFromFile(alias, filename, gero::fileUtils::getFileManagerCallback(fileManager));
}

void OpenALSoundManager::loadOggFromFile(uint32_t alias,
                                         const std::string& filename,
                                         gero::fileUtils::LoadCallback loadFileCallback) {
  std::string content;
  loadFileCallback(filename, content);

  loadOggFromMemory(alias, content);
}

void OpenALSoundManager::loadOggFromMemory(uint32_t alias, const std::string& content) {
  if (_bufferSoundsMap.count(alias) != 0)
    return;

  uint32_t newBuffer;
  OpenAlContext::genBuffers(1, &newBuffer);

  std::int32_t channels;
  std::int32_t sampleRate;
  std::int32_t bitsPerSample;
  uint32_t size;
  char* soundData = decoders::ogg::decode(content, channels, sampleRate, bitsPerSample, size);

  OpenAlContext::BufferFormat format;
  if (channels == 1 && bitsPerSample == 8)
    format = OpenAlContext::BufferFormat::mono8;
  else if (channels == 1 && bitsPerSample == 16)
    format = OpenAlContext::BufferFormat::mono16;
  else if (channels == 2 && bitsPerSample == 8)
    format = OpenAlContext::BufferFormat::stereo8;
  else if (channels == 2 && bitsPerSample == 16)
    format = OpenAlContext::BufferFormat::stereo16;
  else
    D_THROW(std::runtime_error,
            "ERROR: unrecognised ogg format"
              << ", channels=" << channels << ", bitsPerSample=" << bitsPerSample);

  // D_MYLOG("channels " << channels);
  // D_MYLOG("bitsPerSample " << bitsPerSample);
  // D_MYLOG("size " << size);

  OpenAlContext::setBufferData(newBuffer, format, soundData, size, sampleRate);

  decoders::ogg::deallocate(&soundData);

  _bufferSoundsMap[alias] = {newBuffer};
}

void OpenALSoundManager::playSound(uint32_t inAlias,
                                   const glm::vec3& inPosition,
                                   float inVolume,
                                   float inPitch,
                                   uint32_t inPriority /*= 0*/,
                                   int32_t inUserAlias /*= 0*/,
                                   int32_t inMaxAlias /*= 1000*/) {
  _playSound(inAlias, inPosition, inVolume, inPitch, false, inPriority, inUserAlias, inMaxAlias);
}

void OpenALSoundManager::playAbsoluteSound(uint32_t inAlias,
                                           float inVolume,
                                           float inPitch,
                                           uint32_t inPriority /*= 0*/,
                                           int32_t inUserAlias /*= 0*/,
                                           int32_t inMaxAlias /*= 1000*/) {
  _playSound(inAlias, glm::vec3(0, 0, 0), inVolume, inPitch, true, inPriority, inUserAlias, inMaxAlias);
}

void OpenALSoundManager::_playSound(uint32_t inAlias,
                                    const glm::vec3& inPosition,
                                    float inVolume,
                                    float inPitch,
                                    bool inAbsolute,
                                    uint32_t inPriority,
                                    int32_t inUserAlias,
                                    int32_t inMaxAlias) {

  if (!_enabled)
    return;

  auto it = _bufferSoundsMap.find(inAlias);
  if (it == _bufferSoundsMap.end())
    D_THROW(std::runtime_error, "Buffer sound not found, alias: " << inAlias);

  // D_MYLOG("playing \"" << filename << "\"");

  const uint32_t currBuffer = it->second.id;

  // synchronize data
  for (SoundSource& tmpSource : _sources) {
    if (tmpSource.playing == false)
      continue;

    if (OpenAlContext::getSourceState(tmpSource.id) != OpenAlContext::SourceStates::playing)
      tmpSource.playing = false;
  }

  int32_t totalUserAlias = 0;

  for (SoundSource& tmpSource : _sources)
    if (tmpSource.playing == true && tmpSource.userAlias == inUserAlias)
      ++totalUserAlias;

  const bool maxUsageReached = (totalUserAlias >= inMaxAlias);

  SoundSource* bestSource = nullptr;

  if (maxUsageReached == false) {

    float bestDistance = 999999999.0f;

    for (SoundSource& tmpSource : _sources) {
      if (!tmpSource.playing) {
        bestSource = &tmpSource;
        break;
      }

      if (tmpSource.absolute || inPriority < tmpSource.priority)
        continue;

      if (inAbsolute == false) {
        const float distance = glm::distance(tmpSource.position, _listenerPos);
        if (distance > bestDistance)
          continue;

        bestDistance = distance;
      }

      bestSource = &tmpSource;
      break;
    }

  } else {

    float bestDistance = 0.0f;

    // replace already playing sounds
    for (SoundSource& tmpSource : _sources) {
      if (tmpSource.playing == false || tmpSource.userAlias != inUserAlias)
        continue;

      if (tmpSource.absolute || inPriority < tmpSource.priority)
        continue;

      if (inAbsolute == false) {
        const float distance = glm::distance(tmpSource.position, _listenerPos);
        if (distance < bestDistance)
          continue;

        bestDistance = distance;
      }

      bestSource = &tmpSource;
      break;
    }
  }

  if (!bestSource)
    return;

  SoundSource& currSource = *bestSource;

  // _currentSource = (_currentSource + 1) % _sources.size();

  OpenAlContext::SourceStates state = OpenAlContext::getSourceState(currSource.id);
  if (state != OpenAlContext::SourceStates::stopped) {
    OpenAlContext::stopSource(currSource.id);
  }

  currSource.position = inPosition;
  currSource.playing = true;
  currSource.absolute = inAbsolute;
  currSource.priority = inPriority;

  OpenAlContext::setSourcePitch(currSource.id, math::clamp(inPitch, 0.0f, 100.0f));
  OpenAlContext::setSourcePosition(currSource.id, inPosition.x, inPosition.y, inPosition.z);
  OpenAlContext::setSourceVolume(currSource.id, math::clamp(inVolume, 0.0f, 1.0f));
  // OpenAlContext::setSourceMinVolume(currSource.id, float volume);
  // OpenAlContext::setSourceMaxVolume(currSource.id, float volume);
  // OpenAlContext::setSourceDirection(currSource.id, float x, float y, float z);
  // OpenAlContext::setSourceVelocity(currSource.id, float x, float y, float z);
  OpenAlContext::setSourceLooping(currSource.id, false);

  OpenAlContext::setSourceBuffer(currSource.id, currBuffer);
  OpenAlContext::playSource(currSource.id);
}

//
//
//

void OpenALSoundManager::setVolume(float level) { OpenAlContext::setListenerVolume(math::clamp(level, 0.0f, 1.0f)); }

void OpenALSoundManager::setListener(const glm::vec3& pos, const glm::vec3& front, const glm::vec3& up) {

  _listenerPos = pos;

  OpenAlContext::setListenerPosition(pos.x, pos.y, pos.z);
  OpenAlContext::setListenerOrientation(front, up);

  for (SoundSource& currSource : _sources) {
    if (currSource.absolute && currSource.playing) {
      OpenAlContext::SourceStates state = OpenAlContext::getSourceState(currSource.id);
      if (state != OpenAlContext::SourceStates::playing) {
        currSource.playing = false;
      } else {
        OpenAlContext::setSourcePosition(currSource.id, pos.x, pos.y, pos.z);
      }
    }
  }
}

} // namespace audio
} // namespace gero
