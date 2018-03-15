local_repository(
	name = "openocf",
	path = "/home/gar/openocf",
)

new_local_repository(
  name = "toolchain_ndk",
  path="$HOME/android/sdk/ndk-bundle/",
  build_file = 'platforms/ndk/toolchain.BUILD',
)

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

android_sdk_repository(
    name="androidsdk",
    # Linux default install locn:
    path="/home/gar/Android/Sdk",
    api_level=26,	   # Oreo, 8.0.0, required for java.nio.file.*
    # default uses latest build tools installed
    # build_tools_version="27.0.3"
)

android_ndk_repository(
    name="androidndk",
    path="$HOME/ndk/android-ndk-r14b",
    #path="$HOME/android/sdk/ndk-bundle/",
    api_level=26,
)

# bind(name = "android/crosstool", actual = "@android_ndk//:toolchain-libcpp")
