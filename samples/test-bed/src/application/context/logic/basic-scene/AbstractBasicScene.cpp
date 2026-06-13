
#include "AbstractBasicScene.hpp"

#include "internals/BasicScene.hpp"

std::unique_ptr<AbstractBasicScene> AbstractBasicScene::create() {
  return std::make_unique<BasicScene>();
}
