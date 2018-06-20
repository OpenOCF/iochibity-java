## load("//config:variables.bzl", "DEFINES", "TESTINCLUDES")

CROSSTOOL_NG_HOME="/Volumes/CrossToolNG"

ANDROID_SDK_ROOT="/Users/gar/sdk/android"

CSTD = select({"//config:cstd_c11" : ["-std=c11"],
               "//config:cstd_c90" : ["-std=c90"],
               "//config:cstd_iso9899:1990" : ["-iso9899:1990"],
               "//conditions:default" : ["-std=c11"]
    })

COPTS_ANDROID = ["-std=c11",
                 "-x c"]

TESTINCLUDES = ["-Iresource/csdk/include",
    	        "-Iresource/c_common",
		"-Iresource/c_common/ocrandom/include",
		"-Iresource/c_common/oic_malloc/include",
                "-Iresource/c_common/oic_string/include",
                "-Iresource/c_common/oic_time/include",
    	        "-Iresource/csdk/logger/include",
    	        "-Iresource/csdk/stack/include",
    	        "-Iresource/csdk/stack/include/internal",
                "-Iexternal/gtest/include",
] + select({"//config:darwin": ["-UDEBUG"],
            "//conditions:default": []})

TESTDEPS = ["@gtest//:gtest_main",
            "//resource/c_common",
            "//resource/csdk/logger"]

DEFDTLS = select({"//config:disable_dtls": [],
                  "//conditions:default": ["__WITH_DTLS__"]})

DEFTCP  = select({"@//config:with_tcp": ["TCP_ADAPTER", "WITH_TCP"],
                      "//conditions:default": []})

DEFTLS  = select({"//config:enable_tls": ["__WITH_TLS__"],
                      "//conditions:default": []})

DEFOCF = select({"//config:enable_logging": ["TB_LOG"],
                 "//conditions:default": []})

DEFINES = DEFDTLS + DEFTCP + DEFTLS
