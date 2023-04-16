
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

LOG_INFO= '[$(build_mode)] [$(build_platform)]'

#

DIR_SRC=		./src

#
#

DIR_LIB_GERONIMO=								../..
DIR_LIB_GERONIMO_3RD_PARTY_DIR=	$(DIR_LIB_GERONIMO)/thirdparties

ifeq ($(build_platform),native)

DIR_APPLICATION=		./bin
NAME_APPLICATION=		$(DIR_APPLICATION)/exec

NAME_LIB_GERONIMO+=	\
	$(DIR_LIB_GERONIMO)/lib/native/lib-geronimo-audio.a \
	$(DIR_LIB_GERONIMO)/lib/native/lib-geronimo-physics.a \
	$(DIR_LIB_GERONIMO)/lib/native/lib-geronimo-graphics.a \
	$(DIR_LIB_GERONIMO)/lib/native/lib-geronimo-system.a

NAME_LIB_BULLET_PHYSICS+=	\
	$(DIR_LIB_GERONIMO)/thirdparties/lib/native/lib-bullet-physics-dynamics.a \
	$(DIR_LIB_GERONIMO)/thirdparties/lib/native/lib-bullet-physics-collision.a \
	$(DIR_LIB_GERONIMO)/thirdparties/lib/native/lib-bullet-physics-linearmath.a

else ifeq ($(build_platform),web-wasm)

DIR_APPLICATION=		./dist/wasm
NAME_APPLICATION=		$(DIR_APPLICATION)/index.js

NAME_LIB_GERONIMO+=	\
	$(DIR_LIB_GERONIMO)/lib/web-wasm/lib-geronimo-audio.bc \
	$(DIR_LIB_GERONIMO)/lib/web-wasm/lib-geronimo-physics.bc \
	$(DIR_LIB_GERONIMO)/lib/web-wasm/lib-geronimo-graphics.bc \
	$(DIR_LIB_GERONIMO)/lib/web-wasm/lib-geronimo-system.bc

NAME_LIB_BULLET_PHYSICS+= \
	$(DIR_LIB_GERONIMO)/thirdparties/lib/web-wasm/lib-bullet-physics-linearmath.bc \
	$(DIR_LIB_GERONIMO)/thirdparties/lib/web-wasm/lib-bullet-physics-collision.bc \
	$(DIR_LIB_GERONIMO)/thirdparties/lib/web-wasm/lib-bullet-physics-dynamics.bc

endif



#### DIRS

#### /DIRS



ifeq ($(build_platform),native)

DIR_OBJ=	./obj/native

else ifeq ($(build_platform),web-wasm)

DIR_OBJ=	./obj/web-wasm

endif



#### SRC


# common src
SRC+= \
	$(wildcard \
		$(DIR_SRC)/*.cpp \
		$(DIR_SRC)/application/*.cpp \
		$(DIR_SRC)/application/context/*.cpp \
		$(DIR_SRC)/application/context/graphics/*.cpp \
		$(DIR_SRC)/application/context/graphics/renderers/*.cpp \
		$(DIR_SRC)/application/context/graphics/renderers/hud/helpers/*.cpp \
		$(DIR_SRC)/application/context/graphics/renderers/hud/widgets/*.cpp \
		$(DIR_SRC)/application/context/graphics/renderers/scene/*.cpp \
		$(DIR_SRC)/application/context/graphics/renderers/scene/helpers/*.cpp \
		$(DIR_SRC)/application/context/logic/*.cpp \
		$(DIR_SRC)/application/context/logic/controllers/*.cpp \
		$(DIR_SRC)/application/context/logic/inputManagers/*.cpp \
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

LIBRARIES += sdl2 glesv2 openal

CXXFLAGS += $(BUILD_FLAG)
CXXFLAGS += -std=c++17
CXXFLAGS += -Wall -W -Wextra -Wunused -Wpedantic -Wshadow -Wconversion -Werror
CXXFLAGS += -I$(DIR_SRC)
CXXFLAGS += -I$(DIR_LIB_GERONIMO)/src
CXXFLAGS += -I$(DIR_LIB_GERONIMO)/thirdparties


# CXXFLAGS += -DBT_NO_PROFILE
# CXXFLAGS += -DSIMD_FORCE_INLINE=inline
# # CXXFLAGS += -DBT_USE_DOUBLE_PRECISION


LDFLAGS += $(BUILD_FLAG)
LDFLAGS += $(NAME_LIB_GERONIMO)
LDFLAGS += $(NAME_LIB_BULLET_PHYSICS)

CXXFLAGS += $(shell pkg-config $(LIBRARIES) --cflags)

LDFLAGS += $(shell pkg-config $(LIBRARIES) --libs)

ifeq ($(build_platform),native)

CXX=clang++
AR=ar

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


LDFLAGS+=	--preload-file $(DIR_LIB_GERONIMO)/src/geronimo/graphics/advanced-concept/clusteredDeferred/internals/shaders
LDFLAGS+=	--preload-file $(DIR_LIB_GERONIMO)/src/geronimo/graphics/advanced-concept/textRenderer/assets
LDFLAGS+=	--preload-file $(DIR_LIB_GERONIMO)/src/geronimo/graphics/advanced-concept/stackRenderers/shaders

LDFLAGS+=	-s TOTAL_MEMORY=128Mb
LDFLAGS+= -s EXPORTED_RUNTIME_METHODS=cwrap

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
