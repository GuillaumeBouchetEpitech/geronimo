
#include "BrickModel.hpp"

#include "application/context/Context.hpp"

#include "geronimo/system/TraceLogger.hpp"

BrickModel::BrickModel(const std::string& inName)
  : _name(inName)
{}

//MARK:findFloorQuads
bool BrickModel::findFloorQuads(const glm::vec3& inCenter, const glm::vec3& inSize, std::vector<FloorQuad>& outQuads) const {

  outQuads.clear();
  outQuads.reserve(16);

  for (std::size_t ii = 0; ii < this->_floorsManager.getFloorQuads().size(); ++ii) {
    if (this->_floorsManager.getFloorQuads().at(ii).isColliding(inCenter, inSize)) {
      // hard copy -> on purpose
      outQuads.push_back(this->_floorsManager.getFloorQuads().at(ii));
    }
  }

  for (const BrickInstance& currInstance : this->_brickInstancesManager.getBrickInstances()) {
    if (!currInstance.ref) {
      continue;
    }

    glm::mat4 transform = glm::identity<glm::mat4>();
    transform = glm::translate(transform, currInstance.pos);
    transform = transform * glm::mat4_cast(currInstance.quat);


    const auto& allFloorQuads = currInstance.ref->getFloorsManager().getFloorQuads();

    for (std::size_t ii = 0; ii < allFloorQuads.size(); ++ii) {

      const auto& currQuad = allFloorQuads.at(ii);

      // hard copy -> on purpose
      FloorQuad newQuad = FloorQuad::makeFloorFromMat4(currQuad, transform);

      // D_MYLOG("- currQuad: " << currQuad.getOrigin());
      // D_MYLOG("  newQuad: " << newQuad.getOrigin());

      if (newQuad.isColliding(inCenter, inSize)) {
        // hard copy -> on purpose
        outQuads.push_back(newQuad);
      }
    }
  }

  return !outQuads.empty();
}

//MARK:findFloorQuads
bool BrickModel::findFloorQuads(const glm::vec3& inCenter, float inRadius, std::vector<FloorQuad>& outQuads) const {
  return this->findFloorQuads(inCenter, glm::vec3(inRadius, inRadius, inRadius), outQuads);
}

//MARK:buildVertices
void BrickModel::buildVertices(IWireFramesStackRenderer& inWireFrames) const
{
  // axis
  inWireFrames.pushLine(glm::vec3(0, 0, 0), glm::vec3(10, 0, 0), glm::vec3(1, 0, 0));
  inWireFrames.pushLine(glm::vec3(0, 0, 0), glm::vec3(0, 10, 0), glm::vec3(0, 1, 0));
  inWireFrames.pushLine(glm::vec3(0, 0, 0), glm::vec3(0, 0, 10), glm::vec3(0, 0, 1));

  for (const auto& currQuad : this->_floorsManager.getFloorQuads()) {
    currQuad.buildVertices(inWireFrames);
  }

  for (const auto& currQuad : this->_wallsManager.getWallQuads()) {
    currQuad.buildVertices(inWireFrames);
  }

}

//MARK:buildInstances
void BrickModel::buildInstances(const glm::vec3& inOrigin, const glm::quat& inQuat, InstancedBrickModels& inInstancedBrickModels) const
{
  // D_MYERR("getBrickInstances().size(): " << _brickInstancesManager.getBrickInstances().size());
  for (const auto& currInstance : _brickInstancesManager.getBrickInstances())
  {
    if (!currInstance.ref) {
      D_MYERR("invalid ref");
      continue;
    }

    InstancedBrickModels::GeometryInstance instance;

    instance.position = inOrigin + inQuat * currInstance.pos;
    instance.orientation = inQuat * currInstance.quat;
    instance.color = glm::vec4(1,1,1, 1);
    instance.light = false;
    instance.scale = glm::vec3(1,1,1);

    inInstancedBrickModels.pushAlias(currInstance.ref->getName(), instance);

    // D_MYERR(" => pushAlias -> " << currInstance.ref->getName());
  }
}

//MARK:render
void BrickModel::render(const glm::mat4& transform) const
{

  auto& context = Context::get();
  auto& renderer = context.graphic.renderer;
  // gero::graphics::camera::ICamera& camInstance = renderer.getSceneRenderer().getCamera();

  auto& scene = renderer.getSceneRenderer();

  auto& stackRenderers = scene.getStackRenderers();
  // auto& wireFrames = stackRenderers.getWireFramesStack();

  {
    auto& wireFrames = stackRenderers.getWireFramesStack();

    wireFrames.pushLine(glm::vec3(0, 0, 0), glm::vec3(10, 0, 0), glm::vec3(1, 0, 0));
    wireFrames.pushLine(glm::vec3(0, 0, 0), glm::vec3(0, 10, 0), glm::vec3(0, 1, 0));
    wireFrames.pushLine(glm::vec3(0, 0, 0), glm::vec3(0, 0, 10), glm::vec3(0, 0, 1));

    stackRenderers.flush();
  }

  for (const auto& currQuad : this->_floorsManager.getFloorQuads()) {
    currQuad.render();
  }

  for (const auto& currQuad : this->_wallsManager.getWallQuads()) {
    currQuad.render();
  }

  stackRenderers.flush();

}

