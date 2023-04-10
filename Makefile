
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

NAME_GERONIMO_SYSTEM=						$(DIR_LIB)/lib-geronimo-system.a
NAME_GERONIMO_GRAPHICS=					$(DIR_LIB)/lib-geronimo-graphics.a
NAME_GERONIMO_PHYSICS=					$(DIR_LIB)/lib-geronimo-physics.a
NAME_GERONIMO_AUDIO=						$(DIR_LIB)/lib-geronimo-audio.a

else ifeq ($(build_platform),web-wasm)

DIR_LIB=		./lib/web-wasm

NAME_GERONIMO_SYSTEM=						$(DIR_LIB)/lib-geronimo-system.bc
NAME_GERONIMO_GRAPHICS=					$(DIR_LIB)/lib-geronimo-graphics.bc
NAME_GERONIMO_PHYSICS=					$(DIR_LIB)/lib-geronimo-physics.bc
NAME_GERONIMO_AUDIO=						$(DIR_LIB)/lib-geronimo-audio.bc

endif



#### DIRS

DIR_SRC=									./src
DIR_THIRDPARTY=						./thirdparties
DIR_BULLET_PHYSICS_SRC=		$(DIR_THIRDPARTY)/dependencies/bullet3/src

#### /DIRS



ifeq ($(build_platform),native)

DIR_OBJ=			./obj/native

else ifeq ($(build_platform),web-wasm)

DIR_OBJ=			./obj/web-wasm

endif

DIR_OBJ_GERONIMO=					$(DIR_OBJ)/geronimo


#### SRC

SRC_GERONIMO_SYSTEM+=	\
	$(wildcard \
		$(DIR_SRC)/geronimo/system/*.cpp \
		$(DIR_SRC)/geronimo/system/compression/*.cpp \
		$(DIR_SRC)/geronimo/system/containers/*.cpp \
		$(DIR_SRC)/geronimo/system/easing/*.cpp \
		$(DIR_SRC)/geronimo/system/file-utils/*.cpp \
		$(DIR_SRC)/geronimo/system/math/*.cpp \
		$(DIR_SRC)/geronimo/system/messaging/*.cpp \
		$(DIR_SRC)/geronimo/system/metrics/*.cpp \
		$(DIR_SRC)/geronimo/system/multithreading/*.cpp \
		$(DIR_SRC)/geronimo/system/multithreading/internals/*.cpp \
		$(DIR_SRC)/geronimo/system/parser-utils/*.cpp \
		$(DIR_SRC)/geronimo/system/rng/*.cpp \
		$(DIR_SRC)/geronimo/system/rng/proceduralGeneration/*.cpp \
		$(DIR_SRC)/geronimo/system/string-utils/*.cpp \
		)

SRC_GERONIMO_GRAPHICS+=	\
	$(wildcard \
		$(DIR_SRC)/geronimo/graphics/*.cpp \
		$(DIR_SRC)/geronimo/graphics/camera/*.cpp \
		$(DIR_SRC)/geronimo/graphics/loaders/*.cpp \
		$(DIR_SRC)/geronimo/graphics/make-geometries/*.cpp \
		$(DIR_SRC)/geronimo/graphics/sdl/*.cpp \
		$(DIR_SRC)/geronimo/graphics/vertexBuffers/*.cpp \
		$(DIR_SRC)/geronimo/graphics/wrappers/*.cpp \
		$(DIR_SRC)/geronimo/graphics/advanced-concept/clusteredDeferred/*.cpp \
		$(DIR_SRC)/geronimo/graphics/advanced-concept/clusteredDeferred/internals/*.cpp \
		$(DIR_SRC)/geronimo/graphics/advanced-concept/textRenderer/*.cpp \
		)

SRC_GERONIMO_PHYSICS+=	\
	$(wildcard \
		$(DIR_SRC)/geronimo/physics/*.cpp \
		$(DIR_SRC)/geronimo/physics/body/*.cpp \
		$(DIR_SRC)/geronimo/physics/helpers/*.cpp \
		$(DIR_SRC)/geronimo/physics/queries/*.cpp \
		$(DIR_SRC)/geronimo/physics/shape/*.cpp \
		$(DIR_SRC)/geronimo/physics/vehicle/*.cpp \
		)

SRC_GERONIMO_AUDIO+=	\
	$(wildcard \
		$(DIR_SRC)/geronimo/audio/*.cpp \
		$(DIR_SRC)/geronimo/audio/decoders/*.cpp \
		)

#

OBJ_GERONIMO_SYSTEM=		$(patsubst %.cpp, $(DIR_OBJ_GERONIMO)/%.o, $(SRC_GERONIMO_SYSTEM))
OBJ_GERONIMO_GRAPHICS=		$(patsubst %.cpp, $(DIR_OBJ_GERONIMO)/%.o, $(SRC_GERONIMO_GRAPHICS))
OBJ_GERONIMO_PHYSICS=		$(patsubst %.cpp, $(DIR_OBJ_GERONIMO)/%.o, $(SRC_GERONIMO_PHYSICS))
OBJ_GERONIMO_AUDIO=			$(patsubst %.cpp, $(DIR_OBJ_GERONIMO)/%.o, $(SRC_GERONIMO_AUDIO))

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
COMMON_FLAGS += -std=c++17
COMMON_FLAGS += -Wall -W -Wextra -Wunused -Wpedantic -Wshadow -Wconversion -Werror
COMMON_FLAGS += -I$(DIR_SRC)
COMMON_FLAGS += -I$(DIR_THIRDPARTY)
COMMON_FLAGS += -I$(DIR_BULLET_PHYSICS_SRC)
COMMON_FLAGS += $(CXXFLAGS_COMMON_BULLET_PHYSICS)

ifeq ($(build_platform),native)

CXX=clang++
AR=ar

DEPENDENCIES_LIST += sdl2
DEPENDENCIES_CXXFLAGS += $(shell pkg-config $(DEPENDENCIES_LIST) --cflags)

CXXFLAGS_GERONIMO += $(COMMON_FLAGS)
CXXFLAGS_GERONIMO += $(DEPENDENCIES_CXXFLAGS)

else ifeq ($(build_platform),web-wasm)

CXX=em++
AR=emar

CXXFLAGS_GERONIMO += $(COMMON_FLAGS)
CXXFLAGS_GERONIMO += -s USE_SDL=2
CXXFLAGS_GERONIMO += -s USE_PTHREADS=0

endif


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

$(DIR_OBJ_GERONIMO)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo ' --> processing $(LOG_INFO):' $<
	@$(CXX) -c $(CXXFLAGS_GERONIMO) -MMD -MT "$@" -MF "$@.dep" -o "$@" $<

include $(shell test -d $(DIR_OBJ_GERONIMO) && find $(DIR_OBJ_GERONIMO) -name "*.dep")

#

clean:
	@echo ' -> cleaning $(LOG_INFO): geronimo library build file(s)'
	@$(RM) $(DIR_OBJ_GERONIMO)
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
