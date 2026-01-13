
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

ifeq ($(build_platform),native)
# $(info build_platform is valid, value=$(build_platform))
else ifeq ($(build_platform),web-wasm)
# $(info build_platform is valid, value=$(build_platform))
else
$(error unsupported value for "build_platform", value=$(build_platform))
endif

LOG_INFO= '[$(build_platform)] [$(build_mode)]'

#

DIR_SRC=						./src
DIR_LIB_GERONIMO=		../..

ifeq ($(build_platform),native)

DIR_APPLICATION=		./bin
NAME_APPLICATION=		$(DIR_APPLICATION)/exec

else ifeq ($(build_platform),web-wasm)

DIR_APPLICATION=		./dist/wasm
NAME_APPLICATION=		$(DIR_APPLICATION)/index.js

endif



NAME_LIB_GERONIMO+=	\
	$(DIR_LIB_GERONIMO)/lib/geronimo-$(build_platform)-$(build_mode)/lib-geronimo-audio.a \
	$(DIR_LIB_GERONIMO)/lib/geronimo-$(build_platform)-$(build_mode)/lib-geronimo-physics.a \
	$(DIR_LIB_GERONIMO)/lib/geronimo-$(build_platform)-$(build_mode)/lib-geronimo-graphics.a \
	$(DIR_LIB_GERONIMO)/lib/geronimo-$(build_platform)-$(build_mode)/lib-geronimo-system.a

NAME_LIB_BULLET_PHYSICS+=	\
	$(DIR_LIB_GERONIMO)/lib/bullet-$(build_platform)-$(build_mode)/lib-bullet-physics-dynamics.a \
	$(DIR_LIB_GERONIMO)/lib/bullet-$(build_platform)-$(build_mode)/lib-bullet-physics-collision.a \
	$(DIR_LIB_GERONIMO)/lib/bullet-$(build_platform)-$(build_mode)/lib-bullet-physics-linearmath.a



#### DIRS

#### /DIRS



DIR_OBJ=	./obj/sample-$(build_platform)-$(build_mode)



#### SRC


