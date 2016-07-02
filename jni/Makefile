# Makefile for building iotivity java api


# == CHANGE THE SETTINGS BELOW TO SUIT YOUR ENVIRONMENT =======================
# Edison: linux, x86
# Ubuntu: linux, x86_64
# HOST=linux
# ARCH=x86_86
# STAGE=release
# SHLIBSUFFIX =.so
# JNILIBSUFFIX =.so

HOST=darwin
ARCH=x86_64
STAGE=release
SHLIBSUFFIX =.dylib
JNILIBSUFFIX =.jnilib

# Where to install. The installation starts in the src and doc directories,
# so take care if INSTALL_TOP is not an absolute path. See the local target.
INSTALL_TOP= /usr/local
INSTALL_BIN= $(INSTALL_TOP)/bin
INSTALL_INC= $(INSTALL_TOP)/include
INSTALL_LIB= $(INSTALL_TOP)/lib
INSTALL_MAN= $(INSTALL_TOP)/man/man1
INSTALL_JNI= $(INSTALL_TOP)/lib/iotivity/$V

# How to install. If your install program does not support "-p", then
# you may have to run ranlib on the installed liblua.a.
INSTALL= install -p
# INSTALL_EXEC= $(INSTALL) -m 0755
INSTALL_DATA= $(INSTALL) -m 0644
#
# If you don't have "install" you can use "cp" instead.
# INSTALL= cp -p
# INSTALL_EXEC= $(INSTALL)
# INSTALL_DATA= $(INSTALL)

# Programs
CC     ?= gcc
AR     ?= ar rcu
MKDIR  ?= mkdir -p
RANLIB ?= ranlib
RM     ?= rm -f

# Options
CXXFLAGS= -std=c++11 -fPIC -Wall -Wextra $(SYSCFLAGS) $(MYCFLAGS) $(IOT_CXXFLAGS)
CFLAGS  = -std=c11
LDFLAGS= $(SYSLDFLAGS) $(MYLDFLAGS)
LIBS= -lm $(SYSLIBS) $(MYLIBS)

SYSCFLAGS=
SYSLDFLAGS=
SYSLIBS=

OSXCFLAGS=-Wno-unused-private-field -Wno-unused-parameter -Wno-unused-function
MYLDFLAGS=
MYLIBS=
MYOBJS=

# == END OF USER SETTINGS -- NO NEED TO CHANGE ANYTHING BELOW THIS LINE =======
# What to install.
TO_LIB= jni/libiotivity-jni.jnilib
# TO_MAN= iotivity.1 iotivity.1

# version and release.
V= 1.1
R= $V.10


# IOTLIBPATH = [env.get('BUILD_DIR')])
# IOTRPATH = [env.get('BUILD_DIR')])

IOTLIBS = -loc -loctbstack -loc_logger -lconnectivity_abstraction -lpthread

IOTIVITY_LIBPATH="$(IOTIVITY_HOME)/out/$(HOST)/$(ARCH)/$(STAGE)"

# IOTLIBS = $(IOTIVITY_LIBPATH)/liboctbstack.a \
# 	$(IOTIVITY_LIBPATH)/libconnectivity_abstraction.a \
# 	$(IOTIVITY_LIBPATH)/liboc_logger.dylib \
# 	$(IOTIVITY_LIBPATH)/liboc.dylib \
# 	-lpthread

# ifeq ($(SECURED), 1)
#     IOTLIBS += "-ltinydtls -locprovision -locpmapi"
# endif

# elif target_os == 'win32':
#    shlibsuffix = '.dll'
#    jnilibsuffix = '.dll'

#    jni_env.Replace(SHLIBSUFFIX = '.jnilib')

# if target_os in ['darwin', 'ios']:
# 	jni_env.AppendUnique(CPPDEFINES = ['_DARWIN_C_SOURCE'])

