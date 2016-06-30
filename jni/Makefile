# Makefile for building iotivity java api

#FIXME:  check IOTIVITY_HOME

#FIXME:  JAVA_HOME

# == CHANGE THE SETTINGS BELOW TO SUIT YOUR ENVIRONMENT =======================

# Your platform. See PLATS for possible values.
PLAT= none

CC= gcc -std=c11
CXXFLAGS= -O2 -std=c++11 -Wall -Wextra $(SYSCFLAGS) $(MYCFLAGS) $(IOT_CXXFLAGS)
LDFLAGS= $(SYSLDFLAGS) $(MYLDFLAGS)
LIBS= -lm $(SYSLIBS) $(MYLIBS)

AR= ar rcu
MKDIR= mkdir -p
RANLIB= ranlib
RM= rm -f

SYSCFLAGS=
SYSLDFLAGS=
SYSLIBS=

MYCFLAGS=
MYLDFLAGS=
MYLIBS=
MYOBJS=

# == END OF USER SETTINGS -- NO NEED TO CHANGE ANYTHING BELOW THIS LINE =======

IOTLIBPATH = [env.get('BUILD_DIR')])
IOTRPATH = [env.get('BUILD_DIR')])

# IOTLIBS = -loc -loctbstack -loc_logger -lconnectivity_abstraction -lpthread

IOTIVITY_LIB="$(IOTIVITY_HOME)/out/darwin/x86_64/release"

IOTLIBS = $(IOTIVITY_LIB)/liboctbstack.a \
	$(IOTIVITY_LIB)/libconnectivity_abstraction.a \
	$(IOTIVITY_LIB)/liboc_logger.dylib \
	$(IOTIVITY_LIB)/liboc.dylib \
	-lpthread

# ifeq ($(SECURED), 1)
#     IOTLIBS += "-ltinydtls -locprovision -locpmapi"
# endif

# if target_os == 'linux':
#    shlibsuffix = '.so'
#    jnilibsuffix = '.so'
# elif target_os == 'darwin':
#    shlibsuffix = '.dylib'
#    jnilibsuffix = '.jnilib'
# elif target_os == 'win32':
#    shlibsuffix = '.dll'
#    jnilibsuffix = '.dll'

#    jni_env.Replace(SHLIBSUFFIX = '.jnilib')

# if target_os in ['darwin', 'ios']:
# 	jni_env.AppendUnique(CPPDEFINES = ['_DARWIN_C_SOURCE'])

PLATS= aix bsd c89 freebsd generic linux darwin mingw posix solaris

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


IOT_CXXFLAGS= -I. \
	-I./src \
	-I${JAVA_HOME}/include \
	-I${JAVA_HOME}/include/darwin \
	-I${IOTIVITY_HOME}/resource/csdk/connectivity/api \
	-I${IOTIVITY_HOME}/resource/include \
	-I${IOTIVITY_HOME}/resource/c_common \
	-I${IOTIVITY_HOME}/resource/c_common/oic_string/include \
	-I${IOTIVITY_HOME}/resource/c_common/oic_malloc/include \
	-I${IOTIVITY_HOME}/resource/csdk/stack/include \
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
default: $(PLAT)

BASE_O= $(CORE_O)

all:	$(BASE_O)

clean:
	$(RM) $(BASE_O)
	$(RM) *dylib *jnilib

# depend:
# 	@$(CC) $(CFLAGS) -MM l*.c

echo:
	@echo "PLAT= $(PLAT)"
	@echo "CC= $(CC)"
	@echo "CFLAGS= $(CFLAGS)"
	@echo "LDFLAGS= $(SYSLDFLAGS)"
	@echo "LIBS= $(LIBS)"
	@echo "AR= $(AR)"
	@echo "RANLIB= $(RANLIB)"
	@echo "RM= $(RM)"

# Convenience targets for popular platforms
ALL= all

linux:
	$(MAKE) $(ALL) SYSCFLAGS="-DIOT_USE_LINUX" SYSLIBS="-Wl,-E -ldl -lreadline"

darwin:
	$(MAKE) $(ALL) $(CXX_FLAGS) SYSCFLAGS="-DDARWIN_C_SOURCE"
	 g++ -o libiotivity-jni.jnilib -dynamiclib -undefined error \
	-rpath $HOME/out/darwin/x86_64/release \
	-flat_namespace \
	$(IOTLIBS) \
	-L$(IOTIVITY_HOME)/out/darwin/x86_64/release \
	-L. \
	$(CORE_O)

# -flat_namespace


# list targets that do not create files (but not all makes understand .PHONY)
.PHONY: all $(PLATS) default o a clean depend echo none

# DO NOT DELETE

iochibity.o: JniOcStack.o JniUtils.o JniEntityHandler.o JniOnResourceFoundListener.o \
	JniOnDeviceInfoListener.o JniOnPlatformInfoListener.o JniOnPresenceListener.o \
	JniOnGetListener.o JniOnPutListener.o JniOnPostListener.o JniOnDeleteListener.o \
	JniOnObserveListener.o JniOcRepresentation.o JniOcResourceHandle.o \
	JniOcPresenceHandle.o JniOcRequestHandle.o JniOcResourceRequest.o \
	JniOcResourceResponse.o JniOcPlatform.o JniOcResource.o \
	JniOcResourceIdentifier.o JniOcSecurity.o

iotsec.o: JniOcSecureResource.o \
	JniOcProvisioning.o JniProvisionResultListner.o \
	JniPinCheckListener.o JniDisplayPinListener.o \
	JniSecureUtils.o

# (end of Makefile)
