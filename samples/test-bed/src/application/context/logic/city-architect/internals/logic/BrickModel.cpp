
#include "BrickModel.hpp"

#include "application/context/Context.hpp"

#include "geronimo/system/TraceLogger.hpp"

BrickModel::BrickModel(const std::string& inName)
  : _name(inName)
{}

//MARK:findFloorQuads
bool BrickModel::findFloorQuads(
  const glm::vec3& inCenter,
  const glm::vec3& inSize,
  std::vector<FloorQuad>& outQuads
) const {

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

    this->_findFloorQuads(
      glm::identity<glm::mat4>(),
      currInstance,
      inCenter,
      inSize,
      outQuads
    );
  }

  return !outQuads.empty();
}

//MARK:findFloorQuads
bool BrickModel::findFloorQuads(const glm::vec3& inCenter, float inRadius, std::vector<FloorQuad>& outQuads) const {
  return this->findFloorQuads(inCenter, glm::vec3(inRadius, inRadius, inRadius), outQuads);
}

//MARK:_findFloorQuads
void BrickModel::_findFloorQuads(
  const glm::mat4& inTransform,
  const BrickInstance& inBrickInstance,
  const glm::vec3& inCenter,
  const glm::vec3& inSize,
  std::vector<FloorQuad>& outQuads
) const {

  glm::mat4 transform = inTransform;
  transform = glm::translate(transform, inBrickInstance.pos);
  transform = transform * glm::mat4_cast(inBrickInstance.quat);

  const auto& allFloorQuads = inBrickInstance.ref->getFloorsManager().getFloorQuads();

  for (std::size_t ii = 0; ii < allFloorQuads.size(); ++ii) {

    const auto& currQuad = allFloorQuads.at(ii);

    // hard copy -> on purpose
    FloorQuad newQuad = FloorQuad::makeFloorFromMat4(currQuad, transform);

    if (newQuad.isColliding(inCenter, inSize)) {
      // hard copy -> on purpose
      outQuads.push_back(newQuad);
    }
  }

  for (const BrickInstance& currInstance : inBrickInstance.ref->getBrickInstancesManager().getBrickInstances()) {
    if (!currInstance.ref) {
      continue;
    }

    this->_findFloorQuads(
      transform,
      currInstance,
      inCenter,
      inSize,
      outQuads
    );
  }

}

//MARK:computeAABB
void BrickModel::computeAABB(const glm::mat4& inTransform) const
{
  this->_aabb.reset();

  for (const auto& currQuad : this->_floorsManager.getFloorQuads()) {
    auto newQuad = FloorQuad::makeFloorFromMat4(currQuad, inTransform);
    this->_aabb.expand(newQuad.getOrigin());
    this->_aabb.expand(newQuad.getOrigin() + newQuad.getSize());
  }

  for (const auto& currQuad : this->_wallsManager.getWallQuads()) {
    auto newQuad = WallQuad::makeWallFromMat4(currQuad, inTransform);
    this->_aabb.expand(newQuad.getOrigin());
    this->_aabb.expand(newQuad.getOrigin() + newQuad.getSize());
  }

  for (const auto& currInstance : _brickInstancesManager.getBrickInstances())
  {
    if (!currInstance.ref) {
      continue;
    }

    glm::mat4 tmpTransform = inTransform;
    tmpTransform = glm::translate(tmpTransform, currInstance.pos);
    tmpTransform = tmpTransform * glm::mat4_cast(currInstance.quat);
    currInstance.ref->computeAABB(tmpTransform);

    this->_aabb.expand(currInstance.ref->getAABB().getMin());
    this->_aabb.expand(currInstance.ref->getAABB().getMax());
  }

}

//MARK:intersect
// bool BrickModel::intersect(
//   const glm::mat4& inTransform,
//   const glm::vec3& inRayFrom,
//   const glm::vec3& inRayTo,
//   gero::math::RayCastResult& outData
// ) const {

//   bool hasHit = false;

//   const glm::vec3 minVec3 = glm::min(inRayFrom, inRayTo);
//   const glm::vec3 maxVec3 = glm::max(inRayFrom, inRayTo);

//   const glm::vec3 searchSize = (maxVec3 - minVec3);
//   const glm::vec3 searchCenter = minVec3 + searchSize * 0.5f;
//   std::vector<FloorQuad> foundQuads;
//   foundQuads.reserve(64);

//   // this->findFloorQuads(searchCenter, searchSize, foundQuads);

//   for (const BrickInstance& currInstance : this->_brickInstancesManager.getBrickInstances()) {
//     if (!currInstance.ref) {
//       continue;
//     }

//     this->_findFloorQuads(
//       // glm::identity<glm::mat4>(),
//       inTransform,
//       currInstance,
//       searchCenter,
//       searchSize,
//       foundQuads
//     );
//   }


//   // D_MYLOG("searchCenter " << searchCenter);
//   // D_MYLOG("searchSize " << searchSize);

//   if (foundQuads.size() > 0) {
//     D_MYLOG("foundQuads.size() " << foundQuads.size());
//   }

//   const glm::vec3 rayDir = inRayFrom - inRayTo;

//   for (const auto& currQuad : foundQuads) {
//     if (currQuad.intersect(inRayFrom, rayDir, outData)) {
//       hasHit = true;
//     }
//   }

//   return hasHit;
// }

