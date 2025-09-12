
build_platform=native
# build_platform=web-wasm

build_mode=release
# build_mode=debug

#

ifeq ($(build_platform),native)

# $(info build_platform is valid, value=$(build_platform))

else ifeq ($(build_platform),web-wasm)

# $(info build_platform is valid, value=$(build_platform))

else

$(error unsupported value for "build_platform", value=$(build_platform))

endif

LOG_INFO= '[$(build_mode)] [$(build_platform)]'

#

ifeq ($(build_platform),native)

DIR_LIB=		./lib/native

NAME_BULLET_PHYSICS_LINEARMATH=	$(DIR_LIB)/lib-bullet-physics-linearmath.a
NAME_BULLET_PHYSICS_COLLISION=	$(DIR_LIB)/lib-bullet-physics-collision.a
NAME_BULLET_PHYSICS_DYNAMICS=		$(DIR_LIB)/lib-bullet-physics-dynamics.a

else ifeq ($(build_platform),web-wasm)

DIR_LIB=		./lib/web-wasm

NAME_BULLET_PHYSICS_LINEARMATH=	$(DIR_LIB)/lib-bullet-physics-linearmath.a
NAME_BULLET_PHYSICS_COLLISION=	$(DIR_LIB)/lib-bullet-physics-collision.a
NAME_BULLET_PHYSICS_DYNAMICS=		$(DIR_LIB)/lib-bullet-physics-dynamics.a

endif



#### DIRS

DIR_DEPENDENCIES=					./dependencies
DIR_BULLET_PHYSICS_SRC=		$(DIR_DEPENDENCIES)/bullet3/src

#### /DIRS



ifeq ($(build_platform),native)

DIR_OBJ=			./obj/native

else ifeq ($(build_platform),web-wasm)

DIR_OBJ=			./obj/web-wasm

endif

DIR_OBJ_GERONIMO=					$(DIR_OBJ)/geronimo
DIR_OBJ_BULLET_PHYSICS=		$(DIR_OBJ)/bullet-physics


#### SRC

