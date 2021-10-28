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

    SECTION("remove one child") {
        auto child = std::make_shared<Node>(root, "child");
        auto nochild = std::make_shared<Node>(nullptr, "nochild");
        REQUIRE(root->getDepth() == 0);
        root->addChildren(child);
        REQUIRE(root->getDepth() == 1);
        root->removeChildren(child->getName());
        REQUIRE(root->getDepth() == 0);
        root->removeChildren(nochild->getName());
        REQUIRE(root->getDepth() == 0);
        REQUIRE_FALSE(root->getChildren(nochild->getName()));
        REQUIRE_THROWS(root->removeChildren(nochild->getName()));
    }
}
