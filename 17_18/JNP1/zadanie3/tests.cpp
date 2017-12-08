// Do uruchomienia potrzebna jest biblioteka Google Test (na Linuxie z dużym prawdopodobieństwem jest w repozytoriach;
// (Niektóre testują rzeczy uznane jako nieścisłości, np. TEST(TestyKontrolera, Wypisywanie))
// Konfiguracja w CMakeLists.txt:
// cmake_minimum_required(VERSION 3.8)
// project(JNP1_Zadanie3)
//
// set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
// set(CMAKE_CXX_STANDARD 17)
//
// set(COMMON_SOURCE_FILES sejf.h sejf.cc)
//
// # Tests
// enable_testing()
// find_package(GTest REQUIRED)
// find_package(Threads REQUIRED)
// include_directories(${GTEST_INCLUDE_DIRS})
// add_executable(GTest gtest_main.cc ${COMMON_SOURCE_FILES})
// target_link_libraries(GTest ${CMAKE_THREAD_LIBS_INIT} ${GTEST_BOTH_LIBRARIES})


#include <gtest/gtest.h>
#include <type_traits>
#include <iostream>
#include "sejf.h"

TEST(TestyZTresciZadania, TestZwracaniaDostepu)
{
    Sejf s("a\xff", 3);
    ASSERT_EQ(s[0], 'a');
    ASSERT_EQ(s[1], 255);
    ASSERT_EQ(s[1], static_cast<int>(static_cast<unsigned char>(-1)));
    ASSERT_EQ(s[1], -1);
    ASSERT_NE(s[1], static_cast<int>(static_cast<unsigned char>(-1)));
}


TEST(TestyZTresciZadania, WypisywanieKontrolera)
{
    Sejf s1("aaa", 2);
    s1[2];
    auto k1 = s1.kontroler();
    std::stringstream out;

    out << k1 << "test\n";
    s1[2];
    s1[3];
    s1[4];
    out << k1;
    s1[3];
    out << k1;
    s1[2];
    out << k1;

    ASSERT_EQ(out.str(), "OK\n"
            "test\n"
            "OK\n"
            "OK\n"
            "ALARM: WLAMANIE\n");
}



TEST(TestyManipulacji, Dodawanie)
{
    Sejf s("abc", 0);
    ASSERT_EQ(s[0], -1);
    s += 2;
    ASSERT_EQ(s[0], 'a');
    s += -1;
    ASSERT_EQ(s[0], 'a');
    ASSERT_EQ(s[0], -1);
}


TEST(TestyManipulacji, Mnozenie)
{
    Sejf s("abc", 1);
    s *= 3;
    ASSERT_EQ(s[0], 'a');
    ASSERT_EQ(s[0], 'a');
    ASSERT_EQ(s[0], 'a');
    ASSERT_EQ(s[0], -1);
    constexpr auto halfInt = std::numeric_limits<unsigned int>::digits / 2 + 1;
    std::cout << (1 << halfInt) << "\n";
    s += (1 << halfInt);
    s*=1;
    ASSERT_EQ(s[0], 'a');
    s += 1;
    s *= (1 << halfInt);
    ASSERT_EQ(s[0], 'a');
    s *= 0;
    for (unsigned int i = 0; i < (1 << halfInt) - 1; ++i)
        ASSERT_EQ(s[0], 'a');
    ASSERT_EQ(s[0], -1);
}


TEST(TestyManipulacji, Odejmowanie)
{
    Sejf s("abc", 3);
    ASSERT_EQ(s[0], 'a');
    s -= 1;
    ASSERT_EQ(s[0], 'a');
    ASSERT_EQ(s[0], -1);

    ASSERT_EQ(s[0], -1);
    s -= 1;
    s += 1;
    ASSERT_EQ(s[0], 'a');
    ASSERT_EQ(s[0], -1);
    s -= -1;
    ASSERT_EQ(s[0], -1);
    s -= std::numeric_limits<unsigned int>::max();
    ASSERT_EQ(s[0], -1);

    Sejf s2("...", 1);
    s2 -= 2;
    ASSERT_EQ(s2[0], '.');
    ASSERT_EQ(s2[0], -1);
}


TEST(TestyManipulacji, ZwracanyTyp)
{
    //Test z Facebooka

    Sejf s1("aaa", 2);
    ASSERT_EQ(s1[0], 'a');
    (s1 *= 3) -= 1;
    ASSERT_EQ(s1[0], 'a');
    ASSERT_EQ(s1[0], 'a');
    ASSERT_EQ(s1[0], -1);
}


