
#include "FreeFlyCameraController.hpp"

#include "geronimo/graphics/input-managers/KeyboardManager.hpp"
#include "geronimo/graphics/input-managers/MouseManager.hpp"
#include "geronimo/graphics/input-managers/TouchManager.hpp"

#include "application/context/Context.hpp"

void FreeFlyCameraController::update(float elapsedTime) {
  auto& context = Context::get();

  { // mouse

    auto& keyboard = KeyboardManager::get();

    const bool lookUp = keyboard.isPressed(SDLK_UP);
    const bool lookDown = keyboard.isPressed(SDLK_DOWN);
    const bool lookLeft = keyboard.isPressed(SDLK_LEFT);
    const bool lookRight = keyboard.isPressed(SDLK_RIGHT);

    constexpr float k_lookSpeed = 1.1f;
    if (lookUp) {
      _verticalAngle += k_lookSpeed;
    } else if (lookDown) {
      _verticalAngle -= k_lookSpeed;
    }

    if (lookLeft) {
      _horizontalAngle += k_lookSpeed;
    } else if (lookRight) {
      _horizontalAngle -= k_lookSpeed;
    }

    const bool hasTouchEvent = TouchManager::get().getTouchData(0).has_value();
    const bool hasMouseEvent = MouseManager::get().isLocked();

    // touch OR mouse, not both

    if (hasTouchEvent) {

      // touch event(s)

      if (_doubleTapTimeLeft > 0.0f) {
        _doubleTapTimeLeft -= elapsedTime;
      }

      if (auto touch = TouchManager::get().getTouchData(0)) {

        if (_doubleTapTimeLeft > 0.0f) {
          _forceForward = true;
        } else {
          _forceForward = false;
          _doubleTapTimeLeft = 0.25f;
        }

        _horizontalAngle -= float(touch->get().delta.x) / 5.0f;
        _verticalAngle -= float(touch->get().delta.y) / 5.0f;
      }

    } else if (hasMouseEvent) {

      // mouse event(s)

      auto& mouse = MouseManager::get();

      if (mouse.isLocked()) {
        _horizontalAngle -= float(mouse.getDelta().x) / 5.f;
        _verticalAngle -= float(mouse.getDelta().y) / 5.f;
      }
      mouse.resetDelta();

    } else {
      _forceForward = false;
      _doubleTapTimeLeft = 0.0f;
    }

    _verticalAngle = std::min(std::max(_verticalAngle, -89.f), 89.f);

    constexpr float halfPi = float(M_PI_2);

    const float vertAngleRad = glm::radians(_verticalAngle);
    const float horAngleRad = glm::radians(_horizontalAngle);

    const float horAngleCos = std::cos(horAngleRad);
    const float horAngleSin = std::sin(horAngleRad);
    const float vertAngleSin = std::sin(vertAngleRad);

    const float upRadius = std::cos(vertAngleRad - halfPi);
    _upAxis.z = std::sin(vertAngleRad - halfPi);
    _upAxis.x = upRadius * horAngleCos;
    _upAxis.y = upRadius * horAngleSin;

    const float forwardRadius = std::cos(vertAngleRad);
    _forwardAxis.z = vertAngleSin;
    _forwardAxis.x = forwardRadius * horAngleCos;
    _forwardAxis.y = forwardRadius * horAngleSin;

    // const float forwardRadius2 = std::cos(vertAngleRad);
    _forwardMovement.z = vertAngleSin;
    _forwardMovement.x = forwardRadius * horAngleCos;
    _forwardMovement.y = forwardRadius * horAngleSin;

    _leftMovement.x = _upAxis.y * _forwardAxis.z - _upAxis.z * _forwardAxis.y;
    _leftMovement.y = _upAxis.z * _forwardAxis.x - _upAxis.x * _forwardAxis.z;
    _leftMovement.z = _upAxis.x * _forwardAxis.y - _upAxis.y * _forwardAxis.x;

    context.graphic.renderer.getSceneRenderer().lookAt(_position, _position + _forwardAxis, glm::vec3(0, 0, 1));

  } // mouse

  { // keyboard

    // auto& keys = context.inputs.keys;

    auto& keyboard = KeyboardManager::get();

    const bool moveForward = keyboard.isPressed(SDLK_w, SDLK_z);
    const bool moveBackward = keyboard.isPressed(SDLK_s);
    const bool strafeLeft = keyboard.isPressed(SDLK_q, SDLK_a);
    const bool strafeRight = keyboard.isPressed(SDLK_d);
    const bool moveDown = keyboard.isPressed(SDLK_c);
    const bool moveUp = keyboard.isPressed(SDLK_SPACE);
    const bool accelerated = keyboard.isPressed(SDLK_LSHIFT);

    const float speed = 16.0f * (accelerated ? 4.0f : 1.0f);
    glm::vec3 acceleration(0, 0, 0);

    if (moveForward || _forceForward)
      acceleration += _forwardMovement * elapsedTime * speed;
    else if (moveBackward)
      acceleration -= _forwardMovement * elapsedTime * speed;

    if (strafeLeft)
      acceleration -= _leftMovement * elapsedTime * speed;
    else if (strafeRight)
      acceleration += _leftMovement * elapsedTime * speed;

    if (moveUp)
      acceleration -= _upAxis * elapsedTime * speed;
    else if (moveDown)
      acceleration += _upAxis * elapsedTime * speed;

    _position += acceleration;

  } // keyboard

  { // death rayCast

    // auto& mouse = context.inputs.mouse;

    // if (_rayCastCooldown >= 0.0f)
    //     _rayCastCooldown -= elapsedTime;

    // _target = _position + _forward  * 50.0f;

    // PhysicWorld::RayCastParams params(_position, _target, 0.0f);

    // params.collisionGroup = gero::asValue(Groups::player);
    // params.collisionMask = gero::asValue(Groups::dynamic);

    // // params.radius = 0.25f;

    // context.physic.world->getRayCaster().rayCast(params);

    // // if (target_body)
    // // {
    // //     _target = _position + _forward  * fixed_length;
    // // }

    // if (params.result.hasHit)
    // {
    //     auto& target = params.result.impactPoint;

    //     _target = target;

    // //     // D_MYLOG("target= " << target.x << "/" << target.y << "/" <<
    // target.z);

    //     // if (!target_body)
    //     // {
    //     //     _target = target;
    //     // }

    //     if (mouse.buttons[SDL_BUTTON_LEFT])
    //     {
    //         if (_rayCastCooldown < 0.0f)
    //         {
    //             _rayCastCooldown = 0.06125f;
    //             // _rayCastCooldown = 0.0313f;

    //             auto& normal = params.result.impactNormal;

    //             glm::vec3 finalTarget = target - normal * 0.25f;
    //             context.logic.voxelManager.removeVoxel(finalTarget,
    //             params.result.body);
    //         }
    //     }
    //     if (mouse.buttons[SDL_BUTTON_RIGHT])
    //     {
    //         if (_rayCastCooldown < 0.0f)
    //         {
    //             _rayCastCooldown = 0.06125f;
    //             // _rayCastCooldown = 0.0313f;

    //             auto& normal = params.result.impactNormal;

    //             glm::vec3 finalTarget = target + normal * 0.25f;
    //             context.logic.voxelManager.extrudeVoxel(finalTarget,
    //             params.result.body);
    //         }
    //     }

    //     // if (mouse.buttons[SDL_BUTTON_LEFT])
    //     // {
    //     //     target_body = params.result.body;

    //     //     target_body->getRelativeCoord(_target, target_pos);

    //     //     fixed_length = glm::length(_position - _target);
    //     // }

    // }

    // // if (mouse.buttons[SDL_BUTTON_RIGHT])
    // // {
    // //     target_body = nullptr;
    // // }

    // // if (mouse.buttons[SDL_BUTTON_LEFT])
    // // {
    // //     if (_rayCastCooldown < 0.0f)
    // //     {
    // //         _rayCastCooldown = 0.06125f;

    // //         Projectile::Def proj_def;
    // //         proj_def.position = _position;
    // //         // proj_def.position = _position + _leftMovement * 0.75f + _up
    // * 0.5f + _forward * 0.25f;
    // //         // proj_def.linear_velocity = _forward * 50.0f;

    // //         // glm::vec3 vel = glm::normalize(proj_def.position + _forward
    // * 1000.0f);
    // //         glm::vec3 vel = glm::normalize(_position + _forward * 50.0f -
    // proj_def.position);
    // //         proj_def.linear_velocity = vel * 150.0f;

    // //         Projectile* proj = new Projectile();
    // //         proj->initialize(proj_def);

    // //         context.logic.projectiles.push_back(proj);
    // //     }
    // // }

  } // death rayCast

  // if (target_body)
  // {
  //     glm::vec3 pos = target_body->getPosition();

  //     // glm::vec3 force = (_target - pos) * 2.0f;
  //     // target_body->applyForce(force, target_pos);

  //     glm::vec3 force = (_target - pos) * 0.2f;
  //     target_body->applyCentralImpulse(force);
  // }
}

//

void FreeFlyCameraController::setPosition(const glm::vec3& pos) { _position = pos; }

// const glm::vec3& FreeFlyCameraController::getTarget() const
// {
//     return _target;
// }
