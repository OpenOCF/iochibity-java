package(default_visibility = ["//visibility:public"])

filegroup(
    name = "jni_header",
    srcs = ["include/jni.h"],
)

filegroup(
    name = "jni_md_header-darwin",
    srcs = ["include/darwin/jni_md.h"],
)

filegroup(
    name = "jdk-include",
    srcs = glob(["include/**"]),
)

cc_library(
    name = "jdk",
    copts = ["-Iinclude_FOO"],
    hdrs = glob(["include/**"]),
)

cc_library(
    name = "hdrs",
    # copts = ["-Isrc"],
    # deps = ["//src:config"],
    hdrs = glob(["include/**"]),
    visibility = ["//visibility:public"]
)

