import qbs 1.0

Project {
    minimumQbsVersion: "1.3"
    name: "GeoServer"

    CppApplication {
        files: [
            "Area.cpp",
            "Area.h",
            "GeoServer.cpp",
            "GeoServer.h",
            "Location.cpp",
            "Location.h",
            "User.h",
            "VectorUtil.h",
            "Work.h",
            "ThreadPool.h",
        ]
        Depends {name: "cpp"}

        property string gmockHome: qbs.getEnv("GMOCK_HOME")
        cpp.includePaths: ['.', gmockHome + '/include', gmockHome + '/gtest/include']
        cpp.libraryPaths: ['.', gmockHome + '/mybuild', gmockHome + '/gtest/mybuild']

        cpp.cxxFlags: ["-std=c++11" ,"-Wall"]

        cpp.dynamicLibraries: ["gmock", "gtest", "pthread"]

        Group {
            name: "Tests"
            files: [
                "TestTimer.cpp",
                "TestTimer.h",
                "ThreadPoolTest.cpp",
                "WorkTest.cpp",
                "main.cpp",
                "AreaTest.cpp",
                "GeoServerTest.cpp",
                "LocationTest.cpp",
            ]
        }
    }
}
