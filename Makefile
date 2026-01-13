
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

ifeq ($(build_mode),release)
# $(info build_mode is valid, value=$(build_mode))
else ifeq ($(build_mode),debug)
# $(info build_mode is valid, value=$(build_mode))
else
$(error unsupported value for "build_mode", value=$(build_mode))
endif

LOG_INFO= '[${build_platform}] [${build_mode}]'

#

DIR_LIB=		./lib/geronimo-$(build_platform)-${build_mode}

NAME_GERONIMO_SYSTEM=						$(DIR_LIB)/lib-geronimo-system.a
NAME_GERONIMO_GRAPHICS=					$(DIR_LIB)/lib-geronimo-graphics.a
NAME_GERONIMO_PHYSICS=					$(DIR_LIB)/lib-geronimo-physics.a
NAME_GERONIMO_AUDIO=						$(DIR_LIB)/lib-geronimo-audio.a



#### DIRS

DIR_SRC=									./src
DIR_THIRDPARTY=						./thirdparties
DIR_BULLET_PHYSICS_SRC=		$(DIR_THIRDPARTY)/dependencies/bullet3/src

#### /DIRS




DIR_OBJ=			./obj/geronimo-$(build_platform)-${build_mode}


#### SRC

# search in $(DIR_SRC)/geronimo/system/
# exclude the pattern "*.tests.cpp"  (unit tests files)
# include the pattern "*.cpp"        (source files)
SRC_GERONIMO_SYSTEM += $(shell find $(DIR_SRC)/geronimo/system/ ! -name "*.tests.cpp" -name "*.cpp")

# search in $(DIR_SRC)/geronimo/graphics/
# exclude the pattern "*.tests.cpp"  (unit tests files)
# include the pattern "*.cpp"        (source files)
SRC_GERONIMO_GRAPHICS += $(shell find $(DIR_SRC)/geronimo/graphics/ ! -name "*.tests.cpp" -name "*.cpp")
# SRC_GERONIMO_GRAPHICS+=	\
# 	$(wildcard \
# 		$(DIR_SRC)/geronimo/graphics/*.cpp \
# 		$(DIR_SRC)/geronimo/graphics/camera/*.cpp \
# 		$(DIR_SRC)/geronimo/graphics/loaders/*.cpp \
# 		$(DIR_SRC)/geronimo/graphics/make-geometries/*.cpp \
# 		$(DIR_SRC)/geronimo/graphics/sdl/*.cpp \
# 		$(DIR_SRC)/geronimo/graphics/input-managers/*.cpp \
# 		$(DIR_SRC)/geronimo/graphics/vertexBuffers/*.cpp \
# 		$(DIR_SRC)/geronimo/graphics/wrappers/*.cpp \
# 		$(DIR_SRC)/geronimo/graphics/advanced-concept/clusteredDeferred/*.cpp \
# 		$(DIR_SRC)/geronimo/graphics/advanced-concept/clusteredDeferred/internals/*.cpp \
# 		$(DIR_SRC)/geronimo/graphics/advanced-concept/slowDeferred/*.cpp \
# 		$(DIR_SRC)/geronimo/graphics/advanced-concept/slowDeferred/internals/*.cpp \
# 		$(DIR_SRC)/geronimo/graphics/advanced-concept/textRenderer/*.cpp \
# 		$(DIR_SRC)/geronimo/graphics/advanced-concept/stackRenderers/*.cpp \
# 		$(DIR_SRC)/geronimo/graphics/advanced-concept/stackRenderers/internals/*.cpp \
# 		$(DIR_SRC)/geronimo/graphics/advanced-concept/widgets/*.cpp \
# 		$(DIR_SRC)/geronimo/graphics/advanced-concept/widgets/helpers/*.cpp \
		)

