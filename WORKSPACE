android_sdk_repository(
    name="androidsdk",
    # Linux default install locn:
#    path="/home/gar/Android/Sdk",
    path="/Users/gar/sdk/android",
    api_level=27,	   # 24, 25: Nougat; 26, 27: Oreo
    # default uses latest build tools installed
    # build_tools_version="26.1.1"
)

android_ndk_repository(
    name="androidndk",
#    path="$HOME/ndk/android-ndk-r14b",
#    path="$HOME/sdk/android/ndk-bundle/",
    api_level=27,
)

local_repository(
	name = "openocf",
	path = "/Users/gar/openocf",
#	path = "/home/gar/openocf",
)

# new_local_repository(
#   name = "toolchain_ndk",
#   path="$HOME/sdk/android/ndk-bundle/",
#   build_file = 'platforms/ndk/toolchain.BUILD',
# )

new_local_repository(
  name = "toolchain_rpi3b",
  path = "/Volumes/CrossToolNG/armv8-rpi3-linux-gnueabihf",
  build_file = 'platforms/rpi3b/toolchain.BUILD',
)

# new_local_repository(
#   name = "cosysroot_ndk",
#   path = "$HOME/cosysroots/ndk",
#   build_file = "platforms/ndk/cosysroot.BUILD"
# )

# bind(name = "android/crosstool", actual = "@android_ndk//:toolchain-libcpp")


################################################################
####  External repos
## these are temporary until Bazel can use the origin WORKSPACE for externals

# Third-party package for Boost, from https://github.com/nelhage/rules_boost
# This will download and compile Boost when you reference labels like "@boost//:iostreams"
git_repository(
    name = "com_github_nelhage_rules_boost",
    commit = "239ce40e42ab0e3fe7ce84c2e9303ff8a277c41a",
    remote = "https://github.com/nelhage/rules_boost",
)
load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")
boost_deps()

# libcoap
new_http_archive(
    name = "libcoap",
    urls = ["https://github.com/dthaler/libcoap/archive/IoTivity-1.2.1d.zip"],
    sha256 = "aac5b9101fad4cf722f0166f2739ebcefe55ad0ca7aa475550b11c8eb4740511",
    strip_prefix = "libcoap-IoTivity-1.2.1d",
    build_file = "config/libcoap.BUILD",
)

# new_local_repository(
# 	name = "coap_hdrs",
# 	path = "/Users/gar/iotivity/gerrit/config/darwin",
# 	build_file = "config/darwin/BUILD",
# 	#build_file_content = "filegroup(name='foo', srcs=['darwin/coap.h','darwin/coap_config.h'])"
# )

new_http_archive(
    name = "mbedtls",
    urls = ["https://github.com/ARMmbed/mbedtls/archive/mbedtls-2.4.2.zip"],
    sha256 = "dacb9f5dd438c456b9ef6627637f46e16fd41e86d828217ec9f8547d3d22a338",
    strip_prefix = "mbedtls-mbedtls-2.4.2",
    build_file = "config/mbedtls/BUILD",
)
    # urls = ["https://github.com/ARMmbed/mbedtls/archive/mbedtls-2.7.zip"],
    # sha256 = "ab1ea18e7fa721bb4ab08294ab34008c4a934841b037a3cfa5f7ce65648228a2",
    # strip_prefix = "mbedtls-mbedtls-2.7",

# tinycbor
new_http_archive(
    name = "tinycbor",
    urls = ["https://github.com/intel/tinycbor/archive/v0.4.1.zip"],
    sha256 = "c5ace3389cfdd6a121824a26f9c14486df298ba0088209d67aef5bc192827d6e",
    strip_prefix = "tinycbor-0.4.1",
    build_file = "config/tinycbor.BUILD",
    #urls = ["https://github.com/intel/tinycbor/archive/v0.5.1.zip"],
    #sha256 = "48e664e10acec590795614ecec1a71be7263a04053acb9ee81f7085fb9116369",
    #strip_prefix = "tinycbor-0.5.1",
)

# gtest
new_http_archive(
    name = "gtest",
    urls = ["https://github.com/google/googletest/archive/release-1.7.0.zip"],
    sha256 = "b58cb7547a28b2c718d1e38aee18a3659c9e3ff52440297e965f5edffe34b6d0",
    strip_prefix = "googletest-release-1.7.0",
    build_file = "config/gtest.BUILD",
)

# sqlite3
new_http_archive(
    name = "sqlite3",
    urls = ["https://www.sqlite.org/2015/sqlite-amalgamation-3081101.zip"],
    sha256 = "a3b0c07d1398d60ae9d21c2cc7f9be6b1bc5b0168cd94c321ede9a0fce2b3cd7",
    strip_prefix = "sqlite-amalgamation-3081101",
    build_file = "config/sqlite3.BUILD",
)

# cJSON
new_http_archive(
    name = "cjson",
    urls = ["https://github.com/DaveGamble/cJSON/archive/v1.7.7.zip"],
    sha256 = "9494f0a9b2005f471c846fbdbb40a9de074ad5aee56e3e73518055cd639fa77e",
    strip_prefix = "cJSON-1.7.7",
    build_file = "config/cjson.BUILD",
)
