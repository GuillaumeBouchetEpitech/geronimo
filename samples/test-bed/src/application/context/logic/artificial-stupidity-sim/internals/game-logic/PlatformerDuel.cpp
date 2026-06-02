
#include "PlatformerDuel.hpp"


#include "application/context/Context.hpp"

#include "geronimo/graphics/input-managers/KeyboardManager.hpp"
#include "geronimo/graphics/camera/sceneToScreen.hpp"

#include <sstream>
#include <iomanip>

namespace /*anonymous*/ {

  constexpr float k_duelRestartTime = 1.0f;
  constexpr float k_logicFps = 60.0f; // 60fps
  constexpr float k_frameStep = 1.0f / k_logicFps; // frame step in seconds

};

// MARK:ctor
PlatformerDuel::PlatformerDuel(KeyframesManager& inKeyframesManager)
  : _keyframesManager(inKeyframesManager)
{

  this->_reset();

  this->_transitionTimeLeft = k_duelRestartTime;

}

// MARK:update
void PlatformerDuel::update(float deltaTimeSec) {

  //
  // handle the transition logic
  //

  if (this->_transitionTimeLeft > 0.0f) {
    this->_transitionTimeLeft -= deltaTimeSec;
    if (this->_transitionTimeLeft < 0.0f) {
      this->_transitionTimeLeft = 0.0f;

      this->_reset();
    }

    // in transition -> no update
    return;
  }

  //
  // handle the frame skip logic
  //

  if (this->_timeLeftUntilNextUpdate > 0.0f) {
    this->_timeLeftUntilNextUpdate -= deltaTimeSec;

    if (this->_timeLeftUntilNextUpdate <= 0.0f) {
      this->_timeLeftUntilNextUpdate = k_frameStep;
    } else {
      return;
    }
  }

  //
  // handle the game over logic
  //

  bool gameOver = false;

  if (
    this->platformerCharacterPlayer._states.health <= 0 ||
    this->platformerCharacterBot._states.health <= 0
  ) {
    // at least one player is dead -> game over
    gameOver = true;
  }
  else if (this->_totalFramesLeft > 0) {
    this->_totalFramesLeft -= 1;

    if (this->_totalFramesLeft == 0) {
      // out of frames for this duel -> game over
      gameOver = true;

      // forcefully kill the player
      // -> for the leaderboard logic
      this->platformerCharacterPlayer._states.health = 0;
    }
  }

  if (gameOver) {
    // initiate the transition
    this->_transitionTimeLeft = k_duelRestartTime;

    this->_totalDuels += 1;

    const bool redWonTheDuel = this->platformerCharacterPlayer._states.health > 0;
    this->_discriminatorController.learnFromSavedFrames(redWonTheDuel);

    // stop the update now
    return;
  }

  //
  // handle the player input logic
  //

  {
    auto& keyboard = KeyboardManager::get();

    const bool arrowLeft = keyboard.isPressed(SDLK_LEFT);
    const bool arrowRight = keyboard.isPressed(SDLK_RIGHT);
    const bool arrowUp = keyboard.isPressed(SDLK_UP);
    const bool arrowDown = keyboard.isPressed(SDLK_DOWN);
    const bool keyZ = keyboard.isPressed(SDLK_z);
    const bool keyW = keyboard.isPressed(SDLK_w); // azerty
    const bool keyX = keyboard.isPressed(SDLK_x);
    const bool keyC = keyboard.isPressed(SDLK_c);

    auto& redControls =  this->platformerCharacterPlayer._controls;

    redControls.reset();

    if (arrowLeft) { redControls.moveLeft(); }
    if (arrowRight) { redControls.moveRight(); }
    if (arrowUp) { redControls.lookUp(); }
    if (arrowDown) { redControls.lookDown(); }
    if (keyZ || keyW) { redControls.jump(); }
    if (keyX) { redControls.attack(); }
    if (keyC) { redControls.dash(); }
  }

  {
    // // debug
    // this->platformerCharacterBot._controls.lookUp();
    // this->platformerCharacterBot._controls.attack();
  }

  //
  // handle the machine learning logic
  //

  this->_handleMachineLearningLogic();

  //
  // handle the characters update logic
  //

  {
    // float currFramerate = 1.0f / deltaTimeSec;
    // D_MYLOG("currFramerate " << currFramerate);

    this->platformerCharacterPlayer.update(k_frameStep);
    this->platformerCharacterBot.update(k_frameStep);

    this->platformerCharacterPlayer.handleCollisions(this->platformerCharacterBot, this->_keyframesManager);
  }

  if (this->_totalDuels > 0) {
    this->_handleMachineLearningLogic_part2();
  }

}

