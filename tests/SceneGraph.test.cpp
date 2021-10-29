#include <catch.hpp>
#include <iostream>
#include <sstream>

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
    const std::string SCENEGRAPH_EXPECTED =
        "root\n"
        "    Camera\n"
        "    PointLight\n"
        "    Mercury\n"
        "    Venus\n"
        "    Earth\n"
        "        Moon\n"
        "    Mars\n"
        "    Jupiter\n"
        "    Saturn\n"
        "    Uranus\n"
        "    Neptune\n";

    auto solar = make_solar_scene();
    std::stringstream solar_print;
    solar_print << solar;

    REQUIRE(SCENEGRAPH_EXPECTED == solar_print.str());
}
