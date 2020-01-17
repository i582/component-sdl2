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