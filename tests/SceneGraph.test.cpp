#include <catch.hpp>

#include <SceneGraph.hpp>

TEST_CASE("scenegraph initialization") {
    SceneGraph scene_graph{};
    auto root = scene_graph.getRoot();

    REQUIRE(nullptr != root);
    REQUIRE(root->getParent() == nullptr);
    REQUIRE(root->getName() == "root");
}
