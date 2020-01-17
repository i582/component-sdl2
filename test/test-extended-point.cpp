#include <gtest/gtest.h>

#include "../src/tools/rect/extended-rect/extended-rect.h"

using namespace Kit;

TEST(test_extended_point, test_extended_point__simple_constructor)
{
    const Point point(10, 20);

    int x = point.x();
    int y = point.y();

    EXPECT_EQ(x, 10);
    EXPECT_EQ(y , 20);

    const SDL_Point sdl_point = {100, 200};
    const Point point_1(sdl_point);

    x = point_1.x();
    y = point_1.y();

    EXPECT_EQ(x, 100);
    EXPECT_EQ(y , 200);
}

TEST(test_extended_point, test_extended_point__operators)
{
    const Point p1(10, 20);
    const Point p2(5, 25);

    Point result = p1 + p2;

    EXPECT_EQ(result.x(), 15);
    EXPECT_EQ(result.y(), 45);


    result = p1 - p2;

    EXPECT_EQ(result.x(), 5);
    EXPECT_EQ(result.y(), -5);

    const int scale = 10;
    result = p1 * scale;

    EXPECT_EQ(result.x(), 100);
    EXPECT_EQ(result.y(), 200);


    const double double_scale = 1.5;
    result = p1 * double_scale;

    EXPECT_EQ(result.x(), 15);
    EXPECT_EQ(result.y(), 30);



    const int delim = 5;
    result = p1 / delim;

    EXPECT_EQ(result.x(), 2);
    EXPECT_EQ(result.y(), 4);


    const double double_delim = 2.5;
    result = p1 / double_delim;

    EXPECT_EQ(result.x(), 4);
    EXPECT_EQ(result.y(), 8);


    const Point p3(5, 85);
    const Point p4(5, 85);

    bool result_bool = p3 == p4;

    EXPECT_TRUE(result_bool);



    const Point p5(5, 85);
    const Point p6(5, 75);

    result_bool = p5 != p6;

    EXPECT_TRUE(result_bool);


    Point p7;
    p7 = p6;

    EXPECT_EQ(p7.x(), p6.x());
    EXPECT_EQ(p7.y(), p6.y());
}

TEST(test_extended_point, test_extended_point__value_from_string)
{
    Point p1("100px", "50px");

    p1.calc(Rect(0, 0, 200, 300));

    EXPECT_EQ(p1.x(), 100);
    EXPECT_EQ(p1.y(), 50);




    Point p2("20%", "50%");

    p2.calc(Rect(0, 0, 200, 300));

    EXPECT_EQ(p2.x(), 40);
    EXPECT_EQ(p2.y(), 150);




    Point p3("20% + 100px", "50% - 100px");

    p3.calc(Rect(0, 0, 200, 300));

    EXPECT_EQ(p3.x(), 140);
    EXPECT_EQ(p3.y(), 50);
}

TEST(test_extended_point, test_extended_point__in)
{
    const Point p1(100, 100);
    const Rect r1(50, 50, 200, 200);

    bool result = p1.in(r1);

    EXPECT_TRUE(result);



    const Rect r2(10, 10, 10, 10);

    result = p1.in(r2);

    EXPECT_FALSE(result);
}