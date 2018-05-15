# uberjar target:  Server:Server_deploy.jar
java_binary(
    name = "Server", # matches java file containing main routine
    # main_class = "OCFTestServer.class",
    srcs = glob(["src/main/**/*.java"]),
    deps = ["//:OpenOCFJava",
            "//demos/stdjava/common"],
    # OpenOCFJ expects to find svrs.cbor at root, so we need:
    classpath_resources = [":server_config"],
)

genrule(
    name = "server_json",
    srcs = ["src/resources/server_config.edn"],
    outs = ["server_config.json"],
    cmd = "edn-to-json $(location src/resources/server_config.edn)  > \"$@\"",
)

# FIXME: make this dependent on json2cbor too
genrule(
    name = "server_config",
    srcs = [":server_json"],
    outs = ["svrs.cbor"],
    cmd = "json2cbor $(location :server_json) > \"$@\"",
)