// MARK:renderScene
void PlatformerDuel::renderScene()
{
  const glm::vec3 k_origin = glm::vec3(5,50,0);

  auto& context = Context::get();

  auto& sceneRenderer = context.graphic.renderer.getSceneRenderer();
  auto& stackRenderer = sceneRenderer.getStackRenderers();

  auto& wireFramesStack = stackRenderer.getWireFramesStack();
  auto& trianglesStack = stackRenderer.getTrianglesStack();

  {
    // wireframe arena limits

    // ground
    wireFramesStack.pushLine(
      k_origin + glm::vec3(25,25,0) + glm::vec3(-20,0,0),
      k_origin + glm::vec3(25,25,0) + glm::vec3(+20,0,0),
      glm::vec3(1,1,1)
    );
    // left
    wireFramesStack.pushLine(
      k_origin + glm::vec3(25,25,0) + glm::vec3(-20,0,0),
      k_origin + glm::vec3(25,25,0) + glm::vec3(-20,10,0),
      glm::vec3(1,1,1)
    );
    // right
    wireFramesStack.pushLine(
      k_origin + glm::vec3(25,25,0) + glm::vec3(+20,0,0),
      k_origin + glm::vec3(25,25,0) + glm::vec3(+20,10,0),
      glm::vec3(1,1,1)
    );
  }


  {
    // background
    const glm::vec2 k_size = glm::vec2(50,15);

    // color change during transition
    const float coeff = 1.0f - this->_transitionTimeLeft / k_duelRestartTime; // 0->1
    const float rgbValues = 0.1f + coeff * 0.3f;
    const glm::vec4 tmpColor = glm::vec4(rgbValues,rgbValues,rgbValues, 1);

    trianglesStack.pushQuad(glm::vec2(k_origin + glm::vec3(25,25,0)) + glm::vec2(0,5.0f), k_size, tmpColor, k_origin.z - 1.0f);
  }


  constexpr bool k_debugMode = false;

  if (k_debugMode)
  {
    this->platformerCharacterPlayer.renderDebug(stackRenderer, k_origin + glm::vec3(25,25,0));
    this->platformerCharacterBot.renderDebug(stackRenderer, k_origin + glm::vec3(25,25,0));
  }

  this->platformerCharacterPlayer._color = glm::vec3(1.0f,0.80f,0.80f);
  this->platformerCharacterBot._color = glm::vec3(0.80f,0.80f,1.0f);
  this->platformerCharacterPlayer.renderAnimation(sceneRenderer, this->_keyframesManager, k_origin + glm::vec3(25,25,0), k_debugMode);
  this->platformerCharacterBot.renderAnimation(sceneRenderer, this->_keyframesManager, k_origin + glm::vec3(25,25,0), k_debugMode);
}