SRC_BULLET_PHYSICS_LINEARMATH+= \
	$(wildcard \
		$(DIR_BULLET_PHYSICS_SRC)/LinearMath/*.cpp \
		)

SRC_BULLET_PHYSICS_COLLISION+= \
	$(wildcard \
		$(DIR_BULLET_PHYSICS_SRC)/BulletCollision/BroadphaseCollision/*.cpp \
		$(DIR_BULLET_PHYSICS_SRC)/BulletCollision/CollisionShapes/*.cpp \
		$(DIR_BULLET_PHYSICS_SRC)/BulletCollision/CollisionDispatch/*.cpp \
		$(DIR_BULLET_PHYSICS_SRC)/BulletCollision/NarrowPhaseCollision/*.cpp \
		)

# TODO: this will be needed for non-static mesh collider support
# SRC_BULLET_PHYSICS_COLLISION+= $(wildcard $(DIR_BULLET_PHYSICS_SRC)/BulletCollision/Gimpact/*.cpp)

SRC_BULLET_PHYSICS_DYNAMICS+= \
	$(wildcard \
		$(DIR_BULLET_PHYSICS_SRC)/BulletDynamics/Dynamics/*.cpp \
		$(DIR_BULLET_PHYSICS_SRC)/BulletDynamics/Vehicle/*.cpp \
		$(DIR_BULLET_PHYSICS_SRC)/BulletDynamics/ConstraintSolver/*.cpp) # TODO: useful?

#

OBJ_BULLET_PHYSICS_LINEARMATH=	$(patsubst %.cpp, $(DIR_OBJ_BULLET_PHYSICS)/%.o, $(SRC_BULLET_PHYSICS_LINEARMATH))
OBJ_BULLET_PHYSICS_COLLISION=		$(patsubst %.cpp, $(DIR_OBJ_BULLET_PHYSICS)/%.o, $(SRC_BULLET_PHYSICS_COLLISION))
OBJ_BULLET_PHYSICS_DYNAMICS=		$(patsubst %.cpp, $(DIR_OBJ_BULLET_PHYSICS)/%.o, $(SRC_BULLET_PHYSICS_DYNAMICS))

#

#######


ifeq ($(build_mode),release)

BUILD_FLAG= -O3

else

BUILD_FLAG= -g3

endif


CXXFLAGS_COMMON_BULLET_PHYSICS += -DBT_NO_PROFILE
CXXFLAGS_COMMON_BULLET_PHYSICS += -DSIMD_FORCE_INLINE=inline
# CXXFLAGS_COMMON_BULLET_PHYSICS += -DBT_USE_DOUBLE_PRECISION


CXXFLAGS_BULLET_PHYSICS += $(BUILD_FLAG)
CXXFLAGS_BULLET_PHYSICS += -std=c++17
CXXFLAGS_BULLET_PHYSICS += -I$(DIR_BULLET_PHYSICS_SRC)
CXXFLAGS_BULLET_PHYSICS += $(CXXFLAGS_COMMON_BULLET_PHYSICS)

ifeq ($(build_platform),native)

CXX=clang++
AR=ar

else ifeq ($(build_platform),web-wasm)

CXX=em++
AR=emar

endif


RM=			rm -rf


#######

#
## RULE(S)

all:	\
	bullet_physics

ensure_folders:
	@mkdir -p $(DIR_LIB)

bullet_physics: \
	ensure_folders \
	bullet_physics_linearmath \
	bullet_physics_collision \
	bullet_physics_dynamics

bullet_physics_linearmath: ensure_folders $(OBJ_BULLET_PHYSICS_LINEARMATH)
	@echo ' ---> building $(LOG_INFO): "bullet linear math library"'
	@$(AR) cr $(NAME_BULLET_PHYSICS_LINEARMATH) $(OBJ_BULLET_PHYSICS_LINEARMATH)
	@echo '   --> built $(LOG_INFO): "bullet linear math library"'

bullet_physics_collision:	ensure_folders $(OBJ_BULLET_PHYSICS_COLLISION)
	@echo ' ---> building $(LOG_INFO): "bullet collision library"'
	@$(AR) cr $(NAME_BULLET_PHYSICS_COLLISION) $(OBJ_BULLET_PHYSICS_COLLISION)
	@echo '   --> built $(LOG_INFO): "bullet collision library"'

bullet_physics_dynamics:	ensure_folders $(OBJ_BULLET_PHYSICS_DYNAMICS)
	@echo ' ---> building $(LOG_INFO): "bullet dynamics library"'
	@$(AR) cr $(NAME_BULLET_PHYSICS_DYNAMICS) $(OBJ_BULLET_PHYSICS_DYNAMICS)
	@echo '   --> built $(LOG_INFO): "bullet dynamics library"'

#

# for every ".cpp" file
# => ensure the "obj" folder(s)
# => compile in a ".o" file

$(DIR_OBJ_BULLET_PHYSICS)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo ' --> processing $(LOG_INFO):' $<
	@$(CXX) $(CXXFLAGS_BULLET_PHYSICS) $< -c -o $@

#

clean:
	@echo ' -> cleaning $(LOG_INFO): bullet physics library build file(s)'
	@$(RM) $(DIR_OBJ_BULLET_PHYSICS)
	@echo '   -> cleaned $(LOG_INFO): bullet physics library build file(s)'

fclean: clean
	@echo ' -> cleaning $(LOG_INFO): bullet physics library file(s)'
	@$(RM) $(NAME_BULLET_PHYSICS_LINEARMATH)
	@$(RM) $(NAME_BULLET_PHYSICS_COLLISION)
	@$(RM) $(NAME_BULLET_PHYSICS_DYNAMICS)
	@echo '   -> cleaned $(LOG_INFO): bullet physics library file(s)'

re:			fclean all

.PHONY:	\
		all \
		bullet_physics \
		bullet_physics_linearmath \
		bullet_physics_collision \
		bullet_physics_dynamics \
		clean \
		fclean \
		re

## RULE(S)
#
