
#include "ActorCriticController.hpp"

#include "geronimo/system/math/clamp.hpp"

// MARK:set
void ActorCriticInput::set(
  const ActorCriticInput::CharacterState& inSelfState,
  const ActorCriticInput::CharacterState& inOpponentState,
  float inOpponentFacingDirection
) {
  this->update(inSelfState, inOpponentState, inOpponentFacingDirection);
}

// MARK:update
void ActorCriticInput::update(
  const ActorCriticInput::CharacterState& inSelfState
) {

  this->selfState = inSelfState;

  this->selfRelativeSpatialFeatures.deltaToOpponent = this->selfState.pos - this->opponentState.pos;
  this->selfRelativeSpatialFeatures.distanceToOpponent = glm::distance(this->selfState.pos, this->opponentState.pos);
}

// MARK:update2
void ActorCriticInput::update(
  const ActorCriticInput::CharacterState& inSelfState,
  const ActorCriticInput::CharacterState& inOpponentState,
  float inOpponentFacingDirection
) {


  this->selfState = inSelfState;
  this->opponentState = inOpponentState;

  this->selfRelativeSpatialFeatures.deltaToOpponent = this->selfState.pos - this->opponentState.pos;
  this->selfRelativeSpatialFeatures.distanceToOpponent = glm::distance(this->selfState.pos, this->opponentState.pos);
  this->selfRelativeSpatialFeatures.opponentFacingDirection = inOpponentFacingDirection;
  this->selfRelativeSpatialFeatures.opponentIsAirborne = this->opponentState.pos.y > 0.0f;

  // this->selfRelativeTemporalFeatures.

  // this.opponentHistoricalState

}

// MARK:asInputVector
void ActorCriticInput::asInputVector(std::vector<float>& outVector) const
{
  outVector.clear();
  outVector.reserve(64);

  // normalize
  outVector.push_back(this->selfState.pos.x / 20.0f); // [-1..1]
  outVector.push_back(this->selfState.pos.y / 20.0f); // [0..1]
  outVector.push_back(this->selfState.vel.x / 30.0f); // [-1..1]
  outVector.push_back(this->selfState.vel.y / 30.0f); // [-1..1]
  outVector.push_back(this->selfState.attackCooldown / 0.25f); // [0..1]
  outVector.push_back(this->selfState.dashCooldown / 1.0f); // [0..1]
  // outVector.push_back(this->selfState.invulnerableCooldown / 0.5f); // [0..1]
  outVector.push_back(float(this->selfState.health) / 3.0f); // [0..1]
  outVector.push_back(this->selfState.onTheGround ? 1.0f : 0.0f); // [0..1]

  // normalize
  outVector.push_back(this->opponentState.pos.x / 20.0f); // [-1..1]
  outVector.push_back(this->opponentState.pos.y / 20.0f); // [0..1]
  outVector.push_back(this->opponentState.vel.x / 30.0f); // [-1..1]
  outVector.push_back(this->opponentState.vel.y / 30.0f); // [-1..1]
  outVector.push_back(this->opponentState.attackCooldown / 0.25f); // [0..1]
  outVector.push_back(this->opponentState.dashCooldown / 1.0f); // [0..1]
  // outVector.push_back(this->opponentState.invulnerableCooldown / 0.5f); // [0..1]
  outVector.push_back(float(this->opponentState.health) / 3.0f); // [0..1]
  outVector.push_back(this->opponentState.onTheGround ? 1.0f : 0.0f); // [0..1]

  // normalize
  outVector.push_back(this->selfRelativeSpatialFeatures.deltaToOpponent.x / 40.0f); // [-1..1]
  outVector.push_back(this->selfRelativeSpatialFeatures.deltaToOpponent.y / 40.0f); // [-1..1]
  outVector.push_back(this->selfRelativeSpatialFeatures.distanceToOpponent / 40.0f); // [0..1]
  outVector.push_back(this->selfRelativeSpatialFeatures.opponentIsAirborne ? 1.0f : 0.0f); // [0/1]
  outVector.push_back(this->selfRelativeSpatialFeatures.opponentFacingDirection < 0.0f ? -1.0f : 1.0f); // [-1/1]

  // // normalize
  // outVector.push_back(this->selfRelativeTemporalFeatures.avgOpponentVel / 60.0f); // [0..1]
  // outVector.push_back(this->selfRelativeTemporalFeatures.avgOpponentAttackFrequency); // [0..1]
  // outVector.push_back(this->selfRelativeTemporalFeatures.timeLeft); // [0..1]
  // outVector.push_back(this->selfRelativeTemporalFeatures.deathCount); // [-1..1]

  // for (std::size_t ii = 0; ii < this->opponentHistoricalState.size(); ++ii)
  // {
  //   const auto& currState = this->opponentHistoricalState.at(ii);

  //   // normalize
  //   outVector.push_back(currState.pos.x / 20.0f); // [-1..1]
  //   outVector.push_back(currState.pos.y / 20.0f); // [0..1]
  //   outVector.push_back(currState.vel.x / 30.0f); // [-1..1]
  //   outVector.push_back(currState.vel.y / 30.0f); // [-1..1]
  //   outVector.push_back(currState.attackCooldown / 0.25f); // [0..1]
  //   outVector.push_back(currState.dashCooldown / 1.0f); // [0..1]
  //   outVector.push_back(currState.invulnerableCooldown / 0.5f); // [0..1]
  //   outVector.push_back(currState.health / 3.0f); // [0..1]
  //   outVector.push_back(currState.onTheGround ? 1.0f : 0.0f); // [0..1]
  // }

}

