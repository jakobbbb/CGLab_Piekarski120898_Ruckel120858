#include <catch.hpp>
#include <iostream>

#include <SceneGraph.hpp>
#include <scenegraph_solar.hpp>

TEST_CASE("scenegraph initialization") {
    SceneGraph scene_graph{};
    auto root = scene_graph.getRoot();

    REQUIRE(nullptr != root);
    REQUIRE(root->getParent() == nullptr);
    REQUIRE(root->getName() == "root");
}

TEST_CASE("solar scenegraph") {
    auto solar = make_solar_scene();
    std::cout << solar;
}
