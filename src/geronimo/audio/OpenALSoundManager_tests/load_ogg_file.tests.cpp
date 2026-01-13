
#include "headers.hpp"

#include "geronimo/system/math/constants.hpp"

TEST(audio_general_test, load_ogg_file) {
  gero::audio::OpenALSoundManager soundManager;

  // soundManager.loadOggFromFile(666, "./tests/src/audio/assets/Cannon_3.ogg");
  // soundManager.loadOggFromFile(666, "./src/audio/assets/Cannon_3.ogg");
  soundManager.loadOggFromFile(666, "./src/geronimo/audio/OpenALSoundManager_tests/assets/Cannon_3.ogg");

  const glm::vec3 centerPos = {100, 100, 0};

  soundManager.setListener(centerPos, glm::vec3(1, 0, 0), glm::vec3(0, 0, 1));

  soundManager.setVolume(0.125f);

  {
    D_MYLOG("normal pitch");
    soundManager.playSound(666, centerPos, 1.0f, 1.0f);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  {
    D_MYLOG("twice higher pitch");
    soundManager.playSound(666, centerPos, 1.0f, 2.0f);
    std::this_thread::sleep_for(std::chrono::milliseconds(750));
  }

  {
    D_MYLOG("twice lower pitch");
    soundManager.playSound(666, centerPos, 1.0f, 0.5f);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  }

  soundManager.setVolume(0.25f);

  {
    struct SoundDirection {
      std::string_view name;
      glm::vec3 dir;
    };

    const int k_quality = 32;
    const float k_qualityStep = 1.0f / float(k_quality);
    const float k_fstep = gero::math::pi2 * k_qualityStep;

    for (int ii = 0; ii <= k_quality; ++ii) {

      const float angle = float(ii) * k_fstep;
      const glm::vec3 currDir = glm::vec3(10 * std::cos(angle), 10 * std::sin(angle), 0);

      D_MYLOG("four times higher pitch, 3d sound, angle: " << angle << " (" << int32_t(angle / gero::math::pi * 180.0f) << " degrees)");
      soundManager.playSound(666, centerPos + currDir, 2.0f, 8.0f);
      std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }

    // std::array<SoundDirection, 10> allSoundDirections = {{
    //   {"left-back", glm::vec3(-7, +7, 0)},
    //   {"left", glm::vec3(0, +10, 0)},
    //   {"left-front", glm::vec3(+7, +7, 0)},
    //   {"front", glm::vec3(+10, 0, 0)},
    //   {"right-front", glm::vec3(+7, -7, 0)},
    //   {"right", glm::vec3(0, -10, 0)},
    //   {"right-back", glm::vec3(-7, -7, 0)},
    //   {"back", glm::vec3(-10, 0, 0)},
    //   {"top", glm::vec3(0, 0, +10)},
    //   {"down", glm::vec3(0, 0, -10)},
    // }};

    // for (const SoundDirection& data : allSoundDirections) {
    //   D_MYLOG("four times higher pitch, 3d sound " << data.name);
    //   soundManager.playSound(666, centerPos + data.dir, 2.0f, 8.0f);
    //   std::this_thread::sleep_for(std::chrono::milliseconds(150));
    // }
  }

  // ASSERT_EQ(result, true);
}
