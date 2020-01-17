#include <gtest/gtest.h>

#include "../src/tools/utils/utils.h"

using namespace Kit;

TEST(test_utils, test_utils__to_integer)
{
    const string test_string = "105";

    const int result = Utils::to_integer(test_string);

    EXPECT_EQ(result, 105);
}

TEST(test_utils, test_utils__to_double)
{
    const string test_string = "105.532";

    const double result = Utils::to_double(test_string);

    EXPECT_DOUBLE_EQ(result, 105.532);
}

TEST(test_utils, test_utils__split)
{
    const string test_string = "Hello My Little Pony!";

    const auto result = Utils::split(test_string, ' ');


    EXPECT_EQ(result->size(), 4);
    EXPECT_EQ(result->at(0), "Hello");
    EXPECT_EQ(result->at(1), "My");
    EXPECT_EQ(result->at(2), "Little");
    EXPECT_EQ(result->at(3), "Pony!");

    delete result;
}

TEST(test_utils, test_utils__to_lower)
{
    const string test_string = "Hello World!";

    const string result = Utils::to_lower(test_string);

    EXPECT_EQ(result, "hello world!");
}

TEST(test_utils, test_utils__to_upper)
{
    const string test_string = "Hello World!";

    const string result = Utils::to_upper(test_string);

    EXPECT_EQ(result, "HELLO WORLD!");
}

TEST(test_utils, test_utils__is_integer)
{
    const string test_string = "12";

    const auto result = Utils::is_integer(test_string);

    EXPECT_TRUE(result);

    const string test_string_2 = "12.1";

    const auto result_2 = Utils::is_integer(test_string_2);

    EXPECT_FALSE(result_2);
}

TEST(test_utils, test_utils__is_number)
{
    const string test_string = "12.21";

    const auto result = Utils::is_number(test_string);

    EXPECT_TRUE(result);


    const string test_string_2 = "12.21.12";

    const auto result_2 = Utils::is_number(test_string_2);

    EXPECT_FALSE(result_2);
}

TEST(test_utils, test_utils__max_of)
{
    const int val_1 = 100;
    const int val_2 = 56;
    const int val_3 = 0xfffff;
    const int val_4 = -100;

    const int result = Utils::max_of(val_1, val_2, val_3, val_4);

    EXPECT_EQ(result, 0xfffff);
}

TEST(test_utils, test_utils__min_of)
{
    const int val_1 = 100;
    const int val_2 = 56;
    const int val_3 = 0xfffff;
    const int val_4 = -100;

    const int result = Utils::min_of(val_1, val_2, val_3, val_4);

    EXPECT_EQ(result, -100);
}