SRC_GERONIMO_PHYSICS += $(shell find $(DIR_SRC)/geronimo/physics/ ! -name "*.tests.cpp" -name "*.cpp")
# SRC_GERONIMO_PHYSICS+=	\
# 	$(wildcard \
# 		$(DIR_SRC)/geronimo/physics/*.cpp \
# 		$(DIR_SRC)/geronimo/physics/world/*.cpp \
# 		$(DIR_SRC)/geronimo/physics/body/*.cpp \
# 		$(DIR_SRC)/geronimo/physics/body/internals/*.cpp \
# 		$(DIR_SRC)/geronimo/physics/helpers/*.cpp \
# 		$(DIR_SRC)/geronimo/physics/queries/query-shape/*.cpp \
# 		$(DIR_SRC)/geronimo/physics/queries/ray-caster/*.cpp \
# 		$(DIR_SRC)/geronimo/physics/queries/ray-caster/internals/*.cpp \
# 		$(DIR_SRC)/geronimo/physics/shape/*.cpp \
# 		$(DIR_SRC)/geronimo/physics/vehicle/*.cpp \
# 		$(DIR_SRC)/geronimo/physics/vehicle/internals/*.cpp \
# 		$(DIR_SRC)/geronimo/physics/constraints/hinge/*.cpp \
# 		$(DIR_SRC)/geronimo/physics/constraints/hinge/internals/*.cpp \
# 		$(DIR_SRC)/geronimo/physics/constraints/six-dof/*.cpp \
# 		$(DIR_SRC)/geronimo/physics/constraints/six-dof/internals/*.cpp \
# 	)

# not ready (-_-)
# $(DIR_SRC)/geronimo/physics/constraints/universal/*.cpp \
# $(DIR_SRC)/geronimo/physics/constraints/universal/internals/*.cpp \
# $(DIR_SRC)/geronimo/physics/constraints/cone-twist/*.cpp \
# $(DIR_SRC)/geronimo/physics/constraints/cone-twist/internals/*.cpp \

SRC_GERONIMO_AUDIO += $(shell find $(DIR_SRC)/geronimo/audio/ ! -name "*.tests.cpp" -name "*.cpp")
# SRC_GERONIMO_AUDIO+=	\
# 	$(wildcard \
# 		$(DIR_SRC)/geronimo/audio/*.cpp \
# 		$(DIR_SRC)/geronimo/audio/decoders/*.cpp \
# 		)

#

OBJ_GERONIMO_SYSTEM=				$(patsubst %.cpp, $(DIR_OBJ)/system/%.o, $(SRC_GERONIMO_SYSTEM))
OBJ_GERONIMO_GRAPHICS=		$(patsubst %.cpp, $(DIR_OBJ)/graphics/%.o, $(SRC_GERONIMO_GRAPHICS))
OBJ_GERONIMO_PHYSICS=		$(patsubst %.cpp, $(DIR_OBJ)/physics/%.o, $(SRC_GERONIMO_PHYSICS))
OBJ_GERONIMO_AUDIO=			$(patsubst %.cpp, $(DIR_OBJ)/audio/%.o, $(SRC_GERONIMO_AUDIO))

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

COMMON_FLAGS += $(BUILD_FLAG)
COMMON_FLAGS += -std=c++20
COMMON_FLAGS += -Wall -W -Wextra -Wunused -Wpedantic -Wshadow -Wconversion -Werror
COMMON_FLAGS += -I$(DIR_SRC)
COMMON_FLAGS += -I$(DIR_THIRDPARTY)
COMMON_FLAGS += -I$(DIR_BULLET_PHYSICS_SRC)
COMMON_FLAGS += $(CXXFLAGS_COMMON_BULLET_PHYSICS)

ifeq ($(build_platform),native)

# CXX=clang++
CXX=g++
AR=ar

DEPENDENCIES_LIST += sdl2
DEPENDENCIES_CXXFLAGS += $(shell pkg-config $(DEPENDENCIES_LIST) --cflags)

CXXFLAGS_GERONIMO += $(COMMON_FLAGS)
CXXFLAGS_GERONIMO += $(DEPENDENCIES_CXXFLAGS)

else ifeq ($(build_platform),web-wasm)

CXX=em++
AR=emar

CXXFLAGS_GERONIMO += $(COMMON_FLAGS)
# CXXFLAGS_GERONIMO += $(DEPENDENCIES_CXXFLAGS)
CXXFLAGS_GERONIMO += -s USE_SDL=2
CXXFLAGS_GERONIMO += -s USE_PTHREADS=0

endif



CXXFLAGS_GERONIMO_PHYSICS += $(CXXFLAGS_GERONIMO)

# this disabled all warnings (fix a GImpact issue)
CXXFLAGS_GERONIMO_PHYSICS += -w



RM=			rm -rf


#######

#
## RULE(S)

all:	\
	geronimo

ensure_folders:
	@mkdir -p $(DIR_LIB)

geronimo:	\
	ensure_folders	\
	geronimo_system	\
	geronimo_graphics	\
	geronimo_physics	\
	geronimo_audio

