

DIR_SRC=						./src
DIR_APPLICATION=		./bin
DIR_LIB_GERONIMO=			..

NAME_APPLICATION=			$(DIR_APPLICATION)/exec
NAME_GERONIMO+=				$(DIR_LIB_GERONIMO)/lib/native/lib-geronimo-audio.a
NAME_GERONIMO+=				$(DIR_LIB_GERONIMO)/lib/native/lib-geronimo-physics.a
NAME_GERONIMO+=				$(DIR_LIB_GERONIMO)/lib/native/lib-geronimo-graphics.a
NAME_GERONIMO+=				$(DIR_LIB_GERONIMO)/lib/native/lib-geronimo-system.a
NAME_BULLET_PHYSICS+=	$(DIR_LIB_GERONIMO)/thirdparties/lib/native/lib-bullet-physics-dynamics.a
NAME_BULLET_PHYSICS+=	$(DIR_LIB_GERONIMO)/thirdparties/lib/native/lib-bullet-physics-collision.a
NAME_BULLET_PHYSICS+=	$(DIR_LIB_GERONIMO)/thirdparties/lib/native/lib-bullet-physics-linearmath.a

SRC+= \
	$(wildcard \
		$(DIR_SRC)/system/containers/basic_double_linked_list/*.cpp \
		$(DIR_SRC)/system/containers/generic_array_container/*.cpp \
		$(DIR_SRC)/system/containers/dynamic_heap_array/*.cpp \
		$(DIR_SRC)/system/containers/fixed_size_array/*.cpp \
		$(DIR_SRC)/system/containers/generic_array_container_commons/*.cpp \
		$(DIR_SRC)/system/containers/weak_ref_data_pool/*.cpp \
		$(DIR_SRC)/system/containers/static_heap_grid_array/*.cpp \
		$(DIR_SRC)/system/deterministic_rng/*.cpp \
		$(DIR_SRC)/system/parser-utils/*.cpp \
		$(DIR_SRC)/system/compression/*.cpp \
		$(DIR_SRC)/system/math/*.cpp \
		$(DIR_SRC)/system/math/2d/collisions/*.cpp \
		$(DIR_SRC)/system/math/2d/convex-polygon/*.cpp \
		$(DIR_SRC)/system/filtering/*.cpp \
		$(DIR_SRC)/system/multithreading/*.cpp \
		$(DIR_SRC)/system/trees/*.cpp \
	)

SRC+= \
	$(wildcard \
		$(DIR_SRC)/graphic/frustum_culling/*.cpp \
		$(DIR_SRC)/graphic/perspective_clustering/*.cpp \
	)

SRC+= \
	$(wildcard \
		$(DIR_SRC)/physic/*.cpp \
	)

# SRC+= \
# 	$(wildcard \
# 		$(DIR_SRC)/audio/*.cpp \
# 	)


DIR_OBJ=	./obj

OBJ=		$(patsubst %.cpp, $(DIR_OBJ)/%.o, $(SRC))



BUILD_FLAG= -g3

CXX=g++
# CXX=clang++

CXXFLAGS += $(BUILD_FLAG)
CXXFLAGS += -std=c++20
CXXFLAGS += -Wall -W -Wextra -Wunused -Wpedantic -Wshadow -Wconversion -Werror
CXXFLAGS += -I$(DIR_SRC)
CXXFLAGS += -I$(DIR_LIB_GERONIMO)/src
CXXFLAGS += -I$(DIR_LIB_GERONIMO)/thirdparties
CXXFLAGS += -I$(DIR_LIB_GERONIMO)/thirdparties/dependencies/bullet3/src

LDFLAGS += $(BUILD_FLAG)
LDFLAGS += $(NAME_GERONIMO)
LDFLAGS += $(NAME_BULLET_PHYSICS)

# LIBRARIES += sdl2
# LIBRARIES += glesv2
# LIBRARIES += egl
LIBRARIES += openal
LIBRARIES += gtest_main

CXXFLAGS_LINKER = $(CXXFLAGS)
CXXFLAGS_LINKER += $(shell pkg-config $(LIBRARIES) --cflags)
LDFLAGS_LINKER += $(LDFLAGS)
LDFLAGS_LINKER += $(shell pkg-config $(LIBRARIES) --libs)


RM=			rm -rf


#######

#
## RULE(S)

all:	application

ensurefolders:
	@mkdir -p `dirname $(NAME_APPLICATION)`

application:	ensurefolders $(OBJ)
	@echo ' ---> building $(LOG_INFO) "application"'
	@$(CXX) $(CXXFLAGS_LINKER) $(OBJ) -o $(NAME_APPLICATION) $(LDFLAGS_LINKER)
	@echo '   --> built $(LOG_INFO) "application"'

#

$(DIR_OBJ)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo ' --> processing $(LOG_INFO):' $<
	@$(CXX) $(CXXFLAGS) $< -c -o $@

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
