# Makefile for building the Java SDK JNI layer

# == CHANGE THE SETTINGS BELOW TO SUIT YOUR ENVIRONMENT =======================

# Edison: linux, x86 (32 bit)
HOST=linux
ARCH=x86

#HOST=darwin
#ARCH=x86_64

STAGE=release

# Where to install. The installation starts in the src and doc directories,
# so take care if INSTALL_TOP is not an absolute path. See the local target.
# You may want to make INSTALL_JAR and INSTALL_JNI consistent with
# LUA_ROOT, LUA_LDIR, and LUA_CDIR in luaconf.h.
INSTALL_TOP= /usr/local
INSTALL_BIN= $(INSTALL_TOP)/bin
INSTALL_INC= $(INSTALL_TOP)/include
INSTALL_LIB= $(INSTALL_TOP)/lib
INSTALL_MAN= $(INSTALL_TOP)/man/man1
INSTALL_JNI= $(INSTALL_TOP)/lib/iotivity/$V

# INSTALL_JAR= $(INSTALL_TOP)/share/iotivity-jvm/$V

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

# Maven
MVN= mvn
PLUGIN=install:install-file
MVN_GROUP=iotivity
MVN_ARTIFACT=iotivity-api
MVN_VERSION=1.1.10-SNAPSHOT

#MVN_REPO=$HOME/.m2/repository
#PLUGIN=org.apache.maven.plugins:maven-install-plugin:2.5.2:install-file


# Other utilities.
MKDIR= mkdir -p
RM= rm -rf

# == END OF USER SETTINGS -- NO NEED TO CHANGE ANYTHING BELOW THIS LINE =======

# Convenience platforms targets.
PLATS= linux darwin android

# What to install.
# TO_BIN= lua luac
# TO_INC= lua.h luaconf.h lualib.h lauxlib.h lua.hpp
TO_JAR= iotivity-api.jar
TO_LIB= jni/libiotivity-jni.jnilib
# TO_MAN= lua.1 luac.1

# version and release.
V= 1.1
R= $V.10

# Targets start here.
all:	$(PLAT)

$(PLATS):
	cd jni && $(MAKE) $@
	$(MKDIR) api/classes
	javac -d api/classes -sourcepath api/src:api/unix api/src/org/iotivity/base/OcResource.java
	javac -d api/classes -sourcepath api/src:api/unix api/src/org/iotivity/base/OcSecureResource.java
	javac -d api/classes -sourcepath api/src:api/unix api/src/org/iotivity/base/OicSecAcl.java
	javac -d api/classes -sourcepath api/src:api/unix api/src/org/iotivity/base/OxmType.java
	cd api/classes && jar cf ../lib/$(TO_JAR) ./

clean:
	cd jni && $(MAKE) clean

	# $(RM) ./lib .api/class

test:	dummy
	echo "test?"

install: dummy
	$(MVN) $(PLUGIN) -Dfile=api/lib/$(TO_JAR) \
	-DgroupId=$(MVN_GROUP) \
	-DartifactId=$(MVN_ARTIFACT) \
	-Dversion=$(MVN_VERSION) \
	-Dpackaging=jar \
	-DgeneratePom=true
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

none:
	@echo "Please do 'make PLATFORM' where PLATFORM is one of these:"
	@echo "   $(PLATS)"
	@echo "See doc/readme.html for complete instructions."

# make may get confused with test/ and install/
dummy:

# echo config parameters
echo:
	@cd src && $(MAKE) -s echo
	@echo "PLAT= $(PLAT)"
	@echo "V= $V"
	@echo "R= $R"
	@echo "TO_LIB= $(TO_LIB)"
	@echo "INSTALL_LIB= $(INSTALL_LIB)"
	# @echo "TO_BIN= $(TO_BIN)"
	# @echo "TO_INC= $(TO_INC)"
	# @echo "TO_MAN= $(TO_MAN)"
	# @echo "INSTALL_TOP= $(INSTALL_TOP)"
	# @echo "INSTALL_BIN= $(INSTALL_BIN)"
	# @echo "INSTALL_INC= $(INSTALL_INC)"
	# @echo "INSTALL_MAN= $(INSTALL_MAN)"
	@echo "INSTALL_JAR= $(INSTALL_JAR)"
	@echo "INSTALL_JNI= $(INSTALL_JNI)"
	# @echo "INSTALL_EXEC= $(INSTALL_EXEC)"
	@echo "INSTALL_DATA= $(INSTALL_DATA)"

# echo pkg-config data
# pc:
# 	@echo "version=$R"
# 	@echo "prefix=$(INSTALL_TOP)"
# 	@echo "libdir=$(INSTALL_LIB)"
# 	@echo "includedir=$(INSTALL_INC)"

# list targets that do not create files (but not all makes understand .PHONY)
.PHONY: all $(PLATS) clean test install local none dummy echo pecho lecho

# (end of Makefile)
