
#pragma once

#include "extended-recursive-least-square/NeuralNetwork.hpp"
#include "extended-recursive-least-square/ExtenderRecursiveLeastSquareTrainer.hpp"

#include "geronimo/helpers/GLMath.hpp"

#include <memory>

// MARK:DiscriminatorInput
struct DiscriminatorInput {

  struct CharacterState {
    // - 2f -> pos x/y -> x2 [-1..1]
    glm::vec2 pos;
    // - 2f -> vel x/y -> x2 [-1..1]
    glm::vec2 vel;
    // - 3f -> cooldowns attack/dash/invulnerable -> x3 [0..1] -> implies "can dash" and "can attack"
    float attackCooldown;
    float dashCooldown;
    float invulnerableCooldown;
    // - 1f -> health -> x1 [0..1]
    uint32_t health;
    // - 1f -> on the ground -> x1 [0..1]
    bool onTheGround;
  };
  struct CharacterActions {
    // - 2f -> dir x/y -> x2 [-1..1]
    glm::vec2 dir;
    // - 3f -> jump/dash/attack [0..1]
    bool jump;
    bool dash;
    bool attack;
  };

  CharacterState selfState;
  CharacterActions selfActions;

  CharacterState opponentState;
  CharacterActions opponentActions;

  struct CharacterRelativeSpatialFeatures {
    // - 2f -> delta x/y -> x2 [-1..1]
    glm::vec2 deltaToOpponent;
    // - 1f -> distance -> [0..1]
    float distanceToOpponent;
    // // - 1f -> is airborne -> [0..1]
    // bool opponentIsAirborne;
    // // - 1f -> facing dir -> [-1..1]
    // float opponentFacingDirection;
  }
  selfRelativeSpatialFeatures;

  void asInputVector(std::vector<float>& outVector, bool flipX) const;

  bool hasInputs() const;

};

// MARK:DiscriminatorController
class DiscriminatorController
{
public:
  DiscriminatorController();

public:
  float rate(const DiscriminatorInput& inRawInput);

public:
  void clearCharacterFrames();
  void pushCharacterFrame(const DiscriminatorInput& inRed, const DiscriminatorInput& inBlue);

public:
  void learnFromSavedFrames(bool inRedWonTheDuel);

private:
  std::shared_ptr<NeuralNetwork> _neuralNetwork;
  std::shared_ptr<ExtenderRecursiveLeastSquareTrainer> _trainer;

  std::vector<DiscriminatorInput> _characterSavedFrames_player;
  std::vector<DiscriminatorInput> _characterSavedFrames_bot;

};