# if target_arch in ['x86']:
# 	env.AppendUnique(CCFLAGS = ['-m32'])
# 	env.AppendUnique(LINKFLAGS = ['-m32'])
# elif target_arch in ['x86_64']:
# 	env.AppendUnique(CCFLAGS = ['-m64'])
# 	env.AppendUnique(LINKFLAGS = ['-m64'])
# elif target_arch in ['arm'] or target_arch.find('v5') > 0:
# 	env.AppendUnique(CPPFLAGS = ['-march=armv5te'])
# elif target_arch.find('v7a-hard') > 0:
# 	env.AppendUnique(CPPFLAGS = ['-march=armv7-a'])
# 	env.AppendUnique(CPPFLAGS = ['-mfloat-abi=hard'])
# 	env.AppendUnique(CCFLAGS = ['-mfloat-abi=hard'])
# 	env.AppendUnique(LINKFLAGS = ['-mfloat-abi=hard'])
# elif target_arch.find('v7a') > 0:
# 	env.AppendUnique(CPPFLAGS = ['-march=armv7-a'])
# elif target_arch.find('arm64') >= 0:
# 	env.AppendUnique(CPPFLAGS = ['-march=armv8-a'])

	# -I$/usr/include/mraa \

IOT_CXXFLAGS= -I. \
	-I./src \
	-I${JAVA_HOME}/include \
	-I${JAVA_HOME}/include/$(HOST) \
	-I${IOTIVITY_HOME}/resource/csdk/connectivity/api \
	-I${IOTIVITY_HOME}/resource/include \
	-I${IOTIVITY_HOME}/resource/c_common \
	-I${IOTIVITY_HOME}/resource/c_common/oic_string/include \
	-I${IOTIVITY_HOME}/resource/c_common/oic_malloc/include \
	-I${IOTIVITY_HOME}/resource/csdk/stack/include \
	-I${IOTIVITY_HOME}/resource/csdk/stack/include/internal \
	-I${IOTIVITY_HOME}/resource/csdk/ocsocket/include \
	-I${IOTIVITY_HOME}/resource/oc_logger/include \
	-I${IOTIVITY_HOME}/resource/csdk/logger/include \
	-I${IOTIVITY_HOME}/resource/../extlibs/boost/boost_1_58_0 \
	-I${IOTIVITY_HOME}/resource/../build_common/android/compatibility \
	-I${IOTIVITY_HOME}/resource/csdk/security/provisioning/include \
	-I${IOTIVITY_HOME}/resource/csdk/security/provisioning/include/oxm \
	-I${IOTIVITY_HOME}/resource/csdk/security/provisioning/include/internal \
	-I${IOTIVITY_HOME}/resource/csdk/security/include

# IOT_A=	liblua.a
CORE_O= JniOcStack.o JniUtils.o JniEntityHandler.o JniOnResourceFoundListener.o \
	JniOnDeviceInfoListener.o JniOnPlatformInfoListener.o JniOnPresenceListener.o \
	JniOnGetListener.o JniOnPutListener.o JniOnPostListener.o JniOnDeleteListener.o \
	JniOnObserveListener.o JniOcRepresentation.o JniOcResourceHandle.o \
	JniOcPresenceHandle.o JniOcRequestHandle.o JniOcResourceRequest.o \
	JniOcResourceIdentifier.o JniOcSecurity.o \
	JniOcResourceResponse.o JniOcPlatform.o JniOcResource.o

# SEC=	JniOcSecureResource.o JniOcProvisioning.o JniSecureUtils.o \
# 	JniProvisionResultListner.o JniPinCheckListener.o JniDisplayPinListener.o


# Targets start here.
# default: $(PLAT)

BASE_O= $(CORE_O)

# Convenience targets for popular platforms
# ALL= all

all:	check-env $(BASE_O)

ALL=all

# NB: do not change the order of the CORE_O files and the libs:
linux:	check-env
	echo "Making linux"
	$(MAKE) $(ALL)
	g++ -o libiotivity-jni$(JNILIBSUFFIX) \
	-shared \
	-L. $(CORE_O) \
	-L$(IOTIVITY_HOME)/out/$(HOST)/$(ARCH)/$(STAGE) \
	$(IOTLIBS)

