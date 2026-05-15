
#pragma once

#include "KeyframesManager.hpp"
#include "PlatformerCharacter.hpp"

#include "machine-learning/ActorCriticController.hpp"
#include "machine-learning/DiscriminatorController.hpp"

#include "geronimo/helpers/GLMath.hpp"

class PlatformerDuel
{
public:
  PlatformerDuel(KeyframesManager& inKeyframesManager);

public:
  void update(float deltaTimeSec);
  void renderScene();
  void renderHUD();

private:
  void _reset();
  void _handleMachineLearningLogic();
  void _handleMachineLearningLogic_part2();

private:
  KeyframesManager& _keyframesManager;
  PlatformerCharacter platformerCharacterBot;
  PlatformerCharacter platformerCharacterPlayer;

  float _timeLeftUntilNextUpdate = 0.0f;
  uint32_t _totalFramesLeft = 0;
  float _transitionTimeLeft = 0.0f;

  ActorCriticController _actorCriticController;
  DiscriminatorController _discriminatorController;

  uint32_t _totalDuels = 0;

  float _stateRating = 0.0f;

};
