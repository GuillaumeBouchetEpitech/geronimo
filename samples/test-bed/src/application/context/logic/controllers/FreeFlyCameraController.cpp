
#include "FreeFlyCameraController.hpp"

#include "application/context/logic/inputManagers/KeyboardManager.hpp"
#include "application/context/logic/inputManagers/MouseManager.hpp"

#include "application/context/Context.hpp"

void FreeFlyCameraController::update(float elapsed_time) {
  auto& context = Context::get();

  { // mouse

    // auto& mouse = context.inputs.mouse;
    auto& mouse = MouseManager::get();

    _horizontalAngle -= float(mouse.getDelta().x) / 5.f;
    _verticalAngle -= float(mouse.getDelta().y) / 5.f;

    mouse.resetDelta();

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

    auto& camera = context.graphic.camera.scene;

    camera.lookAt(_position, _position + _forwardAxis, glm::vec3(0, 0, 1));

  } // mouse

  { // keyboard

    // auto& keys = context.inputs.keys;

    auto& keyboard = KeyboardManager::get();

    const bool moveForward = keyboard.isPressed(SDLK_UP, SDLK_w, SDLK_z);
    const bool moveBackward = keyboard.isPressed(SDLK_DOWN, SDLK_s);
    const bool strafeLeft = keyboard.isPressed(SDLK_LEFT, SDLK_q, SDLK_a);
    const bool strafeRight = keyboard.isPressed(SDLK_RIGHT, SDLK_d);

    // const bool moveForward =
    //   (keys[SDLK_UP] || // ARROW
    //    keys[SDLK_w] ||  // QWERTY (UK-US keyboard layout)
    //    keys[SDLK_z]     // AZERTY (FR keyboard layout)
    //   );

    // const bool moveBackward =
    //   (keys[SDLK_DOWN] || // ARROW
    //    keys[SDLK_s]);

    // const bool strafeLeft =
    //   (keys[SDLK_LEFT] || // ARROW
    //    keys[SDLK_q] ||    // QWERTY (UK-US keyboard layout)
    //    keys[SDLK_a]       // AZERTY (FR keyboard layout)
    //   );

    // const bool strafeRight =
    //   (keys[SDLK_RIGHT] || // ARROW
    //    keys[SDLK_d]);

    constexpr float speed = 16;
    glm::vec3 acceleration(0, 0, 0);

    if (moveForward)
      acceleration += _forwardMovement * elapsed_time * speed;
    else if (moveBackward)
      acceleration -= _forwardMovement * elapsed_time * speed;

    if (strafeLeft)
      acceleration -= _leftMovement * elapsed_time * speed;
    else if (strafeRight)
      acceleration += _leftMovement * elapsed_time * speed;

    _position += acceleration;

  } // keyboard

  { // death raycast

    // auto& mouse = context.inputs.mouse;

    // if (_raycastCooldown >= 0.0f)
    //     _raycastCooldown -= elapsed_time;

    // _target = _position + _forward  * 50.0f;

    // PhysicWorld::RaycastParams params(_position, _target, 0.0f);

    // params.collisionGroup = gero::asValue(Groups::player);
    // params.collisionMask = gero::asValue(Groups::dynamic);

    // // params.radius = 0.25f;

    // context.physic.world->getRaycaster().raycast(params);

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
    //         if (_raycastCooldown < 0.0f)
    //         {
    //             _raycastCooldown = 0.06125f;
    //             // _raycastCooldown = 0.0313f;

    //             auto& normal = params.result.impactNormal;

    //             glm::vec3 finalTarget = target - normal * 0.25f;
    //             context.logic.voxelManager.removeVoxel(finalTarget,
    //             params.result.body);
    //         }
    //     }
    //     if (mouse.buttons[SDL_BUTTON_RIGHT])
    //     {
    //         if (_raycastCooldown < 0.0f)
    //         {
    //             _raycastCooldown = 0.06125f;
    //             // _raycastCooldown = 0.0313f;

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
    // //     if (_raycastCooldown < 0.0f)
    // //     {
    // //         _raycastCooldown = 0.06125f;

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

  } // death raycast

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

// const glm::vec3& FreeFlyCameraController::getTarget() const
// {
//     return _target;
// }
