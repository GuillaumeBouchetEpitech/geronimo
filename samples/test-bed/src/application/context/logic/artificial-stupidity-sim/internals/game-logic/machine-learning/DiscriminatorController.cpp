
#include "./DiscriminatorController.hpp"

#include "geronimo/system/math/clamp.hpp"
#include "geronimo/system/metrics/Clock.hpp"
#include "geronimo/system/TraceLogger.hpp"

// #include <iomanip>

// MARK:asInputVector
void DiscriminatorInput::asInputVector(std::vector<float>& outVector, bool inFlipX) const
{
  outVector.clear();
  outVector.reserve(16);

  // in case we need to flip the X value on the Y axis
  const float coeffX = inFlipX ? -1.0f : 1.0f;

  // normalize
  outVector.push_back(this->selfState.pos.x / 20.0f * coeffX); // [-1..1]
  outVector.push_back(this->selfState.pos.y / 20.0f); // [0..1]
  outVector.push_back(this->selfState.vel.x / 30.0f * coeffX); // [-1..1]
  outVector.push_back(this->selfState.vel.y / 30.0f); // [-1..1]
  outVector.push_back(this->selfState.attackCooldown / 0.25f); // [0..1]
  outVector.push_back(this->selfState.dashCooldown / 1.0f); // [0..1]
  // outVector.push_back(this->selfState.invulnerableCooldown / 0.5f); // [0..1]
  outVector.push_back(float(this->selfState.health) / 3.0f); // [0..1]
  outVector.push_back(this->selfState.onTheGround ? 1.0f : 0.0f); // [0..1]

  // normalize
  outVector.push_back(this->selfActions.dir.x * coeffX); // [-1..1]
  outVector.push_back(this->selfActions.dir.y); // [-1..1]
  outVector.push_back(this->selfActions.jump ? 1.0f : 0.0f); // [0..1]
  outVector.push_back(this->selfActions.dash ? 1.0f : 0.0f); // [0..1]
  outVector.push_back(this->selfActions.attack ? 1.0f : 0.0f); // [0..1]


  // normalize
  outVector.push_back(this->opponentState.pos.x / 20.0f * coeffX); // [-1..1]
  outVector.push_back(this->opponentState.pos.y / 20.0f); // [0..1]
  outVector.push_back(this->opponentState.vel.x / 30.0f * coeffX); // [-1..1]
  outVector.push_back(this->opponentState.vel.y / 30.0f); // [-1..1]
  outVector.push_back(this->opponentState.attackCooldown / 0.25f); // [0..1]
  outVector.push_back(this->opponentState.dashCooldown / 1.0f); // [0..1]
  // outVector.push_back(this->opponentState.invulnerableCooldown / 0.5f); // [0..1]
  outVector.push_back(float(this->opponentState.health) / 3.0f); // [0..1]
  outVector.push_back(this->opponentState.onTheGround ? 1.0f : 0.0f); // [0..1]

  // normalize
  outVector.push_back(this->opponentActions.dir.x * coeffX); // [-1..1]
  outVector.push_back(this->opponentActions.dir.y); // [-1..1]
  outVector.push_back(this->opponentActions.jump ? 1.0f : 0.0f); // [0..1]
  outVector.push_back(this->opponentActions.dash ? 1.0f : 0.0f); // [0..1]
  outVector.push_back(this->opponentActions.attack ? 1.0f : 0.0f); // [0..1]

  // normalize
  // outVector.push_back(this->selfRelativeSpatialFeatures.deltaToOpponent.x / 40.0f * coeffX); // [-1..1]
  // outVector.push_back(this->selfRelativeSpatialFeatures.deltaToOpponent.y / 40.0f); // [-1..1]
  outVector.push_back(this->selfRelativeSpatialFeatures.distanceToOpponent / 40.0f); // [0..1]
}

bool DiscriminatorInput::hasInputs() const
{
  return (
    this->selfActions.dir.x != 0.0f ||
    this->selfActions.dir.y != 0.0f ||
    this->selfActions.jump ||
    this->selfActions.dash ||
    this->selfActions.attack
  );
}


// MARK:ctor
DiscriminatorController::DiscriminatorController()
{
  // input: 14 floats
  // hidden1: 32 floats
  // hidden2: 16 floats
  // output: 1 float
  // NeuralNetworkTopology networkTopology({ 14, 32, 16, 1 });
  // NeuralNetworkTopology networkTopology({ 13, 32, 16, 1 });
  NeuralNetworkTopology networkTopology = NeuralNetworkTopologyBuilder()
    // .addInputLayer(13)
    // .addInputLayer(13 + 13 + 3)
    .addInputLayer(13 + 13 + 1)
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

  // 60 frame per second -> for 30 seconds -> 1800 frames per character
  _characterSavedFrames_player.reserve(60 * 30);
  _characterSavedFrames_bot.reserve(60 * 30);
}