geronimo_system:	ensure_folders $(OBJ_GERONIMO_SYSTEM)
	@echo ' ---> building $(LOG_INFO): "geronimo system library"'
	@$(AR) cr $(NAME_GERONIMO_SYSTEM) $(OBJ_GERONIMO_SYSTEM)
	@echo '   --> built $(LOG_INFO): "geronimo system library"'

geronimo_graphics:	ensure_folders $(OBJ_GERONIMO_GRAPHICS)
	@echo ' ---> building $(LOG_INFO): "geronimo graphic library"'
	@$(AR) cr $(NAME_GERONIMO_GRAPHICS) $(OBJ_GERONIMO_GRAPHICS)
	@echo '   --> built $(LOG_INFO): "geronimo graphic library"'

geronimo_physics:	ensure_folders $(OBJ_GERONIMO_PHYSICS)
	@echo ' ---> building $(LOG_INFO): "geronimo physic library"'
	@$(AR) cr $(NAME_GERONIMO_PHYSICS) $(OBJ_GERONIMO_PHYSICS)
	@echo '   --> built $(LOG_INFO): "geronimo physic library"'

geronimo_audio:	ensure_folders $(OBJ_GERONIMO_AUDIO)
	@echo ' ---> building $(LOG_INFO): "geronimo audio library"'
	@$(AR) cr $(NAME_GERONIMO_AUDIO) $(OBJ_GERONIMO_AUDIO)
	@echo '   --> built $(LOG_INFO): "geronimo audio library"'

#

# for every ".cpp" file
# => ensure the "obj" folder(s)
# => compile in a ".o" file

# $(DIR_OBJ)/%.o: %.cpp
# 	@mkdir -p $(dir $@)
# 	@echo ' --> processing $(LOG_INFO):' $<
# 	@$(CXX) -c $(CXXFLAGS_GERONIMO) -MMD -MT "$@" -MF "$@.dep" -o "$@" $<

# include $(shell test -d $(DIR_OBJ) && find $(DIR_OBJ) -name "*.dep")

$(DIR_OBJ)/system/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo ' --> system-processing $(LOG_INFO):' $<
	@$(CXX) -c $(CXXFLAGS_GERONIMO) -MMD -MT "$@" -MF "$@.dep" -o "$@" $<

include $(shell test -d $(DIR_OBJ)/system && find $(DIR_OBJ)/system -name "*.dep")

$(DIR_OBJ)/graphics/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo ' --> graphics-processing $(LOG_INFO):' $<
	@$(CXX) -c $(CXXFLAGS_GERONIMO) -MMD -MT "$@" -MF "$@.dep" -o "$@" $<

include $(shell test -d $(DIR_OBJ)/graphics && find $(DIR_OBJ)/graphics -name "*.dep")

$(DIR_OBJ)/physics/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo ' --> physics-processing $(LOG_INFO):' $<
	@$(CXX) -c $(CXXFLAGS_GERONIMO_PHYSICS) -MMD -MT "$@" -MF "$@.dep" -o "$@" $<

include $(shell test -d $(DIR_OBJ)/physics && find $(DIR_OBJ)/physics -name "*.dep")

$(DIR_OBJ)/audio/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo ' --> audio-processing $(LOG_INFO):' $<
	@$(CXX) -c $(CXXFLAGS_GERONIMO) -MMD -MT "$@" -MF "$@.dep" -o "$@" $<

include $(shell test -d $(DIR_OBJ)/audio && find $(DIR_OBJ)/audio -name "*.dep")

#

clean:
	@echo ' -> cleaning $(LOG_INFO): geronimo library build file(s)'
	@$(RM) $(DIR_OBJ)
	@echo '   -> cleaned $(LOG_INFO): geronimo library build file(s)'

fclean: clean
	@echo ' -> cleaning $(LOG_INFO): geronimo library file(s)'
	@$(RM) $(NAME_GERONIMO_SYSTEM)
	@$(RM) $(NAME_GERONIMO_GRAPHICS)
	@$(RM) $(NAME_GERONIMO_PHYSICS)
	@$(RM) $(NAME_GERONIMO_AUDIO)
	@echo '   -> cleaned $(LOG_INFO): geronimo library file(s)'

re:			fclean all

.PHONY:	\
		all \
		geronimo \
		clean \
		fclean \
		re

## RULE(S)
#
