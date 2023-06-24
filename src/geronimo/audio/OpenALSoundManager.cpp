
#include "OpenALSoundManager.hpp"

#include "decoders/ogg.hpp"

#include "geronimo/system/ErrorHandler.hpp"
#include "geronimo/system/file-utils/FileLoadFileCallback.hpp"
#include "geronimo/system/file-utils/getFileContent.hpp"
#include "geronimo/system/math/clamp.hpp"

#include <algorithm>
#include <iostream>

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

  _bufferSoundsMap[alias] = newBuffer;
}

void OpenALSoundManager::playSound(uint32_t alias, const glm::vec3& pos, float volume, float pitch) {

  if (!_enabled)
    return;

  auto it = _bufferSoundsMap.find(alias);
  if (it == _bufferSoundsMap.end())
    D_THROW(std::runtime_error, "Buffer sound not found, alias: " << alias);

  // D_MYLOG("playing \"" << filename << "\"");

  const uint32_t currBuffer = it->second;

  SoundSource& currSource = _sources[_currentSource];

  _currentSource = (_currentSource + 1) % _sources.size();

  OpenAlContext::SourceStates state = OpenAlContext::getSourceState(currSource.id);
  if (state != OpenAlContext::SourceStates::stopped) {
    OpenAlContext::stopSource(currSource.id);
  }

  // currSource.position = pos;
  currSource.playing = true;
  currSource.absolute = false;

  OpenAlContext::setSourcePitch(currSource.id, math::clamp(pitch, 0.0f, 100.0f));
  OpenAlContext::setSourcePosition(currSource.id, pos.x, pos.y, pos.z);
  OpenAlContext::setSourceVolume(currSource.id, math::clamp(volume, 0.0f, 1.0f));
  // OpenAlContext::setSourceMinVolume(currSource.id, float volume);
  // OpenAlContext::setSourceMaxVolume(currSource.id, float volume);
  // OpenAlContext::setSourceDirection(currSource.id, float x, float y, float z);
  // OpenAlContext::setSourceVelocity(currSource.id, float x, float y, float z);
  OpenAlContext::setSourceLooping(currSource.id, false);

  OpenAlContext::setSourceBuffer(currSource.id, currBuffer);
  OpenAlContext::playSource(currSource.id);
}

void OpenALSoundManager::playAbsoluteSound(uint32_t alias, float volume, float pitch) {
  if (!_enabled)
    return;

  auto it = _bufferSoundsMap.find(alias);
  if (it == _bufferSoundsMap.end())
    D_THROW(std::runtime_error, "Buffer sound not found, alias: " << alias);

  // D_MYLOG("playing \"" << filename << "\"");

  const uint32_t currBuffer = it->second;

  SoundSource& currSource = _sources[_currentSource];

  _currentSource = (_currentSource + 1) % _sources.size();

  OpenAlContext::SourceStates state = OpenAlContext::getSourceState(currSource.id);
  if (state != OpenAlContext::SourceStates::stopped) {
    OpenAlContext::stopSource(currSource.id);
  }

  // currSource.position = pos;
  currSource.playing = true;
  currSource.absolute = true;

  OpenAlContext::setSourcePitch(currSource.id, math::clamp(pitch, 0.0f, 100.0f));
  OpenAlContext::setSourcePosition(currSource.id, _listenerPos.x, _listenerPos.y, _listenerPos.z);
  OpenAlContext::setSourceVolume(currSource.id, math::clamp(volume, 0.0f, 1.0f));
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
