
#include "collisions.hpp"

namespace gero {
namespace math {

// bool collisionLineSeg_BBox(const t_vec2f& A, const t_vec2f& B, const t_vec2f& O, const t_vec2f& P)
// {
// 	t_vec2f minAB(std::min(A.x, B.x), std::min(A.y, B.y));
// 	t_vec2f minOP(std::min(O.x, P.x), std::min(O.y, P.y));
// 	t_vec2f maxAB(std::max(A.x, B.x), std::max(A.y, B.y));
// 	t_vec2f maxOP(std::max(O.x, P.x), std::max(O.y, P.y));

// 	return !(maxAB.x < minOP.x ||
// 			 minAB.x > maxOP.x ||
// 			 maxAB.y < minOP.y ||
// 			 minAB.y > maxOP.y);
// }

bool collisionRectRect(const glm::vec2& posA, const glm::vec2& sizeA, const glm::vec2& posB, const glm::vec2& sizeB) {
  return !(posA.x + sizeA.x < posB.x || posA.y + sizeA.y < posB.y || posA.x > posB.x + sizeB.x ||
           posA.y > posB.y + sizeB.y);
}

bool collisionLineSegment(const glm::vec2& A, const glm::vec2& B, const glm::vec2& O, const glm::vec2& P) {
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

bool collisionPointIsLeftOfLine(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c) {
  return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x) > 0.0f;
}

bool collisionPointCircle(const glm::vec2& P, const glm::vec2& C, float radius) {
  const float d2 = (P.x - C.x) * (P.x - C.x) + (P.y - C.y) * (P.y - C.y);

  return (d2 <= radius * radius);
}

bool collisionLineCircle(const glm::vec2& A, const glm::vec2& B, const glm::vec2& C, float radius) {
  // if (!CollisionLineCercle_BBox(A,B,C,radius))
  // 	return false;

  const glm::vec2 u(B.x - A.x, B.y - A.y);
  const glm::vec2 AC(C.x - A.x, C.y - A.y);

  float norm = u.x * AC.y - u.y * AC.x;
  if (norm < 0.0f) {
    norm = -norm; // must be [0..x]
  }

  const float denom = sqrtf(u.x * u.x + u.y * u.y);
  const float CI = norm / denom;

  return (CI < radius);
}

bool collisionSegmentCircle(const glm::vec2& A, const glm::vec2& B, const glm::vec2& C, float radius) {
  if (collisionLineCircle(A, B, C, radius) == false) {
    return false;
  }

  const glm::vec2 AB = B - A;
  const glm::vec2 AC = C - A;
  const glm::vec2 BC = C - B;

  const float scalarProdA = AB.x * AC.x + AB.y * AC.y;
  const float scalarProdB = (-AB.x) * BC.x + (-AB.y) * BC.y;

  if (scalarProdA >= 0 && scalarProdB >= 0) {
    return true;
  }

  // A/B can still be inside the circle
  return (collisionPointCircle(A, C, radius) || collisionPointCircle(B, C, radius));
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

} // namespace math
} // namespace gero