// MARK:renderHUD
void PlatformerDuel::renderHUD()
{

  using namespace gero::graphics::opengl;
  using namespace gero::graphics::opengl::GlContext;

  auto& context = Context::get();
  auto& renderer = context.graphic.renderer;
  auto& hud = renderer.getHudRenderer();

  const glm::vec2& screenSize = context.graphic.renderer.getHudRenderer().getCamera().getSize();

  const float timeLeft = this->_totalFramesLeft / 60.0f;

  {
    const glm::vec3 k_origin = glm::vec3(5,50,0);
    const glm::vec3 botPos = k_origin + glm::vec3(25,25,0) + glm::vec3(this->platformerCharacterBot._states.position, 0);

    const auto& sceneCamera = renderer.getSceneRenderer().getCamera();
    auto& matricesData = sceneCamera.getMatricesData();

    glm::vec3 botScreenCoord = glm::vec3(0, 0, 1);

    gero::graphics::sceneToScreen(botPos,
                                  matricesData.view,
                                  matricesData.projection,
                                  glm::vec2(0, 0),
                                  sceneCamera.getSize(),
                                  botScreenCoord);

    // botScreenCoord

    {

      using namespace gero::graphics::opengl;
      using namespace gero::graphics::opengl::GlContext;

      auto& context = Context::get();
      auto& renderer = context.graphic.renderer;
      auto& hud = renderer.getHudRenderer();

      constexpr float k_textDepth = -0.5f;
      constexpr float k_scale = 16.0f;

      hud.getTextRenderer().clear();

      hud.getTextRenderer().setMainColor(glm::vec4(1, 1, 1, 1))
        .setOutlineColor(glm::vec4(0.3f, 0.3f, 0.3f, 1))
        .setScale(k_scale)
        .setDepth(k_textDepth)
        // .setVerticalTextAlign(TextRenderer::VerticalTextAlign::top)
        // .setHorizontalTextAlign(TextRenderer::HorizontalTextAlign::left)
        .setVerticalTextAlign(advanced::TextRenderer::VerticalTextAlign::center)
        .setHorizontalTextAlign(advanced::TextRenderer::HorizontalTextAlign::center)
        ;

      std::stringstream sstr;
      sstr << std::fixed << std::setprecision(2) << this->_actorCriticController.averageError();
      sstr << std::endl;
      sstr << std::fixed << std::setprecision(3) << this->_stateRating;

      std::string str = sstr.str();

      hud.getTextRenderer().pushText(glm::vec2(botScreenCoord), str);
      hud.getTextRenderer().render();
    }

  }

  {
    constexpr float k_textDepth = -0.5f;
    constexpr float k_scale = 32.0f;

    hud.getTextRenderer().setMainColor(glm::vec4(1, 1, 1, 1))
      .setOutlineColor(glm::vec4(0.3f, 0.3f, 0.3f, 1))
      .setScale(k_scale)
      .setDepth(k_textDepth)
      // .setVerticalTextAlign(TextRenderer::VerticalTextAlign::top)
      // .setHorizontalTextAlign(TextRenderer::HorizontalTextAlign::left)
      .setVerticalTextAlign(advanced::TextRenderer::VerticalTextAlign::center)
      .setHorizontalTextAlign(advanced::TextRenderer::HorizontalTextAlign::center)
      ;

    std::stringstream sstr;
    if (timeLeft < 5) {
      sstr << std::fixed << std::setprecision(2) << timeLeft;
    } else {
      sstr << std::fixed << std::setprecision(0) << timeLeft;
    }
    std::string str = sstr.str();

    hud.getTextRenderer().pushText(glm::vec2(screenSize.x * 0.5f, 100.0f), str);
  }




}

// MARK:_reset
void PlatformerDuel::_reset()
{
  // reset duel
  this->platformerCharacterPlayer._states.reset();
  this->platformerCharacterBot._states.reset();
  this->platformerCharacterPlayer._controls.reset();
  this->platformerCharacterBot._controls.reset();
  this->platformerCharacterPlayer._states.position.x -= 10.0f; // moved to the left
  this->platformerCharacterBot._states.position.x += 10.0f; // moved to the right
  this->platformerCharacterBot._states.facingDir.x = -1.0f; // facing left
  this->platformerCharacterBot.update(0.0f); // sync the states
  this->platformerCharacterPlayer.update(0.0f); // sync the states

  this->_timeLeftUntilNextUpdate = k_frameStep;

  // X fps for 30 seconds
  this->_totalFramesLeft = uint32_t(k_logicFps) * 30;

  {
    ActorCriticInput::CharacterState selfState;
    ActorCriticInput::CharacterState opponentState;

    {
      selfState.pos = this->platformerCharacterBot._states.position;
      selfState.vel = this->platformerCharacterBot._states.velocity;
      selfState.onTheGround = this->platformerCharacterBot._states.isOnTheGround;
      selfState.health = this->platformerCharacterBot._states.health;
      selfState.dashCooldown = this->platformerCharacterBot._states.canDash_leftCooldown;
      selfState.invulnerableCooldown = this->platformerCharacterBot._states.isInvincible_leftCooldown;
      selfState.attackCooldown = this->platformerCharacterBot._states.canAttack_leftCooldown;
    }

    {
      opponentState.pos = this->platformerCharacterPlayer._states.position;
      opponentState.vel = this->platformerCharacterPlayer._states.velocity;
      opponentState.onTheGround = this->platformerCharacterPlayer._states.isOnTheGround;
      opponentState.health = this->platformerCharacterPlayer._states.health;
      opponentState.dashCooldown = this->platformerCharacterPlayer._states.canDash_leftCooldown;
      opponentState.invulnerableCooldown = this->platformerCharacterPlayer._states.isInvincible_leftCooldown;
      opponentState.attackCooldown = this->platformerCharacterPlayer._states.canAttack_leftCooldown;
    }

    this->_actorCriticController._actorCriticInput.set(selfState, opponentState, -1);
  }

  {
    this->_discriminatorController.clearCharacterFrames();
  }
}

