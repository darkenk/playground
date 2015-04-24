#include "gtest/gtest.h"
#include "../parser.hpp"
#include <sstream>

using namespace std;

TEST(ParserTest, parseFileName)
{
    istringstream input{"gcc main.c -o a.out"};
    Parser p{input};
    p.run();
    EXPECT_STREQ("[main.c]", p.result().at(0).mName.c_str());
}

TEST(ParserTest, isCompilerLine)
{
    istringstream input{"gcc -DMajonez main.c -o a.out"};
    Parser p{input};
    p.run();
    EXPECT_EQ(1, p.result().size()) << ::testing::PrintToString(p.result());
}

TEST(ParserTest, findOneDefine)
{
    istringstream input{"gcc -DMajonez main.c -o a.out"};
    Parser p{input};
    p.run();
    EXPECT_STREQ("#define Majonez", p.result().at(0).mDefines.c_str()) << ::testing::PrintToString(p.result());
}

TEST(ParserTest, findTwoDefines)
{
    istringstream input{"gcc -DMajonez -DKrzaki main.c -o a.out"};
    Parser p{input};
    p.run();
    EXPECT_STREQ("#define Majonez\n#define Krzaki", p.result()[0].mDefines.c_str()) << ::testing::PrintToString(p.result());
}

TEST(ParserTest, findDefineEquals)
{
    istringstream input{"gcc -DKrzaki=12 main.c -o a.out"};
    Parser p{input};
    p.run();
    EXPECT_STREQ("#define Krzaki 12", p.result()[0].mDefines.c_str()) << ::testing::PrintToString(p.result());
}

TEST(ParserTest, findInclude)
{
    istringstream input{"gcc -I external/gtest/include main.c -o a.out"};
    Parser p{input};
    p.run();
    EXPECT_STREQ("external/gtest/include", p.result().at(0).mIncludes.c_str()) << ::testing::PrintToString(p.result());
}

TEST(ParserTest, findSystemInclude)
{
    istringstream input{"gcc -isystem external/gtest/include main.c -o a.out"};
    Parser p{input};
    p.run();
    EXPECT_STREQ("external/gtest/include", p.result().at(0).mIncludes.c_str()) << ::testing::PrintToString(p.result());
}

TEST(ParserTest, findBothIncludes)
{
    istringstream input{"gcc -isystem external/gtest/include -I external/libcxx/include main.c -o a.out"};
    Parser p{input};
    p.run();
    EXPECT_STREQ("external/gtest/include\nexternal/libcxx/include", p.result().at(0).mIncludes.c_str()) << ::testing::PrintToString(p.result());
}