//MARK:buildVertices
void BrickModel::buildVertices(
  IWireFramesAccumulator& inWireFrames,
  ITrianglesAccumulator& inTriangles
) const {
  // axis
  inWireFrames.pushLine(glm::vec3(0, 0, 0), glm::vec3(10, 0, 0), glm::vec3(1, 0, 0));
  inWireFrames.pushLine(glm::vec3(0, 0, 0), glm::vec3(0, 10, 0), glm::vec3(0, 1, 0));
  inWireFrames.pushLine(glm::vec3(0, 0, 0), glm::vec3(0, 0, 10), glm::vec3(0, 0, 1));

  for (const auto& currQuad : this->_floorsManager.getFloorQuads()) {
    currQuad.buildVertices_wireframes(inWireFrames);
    currQuad.buildVertices_triangles(inTriangles);
  }

  for (const auto& currQuad : this->_wallsManager.getWallQuads()) {
    currQuad.buildVertices_wireframes(inWireFrames);
    currQuad.buildVertices_triangles(inTriangles);
  }

}

//MARK:buildInstances
void BrickModel::buildInstances(
  IWireFramesAccumulator& inWireFrames,
  ITrianglesAccumulator& inTriangles,
  InstancedBrickModels& inInstancedBrickModels
) const {

  if (!inInstancedBrickModels.hasAlias(this->_name)) {

    inWireFrames.reset();
    inTriangles.reset();
    this->buildVertices(inWireFrames, inTriangles);

    {
      this->computeAABB(glm::identity<glm::mat4>());

      const glm::vec3 aabbSize = this->_aabb.getMax() - this->_aabb.getMin();
      const glm::vec3 extraScale = glm::vec3(0.1f) + aabbSize * 0.01f;

      std::array<glm::vec3, 8> vertices;
      vertices[0] = this->_aabb.getMin() - extraScale;
      vertices[1] = this->_aabb.getMin() - extraScale + ((aabbSize + extraScale*2.0f) * glm::vec3(1, 0, 0));
      vertices[2] = this->_aabb.getMax() + extraScale - ((aabbSize + extraScale*2.0f) * glm::vec3(0, 0, 1));
      vertices[3] = this->_aabb.getMin() - extraScale + ((aabbSize + extraScale*2.0f) * glm::vec3(0, 1, 0));

      vertices[4] = this->_aabb.getMin() - extraScale + ((aabbSize + extraScale*2.0f) * glm::vec3(0, 0, 1));
      vertices[5] = this->_aabb.getMax() + extraScale - ((aabbSize + extraScale*2.0f) * glm::vec3(0, 1, 0));
      vertices[6] = this->_aabb.getMax() + extraScale;
      vertices[7] = this->_aabb.getMax() + extraScale - ((aabbSize + extraScale*2.0f) * glm::vec3(1, 0, 0));

      inWireFrames.pushLine(vertices[0], vertices[1], glm::vec3(1,1,1));
      inWireFrames.pushLine(vertices[1], vertices[2], glm::vec3(1,1,1));
      inWireFrames.pushLine(vertices[2], vertices[3], glm::vec3(1,1,1));
      inWireFrames.pushLine(vertices[3], vertices[0], glm::vec3(1,1,1));

      inWireFrames.pushLine(vertices[4], vertices[5], glm::vec3(1,1,1));
      inWireFrames.pushLine(vertices[5], vertices[6], glm::vec3(1,1,1));
      inWireFrames.pushLine(vertices[6], vertices[7], glm::vec3(1,1,1));
      inWireFrames.pushLine(vertices[7], vertices[4], glm::vec3(1,1,1));

      inWireFrames.pushLine(vertices[0], vertices[4], glm::vec3(1,1,1));
      inWireFrames.pushLine(vertices[1], vertices[5], glm::vec3(1,1,1));
      inWireFrames.pushLine(vertices[2], vertices[6], glm::vec3(1,1,1));
      inWireFrames.pushLine(vertices[3], vertices[7], glm::vec3(1,1,1));
    }

    // D_MYLOG("inTriangles " << inTriangles._vertices.size());

    inInstancedBrickModels.createAlias(this->_name, inWireFrames.getVertices(), inTriangles.getVertices());
  }

  // {
  //   InstancedBrickModels::GeometryInstance newInstance;

  //   newInstance.position = inOrigin;
  //   newInstance.orientation = inQuat;
  //   newInstance.color = glm::vec4(1,1,1, 1);
  //   newInstance.light = false;
  //   newInstance.scale = glm::vec3(1,1,1);

  //   inInstancedBrickModels.pushAlias(this->_name, newInstance);
  // }

  for (const auto& currInstance : _brickInstancesManager.getBrickInstances())
  {
    if (!currInstance.ref) {
      D_MYERR("invalid ref");
      continue;
    }

    currInstance.ref->buildInstances(
      // inOrigin + inQuat * currInstance.pos,
      // currInstance.quat * inQuat,
      inWireFrames,
      inTriangles,
      inInstancedBrickModels
    );
  }
}

void BrickModel::pushNewInstances(
  const glm::vec3& inOrigin,
  const glm::quat& inQuat,
  InstancedBrickModels& inInstancedBrickModels
) const {

  {
    InstancedBrickModels::GeometryInstance newInstance;

    newInstance.position = inOrigin;
    newInstance.orientation = inQuat;
    newInstance.color = glm::vec4(1,1,1, 1);
    newInstance.light = false;
    newInstance.scale = glm::vec3(1,1,1);

    inInstancedBrickModels.pushAlias(this->_name, newInstance);
  }

  for (const auto& currInstance : _brickInstancesManager.getBrickInstances())
  {
    if (!currInstance.ref) {
      D_MYERR("invalid ref");
      continue;
    }

    currInstance.ref->pushNewInstances(
      inOrigin + inQuat * currInstance.pos,
      currInstance.quat * inQuat,
      // inWireFrames,
      // inTriangles,
      inInstancedBrickModels
    );
  }
}