// MARK:rate
float DiscriminatorController::rate(const DiscriminatorInput& inRawInput)
{
  std::vector<float> inputs;
  std::vector<float> outputs;

  inputs.clear();
  outputs.clear();
  inputs.reserve(16);
  outputs.reserve(1);

  inRawInput.asInputVector(inputs, false);

  this->_neuralNetwork->forward(inputs, outputs);

  const float outputValue = outputs.at(0);

  // return gero::math::clamp(outputValue, 0.0f, 1.0f);
  return outputValue;
}




// MARK:clearCharacterFrames
void DiscriminatorController::clearCharacterFrames()
{
  _characterSavedFrames_player.clear();
  _characterSavedFrames_bot.clear();
}




// MARK:pushCharacterFrame
void DiscriminatorController::pushCharacterFrame(const DiscriminatorInput& inRed, const DiscriminatorInput& inBlue)
{
  _characterSavedFrames_player.push_back(inRed);
  _characterSavedFrames_bot.push_back(inBlue);
}




// MARK:learnFromSavedFrames
void DiscriminatorController::learnFromSavedFrames(bool inRedWonTheDuel)
{
  D_MYLOG("start")

  std::size_t totalSavedFrames = _characterSavedFrames_player.size();

  std::vector<TrainingSample> tmpDataset;

  // pre-allocated 4 times
  // -> 2x for both characters
  // -> 2x again for their "X flipped" values
  tmpDataset.reserve(totalSavedFrames * 4);

  TrainingSample tmpSample;
  tmpSample.inputs.resize(15);
  tmpSample.targets.resize(1);

  D_MYLOG("totalSavedFrames = " << totalSavedFrames)
  gero::metrics::Clock tmpClock;
  tmpClock.start();

  for (std::size_t ii = 0; ii < totalSavedFrames; ++ii)
  {
    // D_MYLOG("step = " << ii)

    const auto& currRedInput = _characterSavedFrames_player.at(ii);
    const auto& currBlueInput = _characterSavedFrames_bot.at(ii);

    const bool redHasInput = currRedInput.hasInputs();
    const bool blueHasInput = currBlueInput.hasInputs();

    if (inRedWonTheDuel) {
      if (redHasInput) {
        currRedInput.asInputVector(tmpSample.inputs, false);
        tmpSample.targets.at(0) = 1; // positive feedback
        tmpDataset.push_back(tmpSample);
      }

      // if (blueHasInput) {
      //   currBlueInput.asInputVector(tmpSample.inputs, false);
      //   tmpSample.targets.at(0) = 0; // negative feedback
      //   tmpDataset.push_back(tmpSample);
      // }

      if (redHasInput) {
        currRedInput.asInputVector(tmpSample.inputs, true);
        tmpSample.targets.at(0) = 1; // positive feedback
        tmpDataset.push_back(tmpSample);
      }

      // if (blueHasInput) {
      //   currBlueInput.asInputVector(tmpSample.inputs, true);
      //   tmpSample.targets.at(0) = 0; // negative feedback
      //   tmpDataset.push_back(tmpSample);
      // }
    }
    else {

      // if (blueHasInput) {
      //   currBlueInput.asInputVector(tmpSample.inputs, false);
      //   tmpSample.targets.at(0) = 1; // positive feedback
      //   tmpDataset.push_back(tmpSample);
      // }

      // if (redHasInput) {
      //   currRedInput.asInputVector(tmpSample.inputs, false);
      //   tmpSample.targets.at(0) = 0; // negative feedback
      //   tmpDataset.push_back(tmpSample);
      // }

      // if (blueHasInput) {
      //   currBlueInput.asInputVector(tmpSample.inputs, true);
      //   tmpSample.targets.at(0) = 1; // positive feedback
      //   tmpDataset.push_back(tmpSample);
      // }

      // if (redHasInput) {
      //   currRedInput.asInputVector(tmpSample.inputs, true);
      //   tmpSample.targets.at(0) = 0; // negative feedback
      //   tmpDataset.push_back(tmpSample);
      // }
    }
  }

  D_MYLOG("tmpDataset.size() = " << tmpDataset.size())

  // constexpr int32_t maxEpochs = 300;
  // constexpr int32_t maxEpochs = 1;
  constexpr int32_t maxEpochs = 3;
  int32_t epochs = 0;
  for (; epochs < maxEpochs; ++epochs) {

    D_MYLOG("epochs = " << epochs << " / " << maxEpochs);

    this->_trainer->trainEpoch(tmpDataset);

    D_MYLOG("averageError = " << this->_trainer->averageError());

    if (this->_trainer->isConverged(1e-3)) {
      break;
    }
  }

  tmpClock.stop();

  D_MYLOG("total epochs = " << epochs);
  D_MYLOG("duration (msec) = " << tmpClock.getDuration());
  D_MYLOG("duration (sec) = " << float(tmpClock.getDuration()) / 1000.0f);
  D_MYLOG("stop");

}