// MARK:_handleMachineLearningLogic
void PlatformerDuel::_handleMachineLearningLogic()
{

  {
    DiscriminatorInput redInput;
    DiscriminatorInput blueInput;

    {
      redInput.selfState.pos = this->platformerCharacterPlayer._states.position;
      redInput.selfState.vel = this->platformerCharacterPlayer._states.velocity;
      redInput.selfState.onTheGround = this->platformerCharacterPlayer._states.isOnTheGround;
      redInput.selfState.health = this->platformerCharacterPlayer._states.health;
      redInput.selfState.dashCooldown = this->platformerCharacterPlayer._states.canDash_leftCooldown;
      redInput.selfState.invulnerableCooldown = this->platformerCharacterPlayer._states.isInvincible_leftCooldown;
      redInput.selfState.attackCooldown = this->platformerCharacterPlayer._states.canAttack_leftCooldown;

      // redInput.characterActions.dir.x
      redInput.selfActions.dir.x = 0.0f;
      if (this->platformerCharacterPlayer._controls.isMovingLeft) {
        redInput.selfActions.dir.x = -1.0f;
      } else if (this->platformerCharacterPlayer._controls.isMovingRight) {
        redInput.selfActions.dir.x = 1.0f;
      }

      // redInput.selfActions.dir.y
      redInput.selfActions.dir.y = 0.0f;
      if (this->platformerCharacterPlayer._controls.isLookingUp) {
        redInput.selfActions.dir.y = 1.0f;
      } else if (this->platformerCharacterPlayer._controls.isLookingDown) {
        redInput.selfActions.dir.y = -1.0f;
      }

      redInput.selfActions.jump = this->platformerCharacterPlayer._controls.isJumping;
      redInput.selfActions.dash = this->platformerCharacterPlayer._controls.isDashing;
      redInput.selfActions.attack = this->platformerCharacterPlayer._controls.isAttacking;




      redInput.opponentState.pos = this->platformerCharacterBot._states.position;
      redInput.opponentState.vel = this->platformerCharacterBot._states.velocity;
      redInput.opponentState.onTheGround = this->platformerCharacterBot._states.isOnTheGround;
      redInput.opponentState.health = this->platformerCharacterBot._states.health;
      redInput.opponentState.dashCooldown = this->platformerCharacterBot._states.canDash_leftCooldown;
      redInput.opponentState.invulnerableCooldown = this->platformerCharacterBot._states.isInvincible_leftCooldown;
      redInput.opponentState.attackCooldown = this->platformerCharacterBot._states.canAttack_leftCooldown;

      // redInput.selfActions.dir.x
      redInput.opponentActions.dir.x = 0.0f;
      if (this->platformerCharacterBot._controls.isMovingLeft) {
        redInput.opponentActions.dir.x = -1.0f;
      } else if (this->platformerCharacterBot._controls.isMovingRight) {
        redInput.opponentActions.dir.x = 1.0f;
      }

      // redInput.opponentActions.dir.y
      redInput.opponentActions.dir.y = 0.0f;
      if (this->platformerCharacterBot._controls.isLookingUp) {
        redInput.opponentActions.dir.y = 1.0f;
      } else if (this->platformerCharacterBot._controls.isLookingDown) {
        redInput.opponentActions.dir.y = -1.0f;
      }

      redInput.opponentActions.jump = this->platformerCharacterBot._controls.isJumping;
      redInput.opponentActions.dash = this->platformerCharacterBot._controls.isDashing;
      redInput.opponentActions.attack = this->platformerCharacterBot._controls.isAttacking;




      redInput.selfRelativeSpatialFeatures.deltaToOpponent = redInput.selfState.pos - redInput.opponentState.pos;
      redInput.selfRelativeSpatialFeatures.distanceToOpponent = glm::distance(redInput.selfState.pos, redInput.opponentState.pos);
      // redInput.selfRelativeSpatialFeatures.opponentFacingDirection = inOpponentFacingDirection;
      // redInput.selfRelativeSpatialFeatures.opponentIsAirborne = redInput.opponentState.pos.y > 0.0f;
    }

    {
      blueInput.selfState.pos = this->platformerCharacterBot._states.position;
      blueInput.selfState.vel = this->platformerCharacterBot._states.velocity;
      blueInput.selfState.onTheGround = this->platformerCharacterBot._states.isOnTheGround;
      blueInput.selfState.health = this->platformerCharacterBot._states.health;
      blueInput.selfState.dashCooldown = this->platformerCharacterBot._states.canDash_leftCooldown;
      blueInput.selfState.invulnerableCooldown = this->platformerCharacterBot._states.isInvincible_leftCooldown;
      blueInput.selfState.attackCooldown = this->platformerCharacterBot._states.canAttack_leftCooldown;

      // blueInput.selfActions.dir.x
      blueInput.selfActions.dir.x = 0.0f;
      if (this->platformerCharacterBot._controls.isMovingLeft) {
        blueInput.selfActions.dir.x = -1.0f;
      } else if (this->platformerCharacterBot._controls.isMovingRight) {
        blueInput.selfActions.dir.x = 1.0f;
      }

      // blueInput.selfActions.dir.y
      blueInput.selfActions.dir.y = 0.0f;
      if (this->platformerCharacterBot._controls.isLookingUp) {
        blueInput.selfActions.dir.y = 1.0f;
      } else if (this->platformerCharacterBot._controls.isLookingDown) {
        blueInput.selfActions.dir.y = -1.0f;
      }

      blueInput.selfActions.jump = this->platformerCharacterBot._controls.isJumping;
      blueInput.selfActions.dash = this->platformerCharacterBot._controls.isDashing;
      blueInput.selfActions.attack = this->platformerCharacterBot._controls.isAttacking;



      blueInput.selfState.pos = this->platformerCharacterPlayer._states.position;
      blueInput.selfState.vel = this->platformerCharacterPlayer._states.velocity;
      blueInput.selfState.onTheGround = this->platformerCharacterPlayer._states.isOnTheGround;
      blueInput.selfState.health = this->platformerCharacterPlayer._states.health;
      blueInput.selfState.dashCooldown = this->platformerCharacterPlayer._states.canDash_leftCooldown;
      blueInput.selfState.invulnerableCooldown = this->platformerCharacterPlayer._states.isInvincible_leftCooldown;
      blueInput.selfState.attackCooldown = this->platformerCharacterPlayer._states.canAttack_leftCooldown;

      // blueInput.characterActions.dir.x
      blueInput.selfActions.dir.x = 0.0f;
      if (this->platformerCharacterPlayer._controls.isMovingLeft) {
        blueInput.selfActions.dir.x = -1.0f;
      } else if (this->platformerCharacterPlayer._controls.isMovingRight) {
        blueInput.selfActions.dir.x = 1.0f;
      }

      // blueInput.selfActions.dir.y
      blueInput.selfActions.dir.y = 0.0f;
      if (this->platformerCharacterPlayer._controls.isLookingUp) {
        blueInput.selfActions.dir.y = 1.0f;
      } else if (this->platformerCharacterPlayer._controls.isLookingDown) {
        blueInput.selfActions.dir.y = -1.0f;
      }

      blueInput.selfActions.jump = this->platformerCharacterPlayer._controls.isJumping;
      blueInput.selfActions.dash = this->platformerCharacterPlayer._controls.isDashing;
      blueInput.selfActions.attack = this->platformerCharacterPlayer._controls.isAttacking;




      blueInput.selfRelativeSpatialFeatures.deltaToOpponent = blueInput.selfState.pos - blueInput.opponentState.pos;
      blueInput.selfRelativeSpatialFeatures.distanceToOpponent = glm::distance(blueInput.selfState.pos, blueInput.opponentState.pos);

    }

    this->_discriminatorController.pushCharacterFrame(redInput, blueInput);
  }

  // if (this->_totalDuels > 0)
  {

    // blue character -> look-ahead (actor-critic)
    // -> list all action possible
    // -> rate said actions with the actor-critic
    // blue character -> take action
    // -> the best rated action
    // discriminator -> rate action
    // -> from taken action + new state
    // actor-critic -> learn from discriminator output
    // -> new state

    std::vector<PlatformerCharacterControls> allActions;
    allActions.reserve(12);

    if (this->platformerCharacterBot._states.isAnimLocked() == false)
    {

      // basic movements
      {
        PlatformerCharacterControls tmpControls;
        tmpControls.reset();
        tmpControls.isMovingLeft = true;
        allActions.push_back(tmpControls);
      }

      {
        PlatformerCharacterControls tmpControls;
        tmpControls.reset();
        tmpControls.isMovingRight = true;
        allActions.push_back(tmpControls);
      }

      // jumping movements
      if (this->platformerCharacterBot._states.isOnTheGround)
      {
        {
          PlatformerCharacterControls tmpControls;
          tmpControls.reset();
          tmpControls.isJumping = true;
          allActions.push_back(tmpControls);
        }

        {
          PlatformerCharacterControls tmpControls;
          tmpControls.reset();
          tmpControls.isMovingLeft = true;
          tmpControls.isJumping = true;
          allActions.push_back(tmpControls);
        }

        {
          PlatformerCharacterControls tmpControls;
          tmpControls.reset();
          tmpControls.isMovingRight = true;
          tmpControls.isJumping = true;
          allActions.push_back(tmpControls);
        }
      }

      // skill: dashing

      {
        PlatformerCharacterControls tmpControls;
        tmpControls.reset();
        tmpControls.isMovingLeft = true;
        tmpControls.isDashing = true;
        allActions.push_back(tmpControls);
      }

      {
        PlatformerCharacterControls tmpControls;
        tmpControls.reset();
        tmpControls.isMovingRight = true;
        tmpControls.isDashing = true;
        allActions.push_back(tmpControls);
      }

      // skill: attack

      {
        PlatformerCharacterControls tmpControls;
        tmpControls.reset();
        tmpControls.isMovingLeft = true;
        tmpControls.isAttacking = true;
        allActions.push_back(tmpControls);
      }

      {
        PlatformerCharacterControls tmpControls;
        tmpControls.reset();
        tmpControls.isMovingRight = true;
        tmpControls.isAttacking = true;
        allActions.push_back(tmpControls);
      }

      // skill: upward attack
      {
        PlatformerCharacterControls tmpControls;
        tmpControls.reset();
        tmpControls.isLookingUp = true;
        tmpControls.isAttacking = true;
        allActions.push_back(tmpControls);
      }

      // not on the ground -> downward attack unlocked
      if (!this->platformerCharacterBot._states.isOnTheGround)
      {
        // skill: downward attack
        {
          PlatformerCharacterControls tmpControls;
          tmpControls.reset();
          tmpControls.isLookingDown = true;
          tmpControls.isAttacking = true;
          allActions.push_back(tmpControls);
        }
      }
    }

    //
    //
    //

    // PlatformerCharacterStates bestState = this->platformerCharacterBot._states;
    PlatformerCharacterControls bestControls;
    bestControls.reset();

    // default rating for doing nothing
    float bestRating = 0.1f;

    for (std::size_t ii = 0; ii < allActions.size(); ++ii) {
      const auto& currAction = allActions.at(ii);

      // play the action
      // get the new state
      // rate the new state


      // play the action
      PlatformerCharacterStates tmpNextSelfState = this->platformerCharacterBot.simulate(
        k_frameStep,
        this->platformerCharacterPlayer,
        this->_keyframesManager,
        currAction
      );


      // get the new state
      auto tmpNextInput = this->_actorCriticController._actorCriticInput.selfState;

      {
        tmpNextInput.pos = tmpNextSelfState.position;
        tmpNextInput.vel = tmpNextSelfState.velocity;
        tmpNextInput.onTheGround = tmpNextSelfState.isOnTheGround;
        tmpNextInput.health = tmpNextSelfState.health;
        tmpNextInput.dashCooldown = tmpNextSelfState.canDash_leftCooldown;
        tmpNextInput.invulnerableCooldown = tmpNextSelfState.isInvincible_leftCooldown;
        tmpNextInput.attackCooldown = tmpNextSelfState.canAttack_leftCooldown;
      }

      // rate the new state
      const float rating = this->_actorCriticController.rate(tmpNextInput);

      if (bestRating < rating) {

        // D_MYLOG("rating -> " << rating);

        bestRating = rating;
        // bestState = tmpNextSelfState;
        bestControls = currAction;
      }

    }

    // set the character controls with the best rated action
    this->platformerCharacterBot._controls = bestControls;

    // {

    //   DiscriminatorInput tmpInput;

    //   {
    //     tmpInput.characterState.pos = bestState.position;
    //     tmpInput.characterState.vel = bestState.velocity;
    //     tmpInput.characterState.onTheGround = bestState.isOnTheGround;
    //     tmpInput.characterState.health = bestState.health;
    //     tmpInput.characterState.dashCooldown = bestState.canDash_leftCooldown;
    //     tmpInput.characterState.invulnerableCooldown = bestState.isInvincible_leftCooldown;
    //     tmpInput.characterState.attackCooldown = bestState.canAttack_leftCooldown;

    //     // tmpInput.characterActions.dir.x
    //     tmpInput.characterActions.dir.x = 0.0f;
    //     if (bestControls.isMovingLeft) {
    //       tmpInput.characterActions.dir.x = -1.0f;
    //     } else if (bestControls.isMovingRight) {
    //       tmpInput.characterActions.dir.x = 1.0f;
    //     }

    //     // tmpInput.characterActions.dir.y
    //     tmpInput.characterActions.dir.y = 0.0f;
    //     if (bestControls.isLookingUp) {
    //       tmpInput.characterActions.dir.y = 1.0f;
    //     } else if (bestControls.isLookingDown) {
    //       tmpInput.characterActions.dir.y = -1.0f;
    //     }

    //     tmpInput.characterActions.jump = bestControls.isJumping;
    //     tmpInput.characterActions.dash = bestControls.isDashing;
    //     tmpInput.characterActions.attack = bestControls.isAttacking;
    //   }

    //   // rate the new/future state
    //   const float stateRating = this->_discriminatorController.rate(tmpInput);

    //   // teach the actor/critic from it
    //   // this->_actorCriticController._actorCriticInput.selfState = bestState;

    //   ActorCriticInput::CharacterState tmpSelfState;
    //   tmpSelfState.pos = bestState.position;
    //   tmpSelfState.vel = bestState.velocity;
    //   tmpSelfState.onTheGround = bestState.isOnTheGround;
    //   tmpSelfState.health = bestState.health;
    //   tmpSelfState.dashCooldown = bestState.canDash_leftCooldown;
    //   tmpSelfState.invulnerableCooldown = bestState.isInvincible_leftCooldown;
    //   tmpSelfState.attackCooldown = bestState.canAttack_leftCooldown;

    //   ActorCriticInput::CharacterState tmpOpponentState;
    //   tmpOpponentState.pos = this->platformerCharacterPlayer._states.position;
    //   tmpOpponentState.vel = this->platformerCharacterPlayer._states.velocity;
    //   tmpOpponentState.onTheGround = this->platformerCharacterPlayer._states.isOnTheGround;
    //   tmpOpponentState.health = this->platformerCharacterPlayer._states.health;
    //   tmpOpponentState.dashCooldown = this->platformerCharacterPlayer._states.canDash_leftCooldown;
    //   tmpOpponentState.invulnerableCooldown = this->platformerCharacterPlayer._states.isInvincible_leftCooldown;
    //   tmpOpponentState.attackCooldown = this->platformerCharacterPlayer._states.canAttack_leftCooldown;

    //   this->_actorCriticController._actorCriticInput.update(
    //     tmpSelfState,
    //     tmpOpponentState,
    //     this->platformerCharacterPlayer._states.facingDir.x
    //   );
    //   this->_actorCriticController.learn(stateRating);

    // }

  }

}

