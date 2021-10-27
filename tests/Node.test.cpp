#include <catch.hpp>

#include <Node.hpp>

TEST_CASE("node initialization") {
    auto root = std::make_shared<Node>(nullptr, "root");
    REQUIRE(root->getName() == "root");
    REQUIRE(root->getDepth() == 0);

    SECTION("add one child") {
        auto child = std::make_shared<Node>(root, "child");
        root->addChildren(child);

        REQUIRE(child->getDepth() == 1);
        REQUIRE(child->getParent() == root);
    }
}