# common src
SRC+= \
	$(wildcard \
		$(DIR_SRC)/*.cpp \
		$(DIR_SRC)/application/*.cpp \
		$(DIR_SRC)/application/context/*.cpp \
		$(DIR_SRC)/application/context/graphics/*.cpp \
		$(DIR_SRC)/application/context/graphics/renderers/*.cpp \
		$(DIR_SRC)/application/context/graphics/renderers/hud/*.cpp \
		$(DIR_SRC)/application/context/graphics/renderers/hud/helpers/*.cpp \
		$(DIR_SRC)/application/context/graphics/renderers/hud/widgets/*.cpp \
		$(DIR_SRC)/application/context/graphics/renderers/scene/*.cpp \
		$(DIR_SRC)/application/context/graphics/renderers/scene/helpers/*.cpp \
		$(DIR_SRC)/application/context/graphics/renderers/scene/geometries-stack-renderer/*.cpp \
		$(DIR_SRC)/application/context/graphics/renderers/scene/scene-stack-renderers/*.cpp \
		$(DIR_SRC)/application/context/logic/*.cpp \
		$(DIR_SRC)/application/context/logic/controllers/*.cpp \
		$(DIR_SRC)/application/context/logic/flocking/*.cpp \
		$(DIR_SRC)/application/context/logic/flocking/internals/*.cpp \
		$(DIR_SRC)/application/states/*.cpp \
	)


#

OBJ=		$(patsubst %.cpp, $(DIR_OBJ)/%.o, $(SRC))

#

#

#### /BULLET_SRC



#######


ifeq ($(build_mode),release)

BUILD_FLAG= -O3

else

BUILD_FLAG= -g3

endif

CXXFLAGS += $(BUILD_FLAG)
CXXFLAGS += -std=c++20
CXXFLAGS += -Wall -W -Wextra -Wunused -Wpedantic -Wshadow -Wconversion -Werror
CXXFLAGS += -I$(DIR_SRC)
CXXFLAGS += -I$(DIR_LIB_GERONIMO)/src
CXXFLAGS += -I$(DIR_LIB_GERONIMO)/thirdparties
# CXXFLAGS += -I$(DIR_LIB_GERONIMO)/thirdparties/dependencies/entt/src
CXXFLAGS += -I$(DIR_LIB_GERONIMO)/thirdparties/dependencies/entt/src/entt/entity
CXXFLAGS += -I$(DIR_LIB_GERONIMO)/thirdparties/dependencies/json/include

# CXXFLAGS += -DBT_NO_PROFILE
# CXXFLAGS += -DSIMD_FORCE_INLINE=inline
# # CXXFLAGS += -DBT_USE_DOUBLE_PRECISION


LDFLAGS += $(BUILD_FLAG)
LDFLAGS += $(NAME_LIB_GERONIMO)
LDFLAGS += $(NAME_LIB_BULLET_PHYSICS)

LIBRARIES += sdl2 glesv2 openal

LDFLAGS += $(shell pkg-config $(LIBRARIES) --libs)

ifeq ($(build_platform),native)

CXX=clang++
AR=ar

CXXFLAGS += $(shell pkg-config $(LIBRARIES) --cflags)

else ifeq ($(build_platform),web-wasm)

CXX=em++
AR=emar

CXXFLAGS += -s USE_SDL=2
CXXFLAGS += -s USE_PTHREADS=0

LDFLAGS+= -s GL_PREINITIALIZED_CONTEXT=1
LDFLAGS+= -s USE_WEBGL2=1
LDFLAGS+= -s FULL_ES3=1
LDFLAGS+=	-s WASM=1
LDFLAGS+=	--preload-file ./assets/


# LDFLAGS+=	--preload-file $(DIR_LIB_GERONIMO)/src/geronimo/graphics/advanced-concept/clusteredDeferred/internals/shaders
LDFLAGS+=	--preload-file $(DIR_LIB_GERONIMO)/src/geronimo/graphics/advanced-concept/depthDeferred/internals/shaders/resultQuadRenderer
LDFLAGS+=	--preload-file $(DIR_LIB_GERONIMO)/src/geronimo/graphics/advanced-concept/depthDeferred/internals/shaders/lightStackRenderer
LDFLAGS+=	--preload-file $(DIR_LIB_GERONIMO)/src/geronimo/graphics/advanced-concept/depthDeferred/internals/shaders/fame-merger

LDFLAGS+=	--preload-file $(DIR_LIB_GERONIMO)/src/geronimo/graphics/advanced-concept/textRenderer/assets
LDFLAGS+=	--preload-file $(DIR_LIB_GERONIMO)/src/geronimo/graphics/advanced-concept/stackRenderers/shaders
LDFLAGS+=	--preload-file $(DIR_SRC)/application/context/graphics/renderers/scene/geometries-stack-renderer/shaders
LDFLAGS+=	--preload-file $(DIR_SRC)/application/context/graphics/renderers/scene/scene-stack-renderers/shaders

LDFLAGS += -s EXPORT_NAME="testBedApp"
LDFLAGS += -s MODULARIZE=1

LDFLAGS+=	-s TOTAL_MEMORY=128Mb
# LDFLAGS+= -s EXPORTED_RUNTIME_METHODS=cwrap
# LDFLAGS += -s EXPORTED_FUNCTIONS="['_free']"
# LDFLAGS += -s EXPORTED_RUNTIME_METHODS="['cwrap','_malloc','stringToUTF8','lengthBytesUTF8','UTF8ToString']"
LDFLAGS += -s EXPORTED_RUNTIME_METHODS="['cwrap']"

ifeq ($(build_mode),debug)

LDFLAGS+=	-s DEMANGLE_SUPPORT=1
LDFLAGS+=	-s GL_ASSERTIONS=1
LDFLAGS+=	-s GL_DEBUG=1

endif


endif


RM=			rm -rf


#######

#
## RULE(S)

all:	application

ensurefolders:
	@mkdir -p `dirname $(NAME_APPLICATION)`

application:	ensurefolders $(OBJ)
	@echo ' ---> building $(LOG_INFO) "application"'
	@$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME_APPLICATION) $(LDFLAGS)
	@echo '   --> built $(LOG_INFO) "application"'

#

# for every ".cpp" file
# => ensure the "obj" folder(s)
# => compile in a ".o" file

# # pull in dependency info for *existing* .o files
# -include $(OBJ:.o=.o.d)

$(DIR_OBJ)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo ' --> processing $(LOG_INFO):' $<
	@$(CXX) -c $(CXXFLAGS) -MMD -MT "$@" -MF "$@.dep" -o "$@" $<

include $(shell test -d $(DIR_OBJ) && find $(DIR_OBJ) -name "*.dep")

#

clean:
	@echo ' -> cleaning $(LOG_INFO): application build file(s)'
	@$(RM) $(DIR_OBJ)
	@echo '   -> cleaned $(LOG_INFO): application build file(s)'

fclean:	clean
	@echo ' -> cleaned $(LOG_INFO): application file(s)'
	@$(RM) $(NAME_APPLICATION)
	@echo '   -> cleaned $(LOG_INFO): application file(s)'

re:			fclean all

.PHONY:	\
	all \
	ensurefolders \
	application \
	clean \
	fclean \
	re

## RULE(S)
#
