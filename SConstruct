#******************************************************************
#
# Copyright 2014 Intel Mobile Communications GmbH All Rights Reserved.
#
#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
#-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

# prevent accidentally building all. to intentionally build all:
# $ scons .

##
# The main build script
#
##
import SCons
import os
import sys
import platform
import setup.targets
import build.utils

print "CL TARGETS: ", COMMAND_LINE_TARGETS
print "BUILD_TARGETS is", map(str, BUILD_TARGETS)

hvars = build.utils.get_help_vars()

env = Environment(variables = hvars,
                  ENV = os.environ)
Help(hvars.GenerateHelpText(env))

# STEP 1: get host and target OS and ARCH
if sys.platform.startswith('linux'):
    host_os        = 'linux'
elif sys.platform.startswith('darwin'):
    host_os        = 'darwin'
else:
    host_os = sys.platform

host_arch      = platform.machine()
host_processor = platform.processor

#FIXME: put this Replace stuff in a tool fn
env.Replace(HOST_OS = host_os)
env.Replace(HOST_ARCH = host_arch)

try:
    target_os = env['ENV']['TARGET_OS']
except:
    target_os = host_os
env.Replace(TARGET_OS = target_os)

try:
    target_arch = env['ENV']['TARGET_MACHINE']
except:
    target_arch = host_arch
env.Replace(TARGET_ARCH = target_arch)

#env.Replace(TARGET_OS_VERSION = env['ENV']['TARGET_OS_VERSION'])

env.Replace(SOURCE_ROOT = env['ENV']['IOTIVITY_HOME'])
print "SOURCE ROOT:", env['ENV']['IOTIVITY_HOME']

build_sysroot = env['ENV']['BUILD_SYSROOT']
env.Replace(BUILD_SYSROOT = build_sysroot)
env.Replace(INSTALL_SYSROOT = env['ENV']['INSTALL_SYSROOT'])

print "HOST OS, ARCH:", host_os, host_arch
print "TARGET OS, ARCH:", target_os, target_arch

if host_os == target_os:
        if host_arch == target_arch:
                env = setup.targets.host_features(env)
else:
        # we're cross-compiling
        print "CROSS COMPILING"
	if target_os == 'android':
		env = setup.targets.android(env)
	elif target_os == 'poky-edison':
		env = setup.targets.edison(env)
	elif target_os == 'linux':
		env = setup.targets.edison(env)
	else:
	        env = setup.targets.generic(env)

env = build.utils.print_config(env)

print "SOURCE ROOT", env.get('SOURCE_ROOT')
print "RELEASE?", env['RELEASE']
# print "CPPDEFINES", env['CPPDEFINES']
# print "SHLIBSUFFIX", env['SHLIBSUFFIX']

## setup complete
################################################################

Export('env')
# Load common build config
SConscript('hosts/SConscript')

Import('env')

# print env.Dump()

# if env.get('VERBOSE'):
#     print "ENV:"
#     print env.Dump()
#     print "START"

if os.environ.get('TERM') != None:
	env['ENV']['TERM'] = os.environ['TERM']

# target_os = env.get('TARGET_OS')
# target_arch = env.get('TARGET_ARCH')
src_dir = env.get('SRC_DIR')

build_dir = env.get('BUILD_DIR')

SConscript('jni-c/src/c/SConscript',
           variant_dir=build_sysroot + '/jni',
           duplicate=0)

print "BUILD_TARGETS is", map(str, BUILD_TARGETS)
#if GetOption('aliases'):

print 'Available Target Aliases:'

print '-----'

aliases = SCons.Node.Alias.default_ans.keys()

aliases.sort()

for x in aliases:

    print '\t', x

Default('jni-c')

#print env.Dump()
