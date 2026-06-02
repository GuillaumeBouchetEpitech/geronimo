
#include "PlatformerCharacter.hpp"

#include "KeyframesManager.hpp"

#include "application/context/graphics/renderers/scene/GraphicSceneRenderer.hpp"

#include "geronimo/system/math/clamp.hpp"


namespace /*anonymous*/ {

  constexpr float k_maxAcc = 200.0f;
  constexpr float k_maxVel = 30.0f;

  constexpr float k_gravity = 150.0f;
  constexpr float k_jumpForce = 40.0f;

  constexpr float k_repelForce = 40.0f;

  constexpr float k_dashMaxVel = 60.0f;
  constexpr float k_isDashing_animLockedCooldown = 0.15f;
  constexpr float k_canDash_leftCooldown = 1.0f;
  constexpr float k_dashForce = 30.0f;

  constexpr float k_canAttack_leftCooldown = 0.25f;
  constexpr float k_isAttacking_animLockedCooldown = 0.1f;

  constexpr float k_isHurt_animLockedCooldown = 0.25f;
  constexpr float k_isInvincible_leftCooldown = 0.5f;

};

//MARK:ctor
PlatformerCharacter::PlatformerCharacter()
{
  _states.reset();
  _controls.reset();
}

//MARK:update
void PlatformerCharacter::update(float deltaTimeSec)
{
  _elapsedTime += deltaTimeSec;

  // apply gravity
  // apply controls
  // apply velocity

  // apply gravity
  if (_states.isDashing_animLockedCooldown == 0.0f) {

    _states.velocity.y = std::max(_states.velocity.y - k_gravity * deltaTimeSec, -k_maxVel);

    // D_MYLOG("_states.velocity.y " << _states.velocity.y);

    if (_states.position.y <= 0.0f) {
      _states.position.y = 0.0f;
      _states.velocity.y = 0.0f;
      _states.isOnTheGround = true;
      _states.canDash = true;
    } else {
      _states.isOnTheGround = false;
    }
  }

  // apply controls (step 1)
  const bool isAnimLocked = _states.isAnimLocked();

  if (_states.isDashing_animLockedCooldown > 0.0f) {
    _states.velocity.y = 0.0f;
    if (_states.facingDir.x < 0.0f) {
      _states.velocity.x = std::max(_states.velocity.x - k_dashForce, -k_dashMaxVel);
    } else {
      _states.velocity.x = std::min(_states.velocity.x + k_dashForce, +k_dashMaxVel);
    }
  }

  // apply cooldowns
  _states.updateCooldowns(deltaTimeSec);

  // apply controls (step 2)
  if (!isAnimLocked)
  {

    // walking left/right

    if (_controls.isMovingLeft) {
      _states.facingDir.x = -1.0f;
      _states.velocity.x = std::max(_states.velocity.x - k_maxAcc * deltaTimeSec, -k_maxVel);
    }
    else if (_controls.isMovingRight) {
      _states.facingDir.x = +1.0f;
      _states.velocity.x = std::min(_states.velocity.x + k_maxAcc * deltaTimeSec, +k_maxVel);
    }
    else {
      // going left? -> decelerate right
      if (_states.velocity.x < 0.0f) {
        _states.velocity.x = std::min(_states.velocity.x + k_maxAcc * deltaTimeSec, 0.0f);
      }
      // going right? -> decelerate left
      else if (_states.velocity.x > 0.0f) {
        _states.velocity.x = std::max(_states.velocity.x - k_maxAcc * deltaTimeSec, 0.0f);
      }
    }

    // inital jump

    if (_controls.isJumping && _states.isOnTheGround) {
      _states.velocity.y += k_jumpForce;
      _states.isOnTheGround = false;
    }

    // double jump

    // TODO

    // dashing

    if (_controls.isDashing && _states.canDash && _states.canDash_leftCooldown == 0.0f)
    {
      _states.canDash_leftCooldown = k_canDash_leftCooldown;
      _states.canDash = false;

      _states.isDashing_animLockedCooldown = k_isDashing_animLockedCooldown;
      _states.isInvincible_leftCooldown = _states.isDashing_animLockedCooldown;

      _states.velocity.y = 0.0f;

      if (_states.facingDir.x < 0.0f) {
      _states.velocity.x = std::max(_states.velocity.x - k_dashForce, -k_dashMaxVel);
      } else {
      _states.velocity.x = std::min(_states.velocity.x + k_dashForce, +k_dashMaxVel);
      }
    }

    // attacking

    if (_controls.isAttacking && _states.canAttack_leftCooldown == 0.0f)
    {
      _states.facingDir.y = 0.0f;
      if (_controls.isLookingUp) {
        _states.facingDir.y = +1.0f;
      }
      else if (_controls.isLookingDown && !_states.isOnTheGround) {
        _states.facingDir.y = -1.0f;
      }

      // D_MYLOG("_states.facingDir.y " << _states.facingDir.y);

      _states.canAttack_leftCooldown = k_canAttack_leftCooldown;
      _states.isAttacking_animLockedCooldown = k_isAttacking_animLockedCooldown;
    }

  }
  else {

    if (_states.isDashing_animLockedCooldown > 0.0f) {

      // if dashing -> do nothing?

    } else {

      // going left? -> decelerate right
      if (_states.velocity.x < 0.0f) {
        _states.velocity.x = std::max(_states.velocity.x, -k_maxVel);
        _states.velocity.x = std::min(_states.velocity.x + k_maxAcc * deltaTimeSec, 0.0f);
      }
      // going right? -> decelerate left
      else if (_states.velocity.x > 0.0f) {
        _states.velocity.x = std::min(_states.velocity.x, +k_maxVel);
        _states.velocity.x = std::max(_states.velocity.x - k_maxAcc * deltaTimeSec, 0.0f);
      }
    }

  }

  // apply velocity
  _states.position += _states.velocity * deltaTimeSec;

  // clamp the position (temporary measure?)
  _states.position.x = gero::math::clamp(_states.position.x, -20.0f, +20.0f);
  // _states.position.y = gero::math::clamp(_states.position.y, -10.0f, +50.0f);
  _states.position.y = gero::math::clamp(_states.position.y, 0.0f, +20.0f);

}

