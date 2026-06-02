
#pragma once

#include "geronimo/graphics/opengl/advanced/stackRenderers/StackRenderers.hpp"
#include "geronimo/helpers/GLMath.hpp"

// forward declaration
class IGraphicSceneRenderer;
class KeyframesManager;

// MARK: PlatformerCharacterStates
struct PlatformerCharacterStates
{
  glm::vec2 position = { 0.0f, 0.0f };
  glm::vec2 velocity = { 0.0f, 0.0f };
  glm::vec2 facingDir = { 1.0f, 0.0f };
  float isAttacking_animLockedCooldown = 0.0f;
  float isDashing_animLockedCooldown = 0.0f;
  float isHurt_animLockedCooldown = 0.0f;
  bool isOnTheGround = false;
  float isInvincible_leftCooldown = 0.0f;
  float canAttack_leftCooldown = 0.0f;
  float canDash_leftCooldown = 0.0f;
  bool canDash = false; // must be on the ground to reload dash
  int32_t health = 3;

  void reset()
  {
    position = { 0.0f, 0.0f };
    velocity = { 0.0f, 0.0f };
    facingDir = { 1.0f, 0.0f };
    isAttacking_animLockedCooldown = 0.0f;
    isDashing_animLockedCooldown = 0.0f;
    isHurt_animLockedCooldown = 0.0f;
    isOnTheGround = false;
    isInvincible_leftCooldown = 0.0f;
    canAttack_leftCooldown = 0.0f;
    canDash_leftCooldown = 0.0f;
    canDash = false;
    health = 3;
  }

  bool isAnimLocked() const
  {
    return (
      isAttacking_animLockedCooldown > 0.0f ||
      isDashing_animLockedCooldown > 0.0f ||
      isHurt_animLockedCooldown > 0.0f
    );
  }

  void updateCooldowns(float deltaTimeSec)
  {
    isAttacking_animLockedCooldown = std::max(isAttacking_animLockedCooldown - deltaTimeSec, 0.0f);
    isDashing_animLockedCooldown = std::max(isDashing_animLockedCooldown - deltaTimeSec, 0.0f);
    isHurt_animLockedCooldown = std::max(isHurt_animLockedCooldown - deltaTimeSec, 0.0f);
    isInvincible_leftCooldown = std::max(isInvincible_leftCooldown - deltaTimeSec, 0.0f);
    canAttack_leftCooldown = std::max(canAttack_leftCooldown - deltaTimeSec, 0.0f);
    canDash_leftCooldown = std::max(canDash_leftCooldown - deltaTimeSec, 0.0f);
  }
};

// MARK: PlatformerCharacterControls
struct PlatformerCharacterControls
{
  bool isMovingLeft = false;
  bool isMovingRight = false;
  bool isLookingUp = false;
  bool isLookingDown = false;
  bool isJumping = false;
  bool isAttacking = false;
  bool isDashing = false;

  void moveLeft() { isMovingLeft = true; }
  void moveRight() { isMovingRight = true; }
  void lookUp() { isLookingUp = true; }
  void lookDown() { isLookingDown = true; }
  void jump() { isJumping = true; }
  void attack() { isAttacking = true; }
  void dash() { isDashing = true; }
  void reset()
  {
    isMovingLeft = false;
    isMovingRight = false;
    isLookingUp = false;
    isLookingDown = false;
    isJumping = false;
    isAttacking = false;
    isDashing = false;
  }

};

// MARK: PlatformerCharacter
class PlatformerCharacter
{
public:
  PlatformerCharacter();

public:
  void update(float deltaTimeSec);
  void handleCollisions(PlatformerCharacter& other, KeyframesManager& keyframesManager);

  PlatformerCharacterStates simulate(
    float deltaTimeSec,
    const PlatformerCharacter& other,
    KeyframesManager& keyframesManager,
    const PlatformerCharacterControls& inControls
  ) const;

public:
  void renderDebug(gero::graphics::opengl::advanced::IStackRenderers& stackRenderers, const glm::vec3& inOrigin) const;
  void renderAnimation(IGraphicSceneRenderer& scene, KeyframesManager& keyframesManager, const glm::vec3& inOrigin, bool debugMode) const;

private:
  void _getCurrentAnimationAlias(std::string& out_alias, bool& out_flipX) const;

public:
  PlatformerCharacterStates _states;
  PlatformerCharacterControls _controls;

  glm::vec3 _color;

private:
  float _elapsedTime = 0.0f;

};
