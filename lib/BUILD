cc_library(
    name = "disfs_common",
    srcs = [
        "src/connection.c",
        "src/file_menager.c",
        "src/udp_discovery.c",
    ],
    hdrs = glob(["include/*.h"]),
    includes = ["include"],
    copts = [
    	  "-g3",
	  "-pedantic",
	  "-Werror",
	  "-Wall",
	  ],
    	  
    visibility=["//app:__pkg__"],
)

cc_test(
    name = "simple_test",
    srcs = ["tests/simple_test.c"],
    deps = ["//lib:disfs_common"],
    linkopts = ["-lcmocka"],
)

cc_test(
    name = "udp_packet_test",
    srcs = ["tests/udp_packet_test.c"],
    deps = ["//lib:disfs_common"],
    linkopts = ["-lcmocka"],
)