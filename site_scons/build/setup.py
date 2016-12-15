import os, urllib2
from sys import platform as _platform

# test

import subprocess
from string import maketrans
from SCons.Script import *


host_target_map = {
		'linux': ['linux', 'android', 'arduino', 'yocto', 'tizen'],
		'windows': ['windows', 'android', 'arduino'],
		'darwin': ['darwin', 'ios', 'android', 'arduino'],
		'msys_nt' :['msys_nt'],
		}

# Map of os and allowed archs (os: allowed archs)
os_arch_map = {
		'linux': ['x86', 'x86_64', 'arm', 'arm-v7a', 'arm64'],
		'tizen': ['x86', 'x86_64', 'arm', 'arm-v7a', 'armeabi-v7a', 'arm64'],
		'android': ['x86', 'x86_64', 'armeabi', 'armeabi-v7a', 'armeabi-v7a-hard', 'arm64-v8a'],
		'windows': ['x86', 'amd64', 'arm'],
		'msys_nt':['x86', 'x86_64'],
		'darwin': ['i386', 'x86_64'],
		'ios': ['i386', 'x86_64', 'armv7', 'armv7s', 'arm64'],
		'arduino': ['avr', 'arm'],
		'yocto': ['i586', 'x86_64', 'arm', 'powerpc', 'powerpc64', 'mips', 'mipsel'],
		}

# from Android NDK build/core/toolchains
toolchain_arch_abi = {
        'arm':    ['armabi', 'armeabi-v7a'], # 32-bit
        'arm64':  ['arm64-v8a'],             # 64-bit
        'mips':   ['mips'],                  # 32-bit
        'mips64': ['mips64'],                # 64-bit
        'x86':    ['x86'],                   # 32-bit
        'x86_64': ['x86_64']                 # 64-bit
        }

# see ndk build/core/init.mk
def get_host(env):
        # auto-detect host os, arch, etc. if not provided in env var
# For all systems, we will have HOST_OS_BASE defined as
# $(HOST_OS), except on Cygwin where we will have:
#
#  HOST_OS      == cygwin
#  HOST_OS_BASE == windows
        return env

