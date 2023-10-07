
#include "geronimo/system/TraceLogger.hpp"
#include "geronimo/system/math/2d/convex-polygon/ConvexPolygon.hpp"

#include "gtest/gtest.h"

TEST(system_ConvexPolygon, contains) {

  {
    gero::math::ConvexPolygon polygon;
    polygon.preAllocate(3);

    polygon.addVertex(glm::vec2(0,0));
    polygon.addVertex(glm::vec2(0,10));
    polygon.addVertex(glm::vec2(10,0));

    ASSERT_EQ(polygon.contains(glm::vec2(1,1)), true);

    ASSERT_EQ(polygon.contains(glm::vec2(-111,1)), false);
    ASSERT_EQ(polygon.contains(glm::vec2(111,1)), false);
    ASSERT_EQ(polygon.contains(glm::vec2(1,-111)), false);
    ASSERT_EQ(polygon.contains(glm::vec2(1,111)), false);
  }


}

TEST(system_ConvexPolygon, intersect) {

  {
    gero::math::ConvexPolygon polygon;
    polygon.preAllocate(3);

    polygon.addVertex(glm::vec2(0,0));
    polygon.addVertex(glm::vec2(0,10));
    polygon.addVertex(glm::vec2(10,10));
    polygon.addVertex(glm::vec2(10,0));

    ASSERT_NEAR(polygon.intersect(glm::vec2(-15,+5), glm::vec2(-5,5)), 1.5f, 0.001f);
    ASSERT_NEAR(polygon.intersect(glm::vec2(-5,5), glm::vec2(+5,5)), 0.5f, 0.001f);
    ASSERT_NEAR(polygon.intersect(glm::vec2(+5,5), glm::vec2(+15,5)), 0.5f, 0.001f);
    ASSERT_NEAR(polygon.intersect(glm::vec2(+15,5), glm::vec2(+25,5)), -1.0f, 0.001f);

  }

}


TEST(system_ConvexPolygon, validate) {

  {
    gero::math::ConvexPolygon polygon;
    polygon.preAllocate(4);

    polygon.addVertex(glm::vec2(0,0));
    polygon.addVertex(glm::vec2(0,10));
    polygon.addVertex(glm::vec2(10,10));
    polygon.addVertex(glm::vec2(10,0));

    polygon.validate();

    ASSERT_NEAR(polygon.getVertices().at(0).x,  0.0f, 0.001f);
    ASSERT_NEAR(polygon.getVertices().at(0).y,  0.0f, 0.001f);

    ASSERT_NEAR(polygon.getVertices().at(1).x,  0.0f, 0.001f);
    ASSERT_NEAR(polygon.getVertices().at(1).y, 10.0f, 0.001f);

    ASSERT_NEAR(polygon.getVertices().at(2).x, 10.0f, 0.001f);
    ASSERT_NEAR(polygon.getVertices().at(2).y, 10.0f, 0.001f);

    ASSERT_NEAR(polygon.getVertices().at(3).x, 10.0f, 0.001f);
    ASSERT_NEAR(polygon.getVertices().at(3).y,  0.0f, 0.001f);

  }

  {
    gero::math::ConvexPolygon polygon;
    polygon.preAllocate(4);

    polygon.addVertex(glm::vec2(10,0));
    polygon.addVertex(glm::vec2(10,10));
    polygon.addVertex(glm::vec2(0,10));
    polygon.addVertex(glm::vec2(0,0));

    polygon.validate();

    ASSERT_NEAR(polygon.getVertices().at(0).x,  0.0f, 0.001f);
    ASSERT_NEAR(polygon.getVertices().at(0).y,  0.0f, 0.001f);

    ASSERT_NEAR(polygon.getVertices().at(1).x,  0.0f, 0.001f);
    ASSERT_NEAR(polygon.getVertices().at(1).y, 10.0f, 0.001f);

    ASSERT_NEAR(polygon.getVertices().at(2).x, 10.0f, 0.001f);
    ASSERT_NEAR(polygon.getVertices().at(2).y, 10.0f, 0.001f);

    ASSERT_NEAR(polygon.getVertices().at(3).x, 10.0f, 0.001f);
    ASSERT_NEAR(polygon.getVertices().at(3).y,  0.0f, 0.001f);

  }

}






// TEST(system_ConvexPolygon, limitate) {

//   {
//     gero::math::ConvexPolygon polygon;
//     polygon.preAllocate(4);

//     polygon.addVertex(glm::vec2(0,0));
//     polygon.addVertex(glm::vec2(0,10));
//     polygon.addVertex(glm::vec2(10,10));
//     polygon.addVertex(glm::vec2(10,0));

//     ASSERT_NEAR(polygon.limitate(glm::vec2(15,5)).x, 10.0f, 0.001f);
//     ASSERT_NEAR(polygon.limitate(glm::vec2(15,5)).y,  5.0f, 0.001f);

//     ASSERT_NEAR(polygon.limitate(glm::vec2(5,15)).x,  5.0f, 0.001f);
//     ASSERT_NEAR(polygon.limitate(glm::vec2(5,15)).y, 10.0f, 0.001f);

//     ASSERT_NEAR(polygon.limitate(glm::vec2(-5,5)).x,  0.0f, 0.001f);
//     ASSERT_NEAR(polygon.limitate(glm::vec2(-5,5)).y,  0.0f, 0.001f);

//     ASSERT_NEAR(polygon.limitate(glm::vec2(5,-5)).x,  5.0f, 0.001f);
//     ASSERT_NEAR(polygon.limitate(glm::vec2(5,-5)).y,  0.0f, 0.001f);

//   }

// }




