THIS_ROOT:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

ifeq ($(strip $(YAUL_INSTALL_ROOT)),)
  $(error Undefined YAUL_INSTALL_ROOT (install root directory))
endif

include $(YAUL_INSTALL_ROOT)/share/build.pre.mk

# Required for library usage
include $(YAUL_INSTALL_ROOT)/share/build.mic3d.mk

# Each asset follows the format: <path>;<symbol>. Duplicates are removed
BUILTIN_ASSETS=

SH_PROGRAM:= SONIC_RINGWORLDS
SH_SRCS:= \
	src/main.c \
\
	src/backend/cd_loader.c \
	src/backend/workarea.c \
	src/backend/ssv.c \
\
	src/collision/collision_detection.c \
\
	src/level/level.c \
\
	src/meshes/mesh_torus.c \
	src/meshes/mesh_cube.c \
	src/meshes/mesh_m.c \
	src/meshes/mesh_i.c \
	src/meshes/mesh_c.c \
	src/meshes/mesh_sphere.c \
\
	src/object/object.c \
	src/object/obj_components.c \
\
	src/object/character/character.c \
\
	src/object/character/sonic/sonic.c \
\
	src/object/particle/particle.c \
	src/object/ring/ring.c \
\

SH_CFLAGS+= -O2 -I. -DDEBUG -g $(MIC3D_CFLAGS)
SH_LDFLAGS+= $(MIC3D_LDFLAGS)

IP_VERSION:= V1.000
IP_RELEASE_DATE:= 20160101
IP_AREAS:= JTUBKAEL
IP_PERIPHERALS:= JAMKST
IP_TITLE:= SONIC_RINGWORLDS
IP_MASTER_STACK_ADDR:= 0x06004000
IP_SLAVE_STACK_ADDR:= 0x06001E00
IP_1ST_READ_ADDR:= 0x06004000
IP_1ST_READ_SIZE:= 0

include $(YAUL_INSTALL_ROOT)/share/build.post.iso-cue.mk
