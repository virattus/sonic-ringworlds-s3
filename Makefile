THIS_ROOT:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

ifeq ($(strip $(YAUL_INSTALL_ROOT)),)
  $(error Undefined YAUL_INSTALL_ROOT (install root directory))
endif

include $(YAUL_INSTALL_ROOT)/share/build.pre.mk

# Required for library usage
include $(YAUL_INSTALL_ROOT)/share/build.mic3d.mk

SATCONV:= $(YAUL_INSTALL_ROOT)/bin/satconv$(EXE_EXT)

# Each asset follows the format: <path>;<symbol>. Duplicates are removed
#BUILTIN_ASSETS=\
	#assets/mrc_square_16x16.cpd;asset_font_cpd \

SH_PROGRAM:= SONIC_RINGWORLDS
SH_SRCS:= \
	src/main.c \
	src/globals.c \
\
	src/backend/cd_loader.c \
	src/backend/controller.c \
	src/backend/ssv.c \
	src/backend/state.c \
	src/backend/workarea.c \
	src/backend/vdp_load.c \
\
	src/backend/sound/soundcontrol.c \
\
	src/collision/aabb.c \
	src/collision/line.c \
	src/collision/plane.c \
	src/collision/polygon.c \
	src/collision/sphere.c \
	src/collision/capsule.c \
	src/collision/octree.c \
	src/collision/collisionWorld.c \
	src/collision/frustum.c \
\
	src/level/level.c \
\
	src/object/character/character.c \
\
	src/object/character/player/player.c \
	src/object/character/player/sonic.c \
	src/object/character/player/tails.c \
	src/object/character/player/knuckles.c \
	src/object/character/player/amy.c \
\
	src/object/particle/particle.c \
	src/object/ring/ring.c \
	src/object/ring/ring_bounce.c \
\
	src/object/TPCamera/TPCamera.c \
\
	src/states/creditsstate.c \
	src/states/levelstate.c \
	src/states/titlestate.c \
\
	src/states/cameramovement.c \
	src/states/cdtest.c \
	src/states/modelloading.c \
	src/states/soundtest.c \
	src/states/testcollision.c \
	src/states/vdp2_ngbtest.c \
	src/states/rng_output.c \
\

SH_CFLAGS+= -O2 -I. -I./src -DDEBUG -g $(MIC3D_CFLAGS)
SH_LDFLAGS+= $(MIC3D_LDFLAGS)

IP_VERSION:= V1.000
IP_RELEASE_DATE:= 20250101
IP_AREAS:= JTUBKAEL
IP_PERIPHERALS:= JAMKST
IP_TITLE:= SONIC_RINGWORLDS
IP_MASTER_STACK_ADDR:= 0x06004000
IP_SLAVE_STACK_ADDR:= 0x06001E00
IP_1ST_READ_ADDR:= 0x06004000
IP_1ST_READ_SIZE:= 0

include $(YAUL_INSTALL_ROOT)/share/build.post.iso-cue.mk

work/mrc_squa.cpd work/mrc_squa.pal: work/mrc_square_16x16.bmp
	$(ECHO)cd work; $(SATCONV) assets.txt
	$(ECHO)python work/strip_tle.py work/mrc_squa.tle work/mrc_squa.cpd work/mrc_squa.pal
	$(ECHO)rm -f work/mrc_squa.tle

assets/mrc_square_16x16.cpd: work/mrc_squa.cpd work/mrc_squa.pal
	@printf -- "$(V_BEGIN_MAGENTA)$@$(V_END)\n"
	$(ECHO)python work/4bpp_to_1bpp.py work/mrc_squa.cpd $@

assets/mrc_square_16x16.pal: assets/mrc_square_16x16.cpd
	@printf -- "$(V_BEGIN_MAGENTA)$@$(V_END)\n"
	$(ECHO)cp work/mrc_squa.pal $@

.PHONY: .clean-assets
