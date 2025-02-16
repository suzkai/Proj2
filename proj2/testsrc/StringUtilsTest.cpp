#include <gtest/gtest.h>
#include "StringUtils.h"

using namespace StringUtils;

TEST(StringUtilsTest, SliceTest){
    EXPECT_EQ(Slice("hello", 0, 2), "he");
    EXPECT_EQ(Slice("hello", -3, 0), "llo");
    EXPECT_EQ(Slice("hello", -5, -3), "he");
    EXPECT_EQ(Slice("hello", 0, 0), "hello");
    EXPECT_EQ(Slice("hello", 2, 10), "llo");
}

TEST(StringUtilsTest, Capitalize){
    EXPECT_EQ(Capitalize("hello"), "Hello");
    EXPECT_EQ(Capitalize("Hello"), "Hello");
    EXPECT_EQ(Capitalize("123abc"), "123abc");
    EXPECT_EQ(Capitalize(""), "");
}

TEST(StringUtilsTest, Upper){
    EXPECT_EQ(Upper("hello"), "HELLO");
    EXPECT_EQ(Upper("123abc"), "123ABC");
    EXPECT_EQ(Upper(""), "");
}

TEST(StringUtilsTest, Lower){
    EXPECT_EQ(Lower("HELLO"), "hello");
    EXPECT_EQ(Lower("123ABC"), "123abc");
    EXPECT_EQ(Lower(""), "");
}

TEST(StringUtilsTest, LStrip){
    EXPECT_EQ(LStrip("   hello"), "hello");
    EXPECT_EQ(LStrip("hello"), "hello");
    EXPECT_EQ(LStrip("   "), "");
    EXPECT_EQ(LStrip(""), "");
}

TEST(StringUtilsTest, RStrip){
    EXPECT_EQ(RStrip("hello   "), "hello");
    EXPECT_EQ(RStrip("hello"), "hello");
    EXPECT_EQ(RStrip("   "), "");
    EXPECT_EQ(RStrip(""), "");
}

TEST(StringUtilsTest, Strip){
    EXPECT_EQ(Strip("   hello   "), "hello");
    EXPECT_EQ(Strip("hello"), "hello");
    EXPECT_EQ(Strip("   "), "");
    EXPECT_EQ(Strip(""), "");
}

TEST(StringUtilsTest, Center){
    EXPECT_EQ(Center("hello", 9, ' '), "  hello  "); 
    EXPECT_EQ(Center("hello", 8, '*'), "*hello**");  
    EXPECT_EQ(Center("hello", 5, '-'), "hello"); 
    EXPECT_EQ(Center("", 4, '-'), "----");
}

TEST(StringUtilsTest, LJust){
    EXPECT_EQ(LJust("hello", 8, '-'), "hello---");
    EXPECT_EQ(LJust("hello", 5), "hello");
    EXPECT_EQ(LJust("", 4, '*'), "****");
}

TEST(StringUtilsTest, RJust){
    EXPECT_EQ(RJust("hello", 8, '-'), "---hello");
    EXPECT_EQ(RJust("hello", 5), "hello");
    EXPECT_EQ(RJust("", 4, '*'), "****");
}

TEST(StringUtilsTest, Replace){
    EXPECT_EQ(Replace("hello world", "world", "there"), "hello there");
    EXPECT_EQ(Replace("hello hello hello", "hello", "hi"), "hi hi hi");
    EXPECT_EQ(Replace("abcabc", "abc", "def"), "defdef");
    EXPECT_EQ(Replace("", "abc", "def"), "");
}

TEST(StringUtilsTest, Split){
    EXPECT_EQ(Split("hello world"), (std::vector<std::string>{"hello", "world"}));
    EXPECT_EQ(Split("hello,world", ","), (std::vector<std::string>{"hello", "world"}));
    EXPECT_EQ(Split("a,b,c", ","), (std::vector<std::string>{"a", "b", "c"}));
    EXPECT_EQ(Split("", ","), (std::vector<std::string>{""}));
}

TEST(StringUtilsTest, Join){
    EXPECT_EQ(Join(" ", {"hello", "world"}), "hello world");
    EXPECT_EQ(Join(",", {"a", "b", "c"}), "a,b,c");
    EXPECT_EQ(Join("-", {"one", "two"}), "one-two");
    EXPECT_EQ(Join(":", {}), "");
}

TEST(StringUtilsTest, ExpandTabs){
    EXPECT_EQ(ExpandTabs("hello\tworld"), "hello   world");
    EXPECT_EQ(ExpandTabs("a\tb\tc", 2), "a b c");
    EXPECT_EQ(ExpandTabs("", 4), "");
    EXPECT_EQ(ExpandTabs("\t", 4), "    ");
    EXPECT_EQ(ExpandTabs("123\t5678\n9\t12", 4), "123 5678\n9   12");
}


TEST(StringUtilsTest, EditDistance){
    EXPECT_EQ(EditDistance("kitten", "sitting"), 3);
    EXPECT_EQ(EditDistance("flaw", "lawn"), 2);
    EXPECT_EQ(EditDistance("abc", "abc"), 0);
    EXPECT_EQ(EditDistance("", "abc"), 3);
    EXPECT_EQ(EditDistance("hello", "HELLO", true), 0);
}