//MARK:handleCollisions
void PlatformerCharacter::handleCollisions(PlatformerCharacter& other, KeyframesManager& keyframesManager)
{
  if (
    this->_states.isInvincible_leftCooldown > 0.0f ||
    other._states.isInvincible_leftCooldown > 0.0f
  ) {
    return;
  }

  // constexpr float k_scale = 10.0f / 400.0f;
  constexpr float k_scale = 10.0f / 1.0f;

  std::vector<RectData> this_health;
  std::vector<RectData> this_attack;
  std::vector<RectData> other_health;
  std::vector<RectData> other_attack;

  // pre-allocate
  this_health.reserve(10);
  this_attack.reserve(10);
  other_health.reserve(10);
  other_attack.reserve(10);

  std::string this_alias = "1000";
  std::string other_alias = "1000";

  {
    bool flipX = false;
    this->_getCurrentAnimationAlias(this_alias, flipX);
    keyframesManager.getColliders(this_alias, glm::vec3(this->_states.position,0), k_scale, flipX, this_health, this_attack);
  }

  {
    bool flipX = false;
    other._getCurrentAnimationAlias(other_alias, flipX);
    keyframesManager.getColliders(other_alias, glm::vec3(other._states.position,0), k_scale, flipX, other_health, other_attack);
  }

  // collision this_attack/other_health
  // collision other_attack/this_health
  // collision this_health/other_health
  // collision this_attack/other_attack

  struct CollisionTest {
    const std::vector<RectData>* testA = nullptr;
    const std::vector<RectData>* testB = nullptr;
    bool collide = false;
  };

  std::array<CollisionTest, 3> allCollisionTests = {{
    { &this_attack, &other_health, false },
    { &this_health, &other_attack, false },
    { &this_attack, &other_attack, false },
    // { &this_health, &other_health, false },
  }};

  for (CollisionTest& currTest : allCollisionTests)
  {
    currTest.collide = false;

    const std::vector<RectData>& testA = *(currTest.testA);
    const std::vector<RectData>& testB = *(currTest.testB);

    if (testA.empty() || testB.empty()) {
      // one of rect list is empty -> cannot collide -> skip collision
      continue;
    }

    for (std::size_t ii = 0; !currTest.collide && ii < testA.size(); ++ii)
    for (std::size_t jj = 0; !currTest.collide && jj < testB.size(); ++jj) {
      if (testA.at(ii).testOverlap(testB.at(jj))) {

        // D_MYLOG("overlap");
        // D_MYLOG(" - testA.at(ii).bottomLeft " << testA.at(ii).bottomLeft);
        // D_MYLOG("   testA.at(ii).topRight   " << testA.at(ii).topRight);
        // D_MYLOG(" - testB.at(jj).bottomLeft " << testB.at(jj).bottomLeft);
        // D_MYLOG("   testB.at(jj).topRight   " << testB.at(jj).topRight);

        currTest.collide = true;
      }
    }
  }

  bool this_injured = false;
  bool this_repelled = false;
  bool other_injured = false;
  bool other_repelled = false;

  // disabled for now
  // if (allCollisionTests[0].collide) {
  //   // both injured/repelled
  //   this_injured = this_repelled = true;
  //   other_injured = other_repelled = true;
  // }
  if (allCollisionTests.at(0).collide) {
    // other injured
    // this repelled
    other_injured = true;
    other_repelled = true;
    this_repelled = true;
  }
  if (allCollisionTests.at(1).collide) {
    // this injured
    // other repelled
    this_injured = true;
    this_repelled = true;
    other_repelled = true;
  }
  if (allCollisionTests.at(2).collide) {
    // both repelled
    this_repelled = true;
    other_repelled = true;
  }

  if (this_injured) {
    this->_states.isHurt_animLockedCooldown = k_isHurt_animLockedCooldown;
    this->_states.isInvincible_leftCooldown = k_isInvincible_leftCooldown;
    this->_states.health -= 1;
  }
  if (other_injured) {
    other._states.isHurt_animLockedCooldown = k_isHurt_animLockedCooldown;
    other._states.isInvincible_leftCooldown = k_isInvincible_leftCooldown;
    other._states.health -= 1;
  }

  if (this_repelled || other_repelled) {

    glm::vec2 diff = (this->_states.position - other._states.position);
    float diffLength = glm::length(diff);
    if (diffLength > 0.0f) {

      diff /= diffLength; // normalize
      diff *= k_repelForce; // repel force

      if (this_repelled) {
        // must set the velocity (instead of adding it)
        this->_states.velocity = +diff;

        // is aerial down attack?
        if (this_alias == "2003") {
          // allow dash again if was used
          this->_states.canDash = true;
        }
      }

      if (other_repelled) {
        // must set the velocity (instead of adding it)
        other._states.velocity = -diff;

        // is aerial down attack?
        if (other_alias == "2003") {
          // allow dash again if was used
          other._states.canDash = true;
        }
      }

    }
  }

}