def toolchain(env):
        #FIXME: currently this is just for POC.
        # TODO: elim hardcoded stuff, use sourced env vars to drive config
        # TODO: get the versioning of the tools, needed for linking to crystax boost libs

        # First: tools
        env.Replace(CC = env['ENV']['IOTIVITY_TOOLCHAIN_DIR'] + '/bin/arm-linux-androideabi-gcc')
        env.Replace(CXX = env['ENV']['IOTIVITY_TOOLCHAIN_DIR'] + '/bin/arm-linux-androideabi-g++')
        env.Replace(LINK = env['ENV']['IOTIVITY_TOOLCHAIN_DIR'] + '/bin/arm-linux-androideabi-g++')
        env.Replace(LD = env['ENV']['IOTIVITY_TOOLCHAIN_DIR'] + '/bin/arm-linux-androideabi-ld')
        env.Replace(AR = env['ENV']['IOTIVITY_TOOLCHAIN_DIR'] + '/bin/arm-linux-androideabi-ar')
        env.Replace(RANLIB = env['ENV']['IOTIVITY_TOOLCHAIN_DIR'] + '/bin/arm-linux-androideabi-ranlib')

        # Then: transports
        try:
                ip_api = env['ENV']['IOTIVITY_IP_API']
                if ip_api:
                        env.Append(CPPDEFINES = ['IP_ADAPTER'])
        except KeyError:
                env.Append(CPPDEFINES = ['NO_IP_ADAPTER'])

        try:
                bt_api = env['ENV']['IOTIVITY_BT_API']
                if bt_api:
                        env.Append(CPPDEFINES = ['EDR_ADAPTER'])
        except KeyError:
                env.Append(CPPDEFINES = ['NO_EDR_ADAPTER'])

        try:
                ble_api = env['ENV']['IOTIVITY_BLE_API']
                if ble_api:
                        env.Append(CPPDEFINES = ['LE_ADAPTER'])
        except KeyError:
                env.Append(CPPDEFINES = ['NO_LE_ADAPTER'])

        try:
                nfc_api = env['ENV']['IOTIVITY_NFC_API']
                if nfc_api:
                        env.Append(CPPDEFINES = ['NFC_ADAPTER'])
        except KeyError:
                env.Append(CPPDEFINES = ['NO_NFC_ADAPTER'])
        env.Append(CPPDEFINES = ['HAVE_ARPA_INET_H'])
        env.Append(CPPDEFINES = ['HAVE_ASSERT_H'])
        env.Append(CPPDEFINES = ['HAVE_FCNTL_H'])
        env.Append(CPPDEFINES = ['HAVE_LIMITS_H'])
        env.Append(CPPDEFINES = ['HAVE_NETDB_H'])
        env.Append(CPPDEFINES = ['HAVE_NETINET_IN_H'])
        env.Append(CPPDEFINES = ['HAVE_PTHREAD_H'])
        env.Append(CPPDEFINES = ['HAVE_SYS_SOCKET_H'])
        env.Append(CPPDEFINES = ['HAVE_SYS_TIME_H'])
        env.Append(CPPDEFINES = ['HAVE_SYS_TYPES_H'])
        env.Append(CPPDEFINES = ['HAVE_TIME_H'])
        env.Append(CPPDEFINES = ['WITH_POSIX'])
        env.Append(CPPDEFINES = ['__ANDROID__'])
        env.Append(CPPDEFINES = ['__JAVA__'])
        env.Append(CPPDEFINES = ['WITH_POSIX'])
        #GAR FIXME
        # env.Append(LIBPATH = ['/Users/gar/android/crystax-ndk-10.3.2/sources/boost/1.59/libs/armeabi-v7a/llvm-3.6/'])
        env.Append(CPPPATH = ['/Users/gar/android/crystax-ndk-10.3.2/sources/boost/1.59.0/include'])
        env.Append(LIBPATH = ['/Users/gar/android/crystax-ndk-10.3.2/sources/boost/1.59.0/libs/armeabi-v7a/gnu-4.9/'])
        env.Append(LIBS = ['boost_date_time'])
        env.Append(LIBS = ['boost_system'])
        env.Append(LIBS = ['boost_thread'])
        env.AppendUnique(CXXFLAGS = ['-std=c++11'])
        env.AppendUnique(CFLAGS = ['-std=c11'])
        env.AppendUnique(LINKFLAGS = ['-shared'])
        env.Replace(SHLIBSUFFIX = '.so')

        return env

