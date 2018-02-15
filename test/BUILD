java_binary(
    name = "OCFTestServer",
    srcs = glob(["java/main/java/**/*.java"]),
    deps = ["@openocf_java//:OpenOCF"],
    resources = [":server_config"],
)

java_binary(
    name = "OCFTestClient",
    srcs = glob(["java/main/java/**/*.java"]),
    deps = ["@openocf_java//:OpenOCF"],
    resources = [":client_config"]
)

# prereq: edn-to-json and json2cbor are on the path (e.g. $HOME/bin)
genrule(
    name = "client_json",
    srcs = ["java/main/resources/client_owner_config.edn"],
    outs = ["client_config.json"],
    cmd = "edn-to-json $(location java/main/resources/client_owner_config.edn)  > \"$@\"",
)

genrule(
    name = "client_config",
    srcs = [":client_json"],
    outs = ["client_config.cbor"],
    cmd = "json2cbor $(location :client_json) > \"$@\"",
)

genrule(
    name = "server_json",
    srcs = ["java/main/resources/server_config.edn"],
    outs = ["server_config.json"],
    cmd = "edn-to-json $(location java/main/resources/server_config.edn)  > \"$@\"",
)

genrule(
    name = "server_config",
    srcs = [":server_json"],
    outs = ["server_config.cbor"],
    cmd = "json2cbor $(location :server_json) > \"$@\"",
)
