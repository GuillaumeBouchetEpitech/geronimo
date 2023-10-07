
#include "collisions.hpp"

namespace gero {
namespace math {

bool collisionLineSegment(const glm::vec2& A, const glm::vec2& B, const glm::vec2& O, const glm::vec2& P)
{
  const glm::vec2 AB = glm::vec2(B.x - A.x, B.y - A.y);
  const glm::vec2 AP = glm::vec2(P.x - A.x, P.y - A.y);
  const glm::vec2 AO = glm::vec2(O.x - A.x, O.y - A.y);

  return ((AB.x * AP.y - AB.y * AP.x) * (AB.x * AO.y - AB.y * AO.x) < 0.0f);
};

float rawCollisionSegmentSegment(const glm::vec2& A, const glm::vec2& B, const glm::vec2& O, const glm::vec2& P) {

  if (!collisionLineSegment(A, B, O, P)) {
    return -1.0f;
  }

  const glm::vec2 AB = glm::vec2(B.x - A.x, B.y - A.y);
  const glm::vec2 OP = glm::vec2(P.x - O.x, P.y - O.y);

  return -(A.x * OP.y - O.x * OP.y - OP.x * A.y + OP.x * O.y) / (AB.x * OP.y - AB.y * OP.x);
};

bool collisionSegmentSegment(const glm::vec2& A, const glm::vec2& B, const glm::vec2& O, const glm::vec2& P) {

  const float k = rawCollisionSegmentSegment(A, B, O, P);

  return (k > 0.0f && k < 1.0f);
};

bool pointIsLeftOfLine(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c)
{
  return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x) > 0.0f;
}


// static validateConcaveShape(polygonVertices) {

//   // build all possible edges from the provided vertices
//   const edges = [];
//   for (let ii = 1; ii < polygonVertices.length; ++ii)
//     edges.push([ ii - 1, ii ]);

//   const intersectingEdges = [];

//   // efficiently generate all unique combinations of edges
//   // example for [0,1,2] => (0->1, 0->2, 1->2)
//   for (let ii = 0; ii < edges.length; ++ii)
//   for (let jj = ii + 1; jj < edges.length; ++jj) {

//     const A = polygonVertices[ edges[ii][0] ];
//     const B = polygonVertices[ edges[ii][1] ];
//     const O = polygonVertices[ edges[jj][0] ];
//     const P = polygonVertices[ edges[jj][1] ];

//     const edgesDoIntersect = MathUtilities._collisionSegSeg(A, B, O, P);

//     if (edgesDoIntersect) {
//       // invalid concave shape
//       intersectingEdges.push(`[${edges[ii][0]},${edges[ii][1]}] -> [${edges[jj][0]},${edges[jj][1]}]`);
//     }
//   }

//   if (intersectingEdges.length > 0)
//     throw new Error(`invalid edges for vertices: ${intersectingEdges.join(",")}`);

//   return true; // valid concave shape
// };

}
}