darwin: check-env
	@echo "JNILIBSUFFIX: $(JNILIBSUFFIX)"
	@$(MAKE) $(ALL) CXXFLAGS="$(CXXFLAGS) $(OSXCFLAGS)"
	@g++ -o libiotivity-jni$(JNILIBSUFFIX) -dynamiclib -undefined error \
	-rpath $(HOME)/out/darwin/x86_64/release \
	-flat_namespace \
	$(IOTLIBS) \
	-L$(IOTIVITY_HOME)/out/darwin/x86_64/release \
	-L. \
	$(CORE_O)

# -flat_namespace

# all:	$(BASE_O)

clean:
	$(RM) $(BASE_O)
	$(RM) *dylib *jnilib

# make may get confused with test/ and install/
dummy:

test:	dummy
	@echo test?

install: dummy
	$(MKDIR) $(INSTALL_LIB) $(INSTALL_JNI)
	$(INSTALL_DATA) $(TO_LIB) $(INSTALL_JNI)

	# -DlocalRepositoryPath=$(MVN_REPO)

	# cd src && $(INSTALL_EXEC) $(TO_BIN) $(INSTALL_BIN)
	# cd src && $(INSTALL_DATA) $(TO_INC) $(INSTALL_INC)
	# cd doc && $(INSTALL_DATA) $(TO_MAN) $(INSTALL_MAN)

uninstall:
	cd src && cd $(INSTALL_LIB) && $(RM) $(TO_LIB)

	# cd src && cd $(INSTALL_BIN) && $(RM) $(TO_BIN)
	# cd src && cd $(INSTALL_INC) && $(RM) $(TO_INC)
	# cd doc && cd $(INSTALL_MAN) && $(RM) $(TO_MAN)

local:
	$(MAKE) install INSTALL_TOP=../install

# depend:
# 	@$(CC) $(CFLAGS) -MM l*.c

echo:	check-env
	@echo "HOST= $(HOST)"
	@echo "CC= $(CC)"
	@echo "CFLAGS= $(CFLAGS)"
	@echo "LDFLAGS= $(SYSLDFLAGS)"
	@echo "LIBS= $(LIBS)"
	@echo "AR= $(AR)"
	@echo "RANLIB= $(RANLIB)"
	@echo "RM= $(RM)"
	@echo "SHLIBSUFFIX= $(SHLIBSUFFIX)"
	@echo "JNILIBSUFFIX= $(JNILIBSUFFIX)"


# list targets that do not create files (but not all makes understand .PHONY)
# .PHONY: all $(PLATS) default o a clean depend echo none
.PHONY: o a clean dummy test

check-env:
ifeq ( $($@), "darwin")
SHLIBSUFFIX = '.dylib'
JNILIBSUFFIX = '.jnilib'
endif
ifndef IOTIVITY_HOME
	$(error IOTIVITY_HOME is undefined)
endif
ifeq ($(HOST),'linux')
SHLIBSUFFIX = '.so'
JNILIBSUFFIX = '.so'
endif

# ifeq ($(STAGE),'release')
# MYCFLAGS += -O2
# else
# MYCFLAGS += -g
# endif



# DO NOT DELETE

iochibity.o: \
	JniCaInterface.o \
	JniEntityHandler.o \
	JniOcDirectPairDevice.o \
	JniOcPlatform.o \
	JniOcPresenceHandle.o \
	JniOcRepresentation.o \
	JniOcRequestHandle.o \
	JniOcResource.o \
	JniOcResourceHandle.o \
	JniOcResourceIdentifier.o \
	JniOcResourceRequest.o \
	JniOcResourceResponse.o \
	JniOcSecurity.o \
	JniOcStack.o \
	JniOnDPDevicesFoundListener.o \
	JniOnDeleteListener.o \
	JniOnDeviceInfoListener.o \
	JniOnDirectPairingListener.o \
	JniOnGetListener.o \
	JniOnObserveListener.o \
	JniOnPlatformInfoListener.o \
	JniOnPostListener.o \
	JniOnPresenceListener.o \
	JniOnPutListener.o \
	JniOnResourceFoundListener.o \
	JniUtils.o \

iotsec.o: \
	JniOcSecureResource.o \
	JniOcProvisioning.o \
	JniDisplayPinListener.o \
	JniPinCheckListener.o \
	JniProvisionResultListner.o \
	JniSecureUtils.o

# (end of Makefile)
