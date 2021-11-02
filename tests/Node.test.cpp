#include <catch.hpp>

#include <Node.hpp>

TEST_CASE("node initialization") {
    auto root = std::make_shared<Node>(nullptr, "root");
    REQUIRE(root->getName() == "root");
    REQUIRE(root->getDepth() == 0);

    SECTION("add one child") {
        auto child = std::make_shared<Node>(root, "child");
        root->addChild(child);

        REQUIRE(child->getDepth() == 1);
        REQUIRE(child->getParent() == root);
        REQUIRE(root->getChildrenList().size() == 1);
    }

    SECTION("remove one child") {
        auto child = std::make_shared<Node>(root, "child");
        auto nochild = std::make_shared<Node>(nullptr, "nochild");
        root->addChild(child);
        root->removeChildren(child->getName());

        REQUIRE(child->getDepth() == 0);
        REQUIRE(nochild->getDepth() == 0);
        REQUIRE(root->getChildrenList().size() == 0);
        REQUIRE_FALSE(root->getChildren(child->getName()));
        REQUIRE_FALSE(root->getChildren(nochild->getName()));
        REQUIRE_THROWS(root->removeChildren(child->getName()));
        REQUIRE_THROWS(root->removeChildren(nochild->getName()));
    }
}
