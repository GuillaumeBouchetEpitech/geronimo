
#pragma once

#include <memory>

class AbstractArtificialStupiditySim {
public:
  virtual ~AbstractArtificialStupiditySim() = default;

public:
  virtual void update(float deltaTimeSec) = 0;
  virtual void renderScene() = 0;
  virtual void renderHUD() = 0;

public:
  static std::unique_ptr<AbstractArtificialStupiditySim> create();

};
