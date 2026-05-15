
#pragma once

#include "extended-recursive-least-square/NeuralNetwork.hpp"
#include "extended-recursive-least-square/ExtenderRecursiveLeastSquareTrainer.hpp"

#include "geronimo/helpers/GLMath.hpp"

#include <memory>
// #include <array>


// MARK:ActorCriticInput
struct ActorCriticInput {

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

  CharacterState selfState;
  CharacterState opponentState;

  struct CharacterRelativeSpatialFeatures {
    // - 2f -> delta x/y -> x2 [-1..1]
    glm::vec2 deltaToOpponent;
    // - 1f -> distance -> [0..1]
    float distanceToOpponent;
    // - 1f -> is airborne -> [0..1]
    bool opponentIsAirborne;
    // - 1f -> facing dir -> [-1..1]
    float opponentFacingDirection;
  }
  selfRelativeSpatialFeatures;

  // struct CharacterRelativeTemporalFeatures {
  //   // - 1f -> avg player's velocity (last N frames)
  //   float avgOpponentVel;
  //   // - 1f -> avg player's attack frequency (last N frames)
  //   float avgOpponentAttackFrequency;
  //   // - 1f -> time left [0..1]
  //   float timeLeft;
  //   // - 1f -> death count [0..1]
  //   float deathCount;
  // }
  // selfRelativeTemporalFeatures;

  // std::array<CharacterState, 4> opponentHistoricalState;

public:
  void set(
    const ActorCriticInput::CharacterState& inSelfState,
    const ActorCriticInput::CharacterState& inOpponentState,
    float inOpponentFacingDirection
  );

  void update(
    const ActorCriticInput::CharacterState& inSelfState
  );

  void update(
    const ActorCriticInput::CharacterState& inSelfState,
    const ActorCriticInput::CharacterState& inOpponentState,
    float inOpponentFacingDirection
  );

  void asInputVector(std::vector<float>& outvector) const;

};

// MARK:ActorCriticController
class ActorCriticController
{
public:
  ActorCriticController();

public:
  float rate(const ActorCriticInput::CharacterState& inNextSelfState);
  void learn(float inDiscriminatorRating);

public:
  float averageError() const { return this->_trainer->averageError(); }

public:
  // TODO: is public
  ActorCriticInput _actorCriticInput;

private:
  std::shared_ptr<NeuralNetwork> _neuralNetwork;
  std::shared_ptr<ExtenderRecursiveLeastSquareTrainer> _trainer;

};