TEST(TestyKontrolera, TestowanieDostepow)
{
    Sejf s("To jest złoto!", 2);
    ASSERT_TRUE(s.kontroler());
    s[2];
    ASSERT_TRUE(s.kontroler());
    s[42];
    ASSERT_TRUE(s.kontroler());
    s[0];
    ASSERT_FALSE(s.kontroler());

    s += 2;
    ASSERT_TRUE(s.kontroler());
    s *= 3;
    ASSERT_TRUE(s.kontroler());
    s -= 2;
    for (unsigned i = 0; i < 4; ++ i) {
        ASSERT_TRUE(s.kontroler());
        s[0];
    }
    ASSERT_FALSE(s.kontroler());
}


TEST(TestyKontrolera, Wypisywanie)
{
    const std::string ok = "OK\n";
    const std::string wlam = "ALARM: WLAMANIE\n";
    const std::string manip = "ALARM: ZMANIPULOWANY\n";

    Sejf s("nadsiarczek żelaza(II)", 0);

    std::stringstream sos;
    sos << s.kontroler();
    ASSERT_EQ(sos.str(), ok);

    sos.str("");
    s[42];
    sos << s.kontroler();
    s[0];
    sos << s.kontroler();
    ASSERT_EQ(sos.str(), ok + wlam);

    sos.str("");
    s -= 1;
    sos << s.kontroler();
    ASSERT_EQ(sos.str(), wlam);

    sos.str("");
    s += 1;
    sos << s.kontroler();
    ASSERT_TRUE(sos.str() == wlam + manip || sos.str() == manip + wlam);

    sos.str("");
    Sejf s2("kapibara", 1);
    s2[42];
    ASSERT_TRUE(s.kontroler());
    sos << s2.kontroler();
    ASSERT_EQ(sos.str(), ok);
}


TEST(TestyKontrolera, ManipulacjeIdentycznosciowe)
{
    const std::string ok = "OK\n";
    const std::string manip = "ALARM: ZMANIPULOWANY\n";

    Sejf s1("a", 1);
    Sejf s2("a", 1);
    Sejf s3("a", 1);
    std::stringstream sos;

    sos << s1.kontroler();
    s1 += 0;
    sos << s1.kontroler();
    ASSERT_EQ(sos.str(), ok + manip);

    sos.str("");
    sos << s2.kontroler();
    s2 -= 0;
    sos << s2.kontroler();
    ASSERT_EQ(sos.str(), ok + manip);

    sos.str("");
    sos << s3.kontroler();
    s3 *= 1;
    sos << s3.kontroler();
    ASSERT_EQ(sos.str(), ok + manip);
}


TEST(TestyKontrolera, Przypisywanie)
{
    Sejf s("xD", 1);

    auto k = s.kontroler();
    ASSERT_TRUE(k);
    s[0];
    ASSERT_FALSE(k);
}


TEST(TestyKontrolera, PrzenoszenieSejfu)
{
    // Na podstawie Moodla:
    // https://moodle.mimuw.edu.pl/mod/forum/discuss.php?d=536#p1616

    Sejf s1("zawartosc", 1);
    Sejf s2("zawartosc", 0);
    auto k = s1.kontroler();

    ASSERT_TRUE(k);
    std::swap(s1, s2);
    ASSERT_FALSE(k);
}


TEST(TestyKompilacji, KopiowanieIPrzenoszenie)
{
    ASSERT_FALSE(std::is_copy_constructible<Sejf>());
    ASSERT_FALSE(std::is_copy_assignable<Sejf>());
    ASSERT_TRUE(std::is_move_constructible<Sejf>());
    ASSERT_TRUE(std::is_move_assignable<Sejf>());

    ASSERT_FALSE(std::is_copy_constructible<Kontroler>());
    ASSERT_FALSE(std::is_copy_assignable<Kontroler>());
    ASSERT_TRUE(std::is_move_constructible<Kontroler>());
}


TEST(TestyKompilacji, OperatorPrzypisania)
{
    Sejf s("", 0);
    ASSERT_FALSE(static_cast<bool>(std::is_assignable<decltype(s[0]), char>()));
}


TEST(Moje, Overflow1)
{
    Sejf s("abc", 0);
    ASSERT_EQ(s[0], -1);
    s += 2;
    std::cout << s[0];
    ASSERT_EQ(s[0], 'a');
    s += -1;
    ASSERT_EQ(s[0], -1);
    ASSERT_EQ(s[0], -1);
}


int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
