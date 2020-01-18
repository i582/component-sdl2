#include <gtest/gtest.h>

#include "../src/component/component.h"

using namespace Kit;

TEST(test_component, test_component_children_by_id)
{
    auto comp = new Component("#test", ".test");

    auto comp_children = new Component("#children", ".children");

    comp->append(comp_children);

    auto return_children = comp->children("#children");

    EXPECT_EQ(return_children, comp_children);


    EXPECT_ANY_THROW(auto throw_children = comp->children("#some-id"););


    delete comp_children;
    delete comp;
}

TEST(test_component, test_component_children_by_number)
{
    auto comp = new Component("#test", ".test");

    auto comp_children = new Component("#children", ".children");

    comp->append(comp_children);

    auto return_children = comp->children(0);

    EXPECT_EQ(return_children, comp_children);


    EXPECT_ANY_THROW(auto throw_children = comp->children(10););


    delete comp_children;
    delete comp;
}


TEST(test_component, test_component_classes)
{
    auto comp = new Component("#test", ".test");

    // has
    bool result = comp->hasClass(".test");
    EXPECT_TRUE(result);


    // has
    result = comp->hasClass(".aaaa");
    EXPECT_FALSE(result);


    // add
    comp->addClass(".aaaa");

    result = comp->hasClass(".aaaa");
    EXPECT_TRUE(result);


    // remove
    comp->removeClass(".test");

    result = comp->hasClass(".test");
    EXPECT_FALSE(result);


    // toggle
    comp->toggleClass(".test");

    result = comp->hasClass(".test");
    EXPECT_TRUE(result);


    // toggle
    comp->toggleClass(".test");

    result = comp->hasClass(".test");
    EXPECT_FALSE(result);

    delete comp;
}

TEST(test_component, test_component_user_data)
{
    auto comp = new Component("#test", ".test");

    comp->addUserData("number", 123);

    auto result = comp->userData<int>("number");
    EXPECT_EQ(result, 123);


    result = std::any_cast<int>(comp->userData("number"));
    EXPECT_EQ(result, 123);


    EXPECT_ANY_THROW(comp->userData<string>("number"));

    EXPECT_ANY_THROW(comp->userData("some"));



    delete comp;
}