// MARK:ctor
ActorCriticController::ActorCriticController()
{
  // input: 14 floats
  // hidden1: 32 floats
  // hidden2: 16 floats
  // output: 1 float
  // NeuralNetworkTopology networkTopology({ 23, 32, 16, 1 });
  // NeuralNetworkTopology networkTopology({ 21, 32, 16, 1 });
  NeuralNetworkTopology networkTopology = NeuralNetworkTopologyBuilder()
    .addInputLayer(21)
    .addHiddenLayer(32, NeuralNetworkActivations::leakyReLU)
    .addHiddenLayer(16, NeuralNetworkActivations::leakyReLU)
    .addOutputLayer(1, NeuralNetworkActivations::leakyReLU)
    .build();

  this->_neuralNetwork = std::make_shared<NeuralNetwork>(networkTopology);

  ExtenderRecursiveLeastSquareTrainerDef def;
  def.network = this->_neuralNetwork;
  // forgetting factor
  def.lambda = 0.99f;
  // minimum required epochs for the training convergence to be accurate
  // def.windowSize = 100;
  def.windowSize = 1;

  this->_trainer = std::make_shared<ExtenderRecursiveLeastSquareTrainer>(def);
}

// MARK:rate
float ActorCriticController::rate(const ActorCriticInput::CharacterState& inNextSelfState)
{
  ActorCriticInput tmpCopyActorCriticInput = this->_actorCriticInput;

  // update that input copy with the new state
  // -> which likely came from a look-ahead
  tmpCopyActorCriticInput.update(inNextSelfState);

  std::vector<float> inputs;
  std::vector<float> outputs;

  inputs.clear();
  outputs.clear();
  inputs.reserve(23);
  outputs.reserve(1);

  tmpCopyActorCriticInput.asInputVector(inputs);

  this->_neuralNetwork->forward(inputs, outputs);

  const float outputValue = outputs.at(0);

  return gero::math::clamp(outputValue, 0.0f, 1.0f);
}

// MARK:learn
void ActorCriticController::learn(float inDiscriminatorRating)
{
  TrainingSample tmpSample;
  // tmpSample.inputs.resize(23);
  tmpSample.targets.resize(1);

  // set the sample's input values
  this->_actorCriticInput.asInputVector(tmpSample.inputs);
  // set the sample's target value
  tmpSample.targets.at(0) = inDiscriminatorRating; // discriminator feedback

  std::vector<TrainingSample> tmpDataset;
  tmpDataset.push_back(tmpSample);

  // constexpr int32_t maxEpochs = 300;
  constexpr int32_t maxEpochs = 3;
  int32_t epochs = 0;
  for (; epochs < maxEpochs; ++epochs) {

    // D_MYLOG("epochs = " << epochs << " / " << maxEpochs);

    this->_trainer->trainEpoch(tmpDataset);

    // D_MYLOG("averageError = " << std::setprecision(4) << this->_trainer->averageError());

    if (this->_trainer->isConverged(1e-3)) {
      break;
    }
  }

}


