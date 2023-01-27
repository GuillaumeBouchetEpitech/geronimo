

DIR_APPLICATION=		./bin
DIR_SRC=						./src
DIR_OBJ=						./obj
DIR_LIB_GERONIMO=		..
DIR_THIRDPARTY=			$(DIR_LIB_GERONIMO)/thirdparties

#
#
#

NAME_APPLICATION=	$(DIR_APPLICATION)/exec

NAME_GERONIMO+=		$(DIR_LIB_GERONIMO)/lib/native/lib-geronimo-system.a
NAME_GERONIMO+=		$(DIR_LIB_GERONIMO)/lib/native/lib-geronimo-graphic.a
NAME_GERONIMO+=		$(DIR_LIB_GERONIMO)/lib/native/lib-geronimo-physics.a
NAME_GERONIMO+=		$(DIR_LIB_GERONIMO)/lib/native/lib-geronimo-audio.a
NAME_GERONIMO+=		$(DIR_LIB_GERONIMO)/lib/native/lib-geronimo-network.a

NAME_BULLET_PHYSICS+=		$(DIR_THIRDPARTY)/lib/native/lib-bullet-physics-dynamics.a
NAME_BULLET_PHYSICS+=		$(DIR_THIRDPARTY)/lib/native/lib-bullet-physics-collision.a
NAME_BULLET_PHYSICS+=		$(DIR_THIRDPARTY)/lib/native/lib-bullet-physics-linearmath.a

#
#
#

SRC+= \
	$(wildcard \
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
		$(DIR_SRC)/graphic/frustum_culling/*.cpp \
	)

SRC+= \
	$(wildcard \
		$(DIR_SRC)/physic/*.cpp \
	)

# SRC+= \
# 	$(wildcard \
# 		$(DIR_SRC)/audio/*.cpp \
# 	)

SRC+= \
	$(wildcard \
		$(DIR_SRC)/network/*.cpp \
		$(DIR_SRC)/network/tcp_sockets/*.cpp \
		$(DIR_SRC)/network/udp_sockets/*.cpp \
	)


OBJ=		$(patsubst %.cpp, $(DIR_OBJ)/%.o, $(SRC))



BUILD_FLAG= -g3

CXXFLAGS += $(BUILD_FLAG)
CXXFLAGS += -std=c++17
CXXFLAGS += -Wall -W -Wextra -Wunused -Wpedantic -Wshadow -Wconversion -Werror
CXXFLAGS += -I$(DIR_SRC)
CXXFLAGS += -I$(DIR_LIB_GERONIMO)/src
CXXFLAGS += -I$(DIR_THIRDPARTY)
CXXFLAGS += -I$(DIR_THIRDPARTY)/dependencies/bullet3/src
CXXFLAGS += -no-pie

LDFLAGS += $(BUILD_FLAG)
LDFLAGS += $(NAME_GERONIMO)
LDFLAGS += $(NAME_BULLET_PHYSICS)

DEPENDENCIES_LIST += sdl2
DEPENDENCIES_LIST += glesv2
DEPENDENCIES_LIST += openal
DEPENDENCIES_LIST += gtest_main

CXXFLAGS +=	$(shell pkg-config $(DEPENDENCIES_LIST) --cflags)
LDFLAGS +=	$(shell pkg-config $(DEPENDENCIES_LIST) --libs)


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