def get_help_vars():
        help_vars = Variables()
        help_vars.Add(BoolVariable('VERBOSE', 'Show compilation', False))
        help_vars.Add(BoolVariable('RELEASE', 'Build for release?', True)) # set to 'no', 'false' or 0 for debug
        # help_vars.Add(EnumVariable('TARGET_OS', 'Target platform', host, host_target_map[host]))

        help_vars.Add(BoolVariable('WITH_RA', 'Build with Remote Access module', False))
        help_vars.Add(BoolVariable('WITH_TCP', 'Build with TCP adapter', False))
        help_vars.Add(ListVariable('WITH_MQ', 'Build with MQ publisher/broker', 'OFF', ['OFF', 'SUB', 'PUB', 'BROKER']))
        help_vars.Add(BoolVariable('WITH_CLOUD', 'Build including Cloud Connector and Cloud Client sample', False))
        help_vars.Add(ListVariable('RD_MODE', 'Resource Directory build mode', 'CLIENT', ['CLIENT', 'SERVER']))

        help_vars.Add(BoolVariable('SIMULATOR', 'Build with simulator module', False))

        help_vars.Add(BoolVariable('WITH_RA_IBB', 'Build with Remote Access module(workssys)', False))

        # if target_os in targets_disallow_multitransport:
	help_vars.Add(ListVariable('TARGET_TRANSPORT', 'Target transport', 'IP', ['BT', 'BLE', 'IP', 'NFC']))
        # else:
        # help_vars.Add(ListVariable('TARGET_TRANSPORT', 'Target transport', 'ALL', ['ALL', 'BT', 'BLE', 'IP', 'NFC']))

        # help_vars.Add(EnumVariable('TARGET_ARCH', 'Target architecture', default_arch, os_arch_map[target_os]))
        help_vars.Add(EnumVariable('SECURED', 'Build with DTLS', '0', allowed_values=('0', '1')))
        help_vars.Add(EnumVariable('DTLS_WITH_X509', 'DTLS with X.509 support', '0', allowed_values=('0', '1')))
        help_vars.Add(EnumVariable('TEST', 'Run unit tests', '0', allowed_values=('0', '1')))
        help_vars.Add(BoolVariable('LOGGING', 'Enable stack logging', False)) # logging_default))
        # help_vars.Add(BoolVariable('UPLOAD', 'Upload binary ? (For Arduino)', require_upload))
        help_vars.Add(EnumVariable('ROUTING', 'Enable routing', '', allowed_values=('GW', 'EP')))
        help_vars.Add(EnumVariable('BUILD_SAMPLE', 'Build with sample', 'ON', allowed_values=('ON', 'OFF')))
        # help_vars.AddVariables(('DEVICE_NAME', 'Network display name for device (For Arduino)', device_name, None, None),)
        help_vars.Add(PathVariable('ANDROID_NDK', 'Android NDK path', None, PathVariable.PathAccept))
        help_vars.Add(PathVariable('ANDROID_HOME', 'Android SDK path', None, PathVariable.PathAccept))
        #ES_TARGET_ENROLLEE is specifying what is our target enrollee (Arduino or rest of platforms which support Multicast)
        help_vars.Add(EnumVariable('ES_TARGET_ENROLLEE', 'Target Enrollee', 'arduino', allowed_values=('arduino', 'tizen', 'linux', 'darwin')))
        #ES_ROLE is for specifying the role (Enrollee or Mediator) for which scons is being executed
        help_vars.Add(EnumVariable('ES_ROLE', 'Target build mode', 'mediator', allowed_values=('mediator', 'enrollee')))
        #ES_SOFT_MODE is for specifying MODE (Mode 1 : Enrollee with  Soft AP or Mode 2  : Mediator with Soft AP)
        help_vars.Add(EnumVariable('ES_SOFTAP_MODE', 'Target build mode', 'ENROLLEE_SOFTAP', allowed_values=('ENROLLEE_SOFTAP', 'MEDIATOR_SOFTAP')))
 	help_vars.Add('TC_PREFIX', "Toolchain prefix (Generally only be required for cross-compiling)", os.environ.get('TC_PREFIX'))
 	help_vars.Add(PathVariable('TC_PATH',
 			'Toolchain path (Generally only be required for cross-compiling)',
 			os.environ.get('TC_PATH')))

        return help_vars

        # home        = os.environ['HOME']

# cppdefs   = {'CPPDEFINES': features_env['CPPDEFINES']}
# #cppflags  = {'CPPFLAGS': features_env['CPPFLAGS']}
# pthreadcflags  = {'PTHREAD_CFLAGS': features_env['PTHREAD_CFLAGS']}
# pthreadlibs   = {'PTHREAD_LIBS': features_env['PTHREAD_LIBS']}

# env.MergeFlags(cppdefs)
# #env.MergeFlags(cppflags)
# env.MergeFlags(pthreadcflags)
# env.MergeFlags(pthreadlibs)

# print "CPPDEFINES", env.get('CPPDEFINES')
# print "CPPFLAGS", env.get('CPPFLAGS')


# try:
# 	env.AppendUnique(HAVE_LIBCURL = features_env['HAVE_LIBCURL'])
# except:
# 	pass
