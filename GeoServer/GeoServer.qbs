import qbs 1.0

Project {
    minimumQbsVersion: "1.3"
    name: "GeoServer"

    CppApplication {
        files: [
            "main.cpp",
        ]
        Depends {name: "cpp"}

        property string gmockHome: qbs.getEnv("GMOCK_HOME")
        cpp.includePaths: ['.', gmockHome + '/include', gmockHome + '/gtest/include']
        cpp.libraryPaths: ['.', gmockHome + '/mybuild', gmockHome + '/gtest/mybuild']

        cpp.cxxFlags: ["-std=c++11" ,"-Wall"]

        cpp.dynamicLibraries: ["gmock", "gtest", "pthread"]
    }
}
