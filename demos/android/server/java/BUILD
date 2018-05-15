android_binary(
    name = "server",
    # default fat_apk_cpu = armeabi-v7a
    # apk name: openocfserver.apk
    # .so output to apk: libopenocfserver.so
    # application must pass "openocfserver" to AndroidService.config(...) so it know what to System.loadLibrary
    srcs = glob(["org/openocf/test/server/*.java"])
    + glob(["org/openocf/test/*.java"]),
    manifest = "org/openocf/test/server/ServerManifest.xml",
    deps = ["//:OpenOCFAndroid",
            "//demos/android/common/java:common"],
        # "@androidsdk//com.android.support:appcompat-v7-22.0.0",
    resource_files = glob(["org/openocf/test/server/res/**"]),
    assets_dir = "config",
    assets = [":server_config"]
)

# prereq: edn-to-json and json2cbor are on the path (e.g. $HOME/bin)
genrule(
    name = "server_json",
    srcs = ["resources/server_config.edn"],
    outs = ["server_config.json"],
    cmd = "edn-to-json $(location resources/server_config.edn)  > \"$@\"",
)

genrule(
    name = "server_config",
    srcs = [":server_json"],
    outs = ["config/svrs.cbor"],
    cmd = "json2cbor $(location :server_json) > \"$@\"",
)

# android_binary(
#     name = "client",
#     srcs = glob(["src/client/java/org/openocf/test/client/*.java"])
#     + glob(["src/client/java/org/openocf/test/*.java"]),
#     manifest = "src/client/java/org/openocf/test/client/ClientManifest.xml",
#     deps = ["@openocf_java//:OpenOCFAndroid"],
#     resource_files = glob(["src/client/java/org/openocf/test/client/res/**"]),
#     assets_dir = "config",
#     assets = [":client_config"]
# )

# genrule(
#     name = "client_json",
#     srcs = ["src/client/java/resources/client_owner_config.edn"],
#     outs = ["client_config.json"],
#     cmd = "edn-to-json $(location src/client/java/resources/client_owner_config.edn)  > \"$@\"",
# )

# genrule(
#     name = "client_config",
#     srcs = [":client_json"],
#     outs = ["config/svrs.cbor"],
#     cmd = "json2cbor $(location :client_json) > \"$@\"",
# )

# android_binary(
#     name = "openocfclient",
#     srcs = glob(["main/**/*.java"]),
#     manifest = "manifests/ClientManifest.xml",
#     resource_files = glob(["res/**"]),
#     deps = [
#         "//:OpenOCFAndroid",
#         "@androidsdk//com.android.support:support-v4-21.0.2",
#         "@androidsdk//com.android.support:support-v13-21.0.2",
#         "@androidsdk//com.android.support:cardview-v7-21.0.2",
#         "@androidsdk//com.android.support:appcompat-v7-22.0.0",
#     ],
# )

# to select a target cpu pass --fat_apk_cpu cpu[,cpu]*
# cc_library(
#     name = "ocftest",
#     copts = ["-Isrc/util"],
#     deps = ["@openocf//src/ocf:hdrs"],
#     alwayslink = 1,
#     linkstatic = 1,
#     srcs = ["main/jni/openocf_Service.c",
#             "main/jni/openocf_Service.h"],
# )

# to generate the jni headers, comment out the :jni dep in the :hello
# target, then build the following. Copy the resulting header into
# src/main/android/jni, then restore the :jni dep and comment out the
# following. A ghastly hack, but it works.

# unfortunately the following only works for one file at a time. we're
# probably better of building the lib and running a shell script to
# run javah

# genrule(
#   name = "gen_headers",
# # outs must list one file per app jni class contained in the srcs jar(s)
#     outs = [#"org_example_hello_HelloApi.h"],
#           "jni/org_example_hello_Goodbye.h"],
#   srcs = [":hello_deploy.jar"],
#   cmd = """
# mkdir classes_tmp
# unzip -d classes_tmp $(location :hello_deploy.jar) > /dev/null
# pwd
# # run javah once per class
# #javah -classpath classes_tmp -o $@ org.example.hello.HelloApi
#   # javah -classpath classes_tmp -o "`pwd`src/main/android/org_example_hello_HelloApi.h" org.example.hello.GoodbyeApi
# javah -classpath classes_tmp -o $@ org.example.hello.GoodbyeApi
# # javah -classpath classes_tmp -o org_example_hello_GoodbyeApi org.example.hello.GoodbyeApi
# rm -rf classes_tmp
# """)

# this won't work either. it works in a shell script but not in bazel:

# genrule(
#   name = "gen_headers",
#     outs = [#"org_example_hello_HelloApi.h"],
#           "jni/org_example_hello_Goodbye.h"],
#   srcs = glob(["src/main/android/java/**/*.java"]),
#   cmd = "javah -cp src/main/android/java -d src/main/android/jni org.example.hello.HelloApi"
# )