//MARK:simulate
PlatformerCharacterStates PlatformerCharacter::simulate(
  float deltaTimeSec,
  const PlatformerCharacter& other,
  KeyframesManager& inKeyframesManager,
  const PlatformerCharacterControls& inControls
) const {

  PlatformerCharacter tmpSelfCharacter = *this; // hard copy
  PlatformerCharacter tmpOpponentCharacter = other; // hard copy

  // apply controls
  tmpSelfCharacter._controls = inControls;

  // regular update
  tmpSelfCharacter.update(deltaTimeSec);
  tmpOpponentCharacter.handleCollisions(tmpSelfCharacter, inKeyframesManager);

  return tmpSelfCharacter._states;
}

//MARK:renderDebug
void PlatformerCharacter::renderDebug(gero::graphics::opengl::advanced::IStackRenderers& stackRenderers, const glm::vec3& inOrigin) const
{
  auto& wireFrames = stackRenderers.getWireFramesStack();

  const glm::vec3 k_origin = inOrigin + glm::vec3(_states.position, 0.0f);

  wireFrames.pushCross(k_origin, glm::vec3(1,1,0), 0.25f);
}


//MARK:renderAnimation
void PlatformerCharacter::renderAnimation(IGraphicSceneRenderer& scene, KeyframesManager& keyframesManager, const glm::vec3& inOrigin, bool debugMode) const
{
  // constexpr float k_scale = 10.0f / 400.0f;
  constexpr float k_scale = 10.0f / 1.0f;

  std::string alias = "1000";
  bool flipX = false;
  _getCurrentAnimationAlias(alias, flipX);

  glm::vec3 color = glm::vec3(1,1,1);
  if (_states.isInvincible_leftCooldown > 0.0f) {
    const int32_t step = int32_t(_states.isInvincible_leftCooldown * 8.0f) % 2;
    if (step == 0) {
      color = glm::vec3(0.5f, 0.5f, 0.5f);
    } else {
      color = glm::vec3(0.0f, 0.0f, 0.0f);
    }
  }

  keyframesManager.renderFrame(
    alias,
    inOrigin + glm::vec3(_states.position,0),
    color * this->_color,
    k_scale,
    flipX,
    debugMode
  );
}


//MARK:_getCurrentAnimationAlias
void PlatformerCharacter::_getCurrentAnimationAlias(std::string& out_alias, bool& out_flipX) const
{
  out_flipX = _states.facingDir.x < 0.0f;

  if (_states.isDashing_animLockedCooldown > 0.0f) {
    out_alias = "3000";
  } else if (_states.isOnTheGround) {

    if (_states.isAttacking_animLockedCooldown > 0.0f) {

      if (_states.facingDir.y > 0.0f) {
        out_alias = "1003";
      } else {
        out_alias = "1002";
      }

    } else if (_states.velocity.x == 0.0f) {
      out_alias = "1000";
    } else {

      // running
      const int32_t step = int32_t(this->_elapsedTime * 8.0f) % 2;
      out_alias = step == 0 ? "1011" : "1001";
    }

  } else {

    if (_states.isAttacking_animLockedCooldown > 0.0f) {
      if (_states.facingDir.y == 0.0f) {
        out_alias = "2001";
      } else if (_states.facingDir.y > 0.0f) {
        out_alias = "2002";
      } else {
        out_alias = "2003";
      }

    } else {
      out_alias = _states.velocity.y > 0.0f ? "2100" : "2101";
    }

  }

}

