

DIR_SRC=						./src
DIR_APPLICATION=		./bin
DIR_LIB_GERONIMO=			..
DIR_LIB_GERONIMO_SRC=	$(DIR_LIB_GERONIMO)/src
DIR_LIB_GERONIMO_DEP=	$(DIR_LIB_GERONIMO)/lib/native

NAME_APPLICATION=								$(DIR_APPLICATION)/exec
NAME_GERONIMO_SYSTEM=						$(DIR_LIB_GERONIMO_DEP)/lib-geronimo-system.a
NAME_GERONIMO_GRAPHIC=					$(DIR_LIB_GERONIMO_DEP)/lib-geronimo-graphic.a
NAME_GERONIMO_PHYSICS=					$(DIR_LIB_GERONIMO_DEP)/lib-geronimo-physics.a
NAME_GERONIMO_AUDIO=						$(DIR_LIB_GERONIMO_DEP)/lib-geronimo-audio.a
NAME_BULLET_PHYSICS_LINEARMATH=	$(DIR_LIB_GERONIMO_DEP)/lib-bullet-physics-linearmath.a
NAME_BULLET_PHYSICS_COLLISION=	$(DIR_LIB_GERONIMO_DEP)/lib-bullet-physics-collision.a
NAME_BULLET_PHYSICS_DYNAMICS=		$(DIR_LIB_GERONIMO_DEP)/lib-bullet-physics-dynamics.a


SRC+= \
	$(wildcard \
		$(DIR_SRC)/*.cpp \
		$(DIR_SRC)/system/containers/basic_double_linked_list/*.cpp \
		$(DIR_SRC)/system/containers/generic_array_container/*.cpp \
		$(DIR_SRC)/system/containers/dynamic_heap_array/*.cpp \
		$(DIR_SRC)/system/containers/generic_array_container_commons/*.cpp \
		$(DIR_SRC)/system/containers/weak_ref_data_pool/*.cpp \
		$(DIR_SRC)/system/containers/static_heap_grid_array/*.cpp \
		$(DIR_SRC)/system/deterministic_rng/*.cpp \
		$(DIR_SRC)/system/compression/*.cpp \
		$(DIR_SRC)/system/math/*.cpp \
		$(DIR_SRC)/system/multithreading/*.cpp \
	)

SRC+= \
	$(wildcard \
		$(DIR_SRC)/*.cpp \
		$(DIR_SRC)/graphic/frustum_culling/*.cpp \
	)

SRC+= \
	$(wildcard \
		$(DIR_SRC)/*.cpp \
		$(DIR_SRC)/physic/*.cpp \
	)

# $(DIR_SRC)/audio/*.cpp \


DIR_OBJ=	./obj

OBJ=		$(patsubst %.cpp, $(DIR_OBJ)/%.o, $(SRC))



BUILD_FLAG= -g3

CXXFLAGS += $(BUILD_FLAG)
CXXFLAGS += -std=c++17
CXXFLAGS += -Wall -W -Wextra -Wunused -Wpedantic -Wshadow -Wconversion -Werror
CXXFLAGS += -I$(DIR_SRC)
CXXFLAGS += -I$(DIR_LIB_GERONIMO_SRC)
CXXFLAGS += -no-pie

LDFLAGS += $(BUILD_FLAG)
LDFLAGS += $(NAME_GERONIMO_AUDIO)
LDFLAGS += $(NAME_GERONIMO_PHYSICS)
LDFLAGS += $(NAME_GERONIMO_GRAPHIC)
LDFLAGS += $(NAME_GERONIMO_SYSTEM)
LDFLAGS += $(NAME_BULLET_PHYSICS_DYNAMICS)
LDFLAGS += $(NAME_BULLET_PHYSICS_COLLISION)
LDFLAGS += $(NAME_BULLET_PHYSICS_LINEARMATH)

LIBRARIES += sdl2
LIBRARIES += glesv2
LIBRARIES += egl
LIBRARIES += openal
LIBRARIES += gtest_main

CXXFLAGS += $(shell pkg-config $(LIBRARIES) --cflags)
LDFLAGS += $(shell pkg-config $(LIBRARIES) --libs)


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
