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
        "        Sun Geometry\n"
        "    Mercury Holder\n"
        "        Mercury Geometry\n"
        "    Venus Holder\n"
        "        Venus Geometry\n"
        "    Earth Holder\n"
        "        Earth Geometry\n"
        "            Moon Holder\n"
        "                Moon Geometry\n"
        "    Mars Holder\n"
        "        Mars Geometry\n"
        "    Jupiter Holder\n"
        "        Jupiter Geometry\n"
        "    Saturn Holder\n"
        "        Saturn Geometry\n"
        "    Uranus Holder\n"
        "        Uranus Geometry\n"
        "    Neptune Holder\n"
        "        Neptune Geometry\n";

    auto solar = make_solar_scene();
    std::stringstream solar_print;
    solar_print << solar;

    REQUIRE(SCENEGRAPH_EXPECTED == solar_print.str());
}