TEST(ParserTest, realExample)
{
    istringstream input{"prebuilts/clang/linux-x86/host/3.5/bin/clang++ -I external/libcxx/include -I external/gtest/include -I external/valgrind/main/include -I external/valgrind/main -I external/vixl/src -I external/zlib -I frameworks/compile/mclinker/include -I art/sigchainlib -I art/runtime -I out/host/linux-x86/obj/SHARED_LIBRARIES/libart_intermediates -I out/host/linux-x86/gen/SHARED_LIBRARIES/libart_intermediates -I libnativehelper/include/nativehelper  -isystem system/core/include -isystem hardware/libhardware/include -isystem hardware/libhardware_legacy/include -isystem hardware/ril/include -isystem libnativehelper/include -isystem frameworks/native/include -isystem frameworks/native/opengl/include -isystem frameworks/av/include -isystem frameworks/base/include -isystem tools/include -isystem out/host/linux-x86/obj/include -isystem prebuilts/clang/linux-x86/host/3.5/lib/clang/3.5/include/ -c    -fno-exceptions -Wno-multichar -m64 -Wa,--noexecstack -fPIC -no-canonical-prefixes -include build/core/combo/include/arch/linux-x86/AndroidConfig.h -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=0 -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS -DANDROID -fmessage-length=0 -W -Wall -Wno-unused -Winit-self -Wpointer-arith -O2 -g -fno-strict-aliasing -DNDEBUG -UDEBUG  -D__compiler_offsetof=__builtin_offsetof -Werror=int-conversion   --gcc-toolchain=prebuilts/gcc/linux-x86/host/x86_64-linux-glibc2.11-4.6/ -no-integrated-as    --gcc-toolchain=prebuilts/gcc/linux-x86/host/x86_64-linux-glibc2.11-4.6/ --sysroot=prebuilts/gcc/linux-x86/host/x86_64-linux-glibc2.11-4.6//sysroot -no-integrated-as -target x86_64-linux-gnu   -DANDROID -fmessage-length=0 -W -Wall -Wno-unused -Winit-self -Wpointer-arith -Wsign-promo -DNDEBUG -UDEBUG    --gcc-toolchain=prebuilts/gcc/linux-x86/host/x86_64-linux-glibc2.11-4.6/ --sysroot=prebuilts/gcc/linux-x86/host/x86_64-linux-glibc2.11-4.6//sysroot -isystem prebuilts/gcc/linux-x86/host/x86_64-linux-glibc2.11-4.6//x86_64-linux/include/c++/4.6 -isystem prebuilts/gcc/linux-x86/host/x86_64-linux-glibc2.11-4.6//x86_64-linux/include/c++/4.6/x86_64-linux -isystem prebuilts/gcc/linux-x86/host/x86_64-linux-glibc2.11-4.6//x86_64-linux/include/c++/4.6/backward -no-integrated-as -target x86_64-linux-gnu    -DUSE_JEMALLOC -DGC_DUMP -fno-rtti -std=gnu++11 -ggdb3 -Wall -Werror -Wextra -Wno-sign-promo -Wno-unused-parameter -Wstrict-aliasing -fstrict-aliasing -fvisibility=protected -DANDROID_SMP=1 -DART_BASE_ADDRESS=0x60000000 -DART_DEFAULT_INSTRUCTION_SET_FEATURES=default -DART_DEFAULT_GC_TYPE_IS_CMS -DART_BASE_ADDRESS_MIN_DELTA=-0x1000000 -DART_BASE_ADDRESS_MAX_DELTA=0x1000000 -Wthread-safety -O3 -Wframe-larger-than=2600 -D_USING_LIBCXX -fPIC   -nostdinc++  -MD -MF out/host/linux-x86/obj/SHARED_LIBRARIES/libart_intermediates/thread_list.d -o out/host/linux-x86/obj/SHARED_LIBRARIES/libart_intermediates/thread_list.o art/runtime/thread_list.cc"};
    Parser p{input};
    p.run();
    EXPECT_EQ(p.result().size(), 1) << ::testing::PrintToString(p.result());
    File f = p.result().at(0);
    EXPECT_STREQ("[art/runtime/thread_list.cc]", f.mName.c_str());
    EXPECT_STREQ("#define _FORTIFY_SOURCE 0\n#define __STDC_FORMAT_MACROS\n#define __STDC_CONSTANT_MACROS\n#define ANDROID\n#define NDEBUG\n#define __compiler_offsetof __builtin_offsetof\n#define ANDROID\n#define NDEBUG\n#define USE_JEMALLOC\n#define GC_DUMP\n#define ANDROID_SMP 1\n#define ART_BASE_ADDRESS 0x60000000\n#define ART_DEFAULT_INSTRUCTION_SET_FEATURES default\n#define ART_DEFAULT_GC_TYPE_IS_CMS\n#define ART_BASE_ADDRESS_MIN_DELTA -0x1000000\n#define ART_BASE_ADDRESS_MAX_DELTA 0x1000000\n#define _USING_LIBCXX", f.mDefines.c_str());
    EXPECT_STREQ("external/libcxx/include\nexternal/gtest/include\nexternal/valgrind/main/include\nexternal/valgrind/main\nexternal/vixl/src\nexternal/zlib\nframeworks/compile/mclinker/include\nart/sigchainlib\nart/runtime\nout/host/linux-x86/obj/SHARED_LIBRARIES/libart_intermediates\nout/host/linux-x86/gen/SHARED_LIBRARIES/libart_intermediates\nlibnativehelper/include/nativehelper\nsystem/core/include\nhardware/libhardware/include\nhardware/libhardware_legacy/include\nhardware/ril/include\nlibnativehelper/include\nframeworks/native/include\nframeworks/native/opengl/include\nframeworks/av/include\nframeworks/base/include\ntools/include\nout/host/linux-x86/obj/include\nprebuilts/clang/linux-x86/host/3.5/lib/clang/3.5/include/\nprebuilts/gcc/linux-x86/host/x86_64-linux-glibc2.11-4.6//x86_64-linux/include/c++/4.6\nprebuilts/gcc/linux-x86/host/x86_64-linux-glibc2.11-4.6//x86_64-linux/include/c++/4.6/x86_64-linux\nprebuilts/gcc/linux-x86/host/x86_64-linux-glibc2.11-4.6//x86_64-linux/include/c++/4.6/backward", f.mIncludes.c_str());
}

TEST(ParserTest, qtConfigFile)
{
    istringstream input{"gcc -DMajonez -DKrzaki main.c -o a.out"};
    Parser p{input};
    p.run();
    EXPECT_STREQ("[main.c]\n#define Majonez\n#define Krzaki\n", p.generateQtCreatorConfigFileContent().c_str()) << ::testing::PrintToString(p.generateQtCreatorConfigFileContent());
}

TEST(ParserTest, qtIncludeFile)
{
    istringstream input{"gcc -isystem external/gtest/include -I external/libcxx/include main.c -o a.out"};
    Parser p{input};
    p.run();
    EXPECT_STREQ("[main.c]\nexternal/gtest/include\nexternal/libcxx/include\n", p.generateQtCreatorIncludeFileContent().c_str()) << ::testing::PrintToString(p.generateQtCreatorIncludeFileContent());
}
