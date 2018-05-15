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