void PlatformerDuel::_handleMachineLearningLogic_part2()
{

  {

    DiscriminatorInput tmpInput;

    {
      tmpInput.selfState.pos = this->platformerCharacterBot._states.position;
      tmpInput.selfState.vel = this->platformerCharacterBot._states.velocity;
      tmpInput.selfState.onTheGround = this->platformerCharacterBot._states.isOnTheGround;
      tmpInput.selfState.health = this->platformerCharacterBot._states.health;
      tmpInput.selfState.dashCooldown = this->platformerCharacterBot._states.canDash_leftCooldown;
      tmpInput.selfState.invulnerableCooldown = this->platformerCharacterBot._states.isInvincible_leftCooldown;
      tmpInput.selfState.attackCooldown = this->platformerCharacterBot._states.canAttack_leftCooldown;

      // tmpInput.selfActions.dir.x
      tmpInput.selfActions.dir.x = 0.0f;
      if (this->platformerCharacterBot._controls.isMovingLeft) {
        tmpInput.selfActions.dir.x = -1.0f;
      } else if (this->platformerCharacterBot._controls.isMovingRight) {
        tmpInput.selfActions.dir.x = 1.0f;
      }

      // tmpInput.selfActions.dir.y
      tmpInput.selfActions.dir.y = 0.0f;
      if (this->platformerCharacterBot._controls.isLookingUp) {
        tmpInput.selfActions.dir.y = 1.0f;
      } else if (this->platformerCharacterBot._controls.isLookingDown) {
        tmpInput.selfActions.dir.y = -1.0f;
      }

      tmpInput.selfActions.jump = this->platformerCharacterBot._controls.isJumping;
      tmpInput.selfActions.dash = this->platformerCharacterBot._controls.isDashing;
      tmpInput.selfActions.attack = this->platformerCharacterBot._controls.isAttacking;




      tmpInput.opponentState.pos = this->platformerCharacterBot._states.position;
      tmpInput.opponentState.vel = this->platformerCharacterBot._states.velocity;
      tmpInput.opponentState.onTheGround = this->platformerCharacterBot._states.isOnTheGround;
      tmpInput.opponentState.health = this->platformerCharacterBot._states.health;
      tmpInput.opponentState.dashCooldown = this->platformerCharacterBot._states.canDash_leftCooldown;
      tmpInput.opponentState.invulnerableCooldown = this->platformerCharacterBot._states.isInvincible_leftCooldown;
      tmpInput.opponentState.attackCooldown = this->platformerCharacterBot._states.canAttack_leftCooldown;

      // tmpInput.selfActions.dir.x
      tmpInput.opponentActions.dir.x = 0.0f;
      if (this->platformerCharacterBot._controls.isMovingLeft) {
        tmpInput.opponentActions.dir.x = -1.0f;
      } else if (this->platformerCharacterBot._controls.isMovingRight) {
        tmpInput.opponentActions.dir.x = 1.0f;
      }

      // tmpInput.opponentActions.dir.y
      tmpInput.opponentActions.dir.y = 0.0f;
      if (this->platformerCharacterBot._controls.isLookingUp) {
        tmpInput.opponentActions.dir.y = 1.0f;
      } else if (this->platformerCharacterBot._controls.isLookingDown) {
        tmpInput.opponentActions.dir.y = -1.0f;
      }

      tmpInput.opponentActions.jump = this->platformerCharacterBot._controls.isJumping;
      tmpInput.opponentActions.dash = this->platformerCharacterBot._controls.isDashing;
      tmpInput.opponentActions.attack = this->platformerCharacterBot._controls.isAttacking;




      tmpInput.selfRelativeSpatialFeatures.deltaToOpponent = tmpInput.selfState.pos - tmpInput.opponentState.pos;
      tmpInput.selfRelativeSpatialFeatures.distanceToOpponent = glm::distance(tmpInput.selfState.pos, tmpInput.opponentState.pos);
    }

    // rate the new/future state
    const float stateRating = this->_discriminatorController.rate(tmpInput);

    this->_stateRating = stateRating;

    // teach the actor/critic from it
    // this->_actorCriticController._actorCriticInput.selfState = bestState;

    ActorCriticInput::CharacterState tmpSelfState;
    tmpSelfState.pos = this->platformerCharacterBot._states.position;
    tmpSelfState.vel = this->platformerCharacterBot._states.velocity;
    tmpSelfState.onTheGround = this->platformerCharacterBot._states.isOnTheGround;
    tmpSelfState.health = this->platformerCharacterBot._states.health;
    tmpSelfState.dashCooldown = this->platformerCharacterBot._states.canDash_leftCooldown;
    tmpSelfState.invulnerableCooldown = this->platformerCharacterBot._states.isInvincible_leftCooldown;
    tmpSelfState.attackCooldown = this->platformerCharacterBot._states.canAttack_leftCooldown;

    ActorCriticInput::CharacterState tmpOpponentState;
    tmpOpponentState.pos = this->platformerCharacterPlayer._states.position;
    tmpOpponentState.vel = this->platformerCharacterPlayer._states.velocity;
    tmpOpponentState.onTheGround = this->platformerCharacterPlayer._states.isOnTheGround;
    tmpOpponentState.health = this->platformerCharacterPlayer._states.health;
    tmpOpponentState.dashCooldown = this->platformerCharacterPlayer._states.canDash_leftCooldown;
    tmpOpponentState.invulnerableCooldown = this->platformerCharacterPlayer._states.isInvincible_leftCooldown;
    tmpOpponentState.attackCooldown = this->platformerCharacterPlayer._states.canAttack_leftCooldown;

    this->_actorCriticController._actorCriticInput.update(
      tmpSelfState,
      tmpOpponentState,
      this->platformerCharacterPlayer._states.facingDir.x
    );
    this->_actorCriticController.learn(stateRating);

  }

}
