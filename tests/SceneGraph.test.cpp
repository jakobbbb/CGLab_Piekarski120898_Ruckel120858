#include <catch.hpp>
#include <iostream>
#include <sstream>

#include <SceneGraph.hpp>
#include <scenegraph_solar.hpp>

TEST_CASE("scenegraph initialization") {
    SceneGraph scene_graph = SceneGraph::getInstance();
    auto root = scene_graph.getRoot();

    REQUIRE(nullptr != root);
    REQUIRE(root->getParent() == nullptr);
    REQUIRE(root->getName() == "root");
}

TEST_CASE("solar scenegraph") {
    const std::string SCENEGRAPH_EXPECTED =
        "root\n"
        "    Camera\n"
        "    Stars\n"
        "    PointLight\n"
        "        Sun Geometry\n"
        "    Mercury Orbit\n"
        "    Mercury Holder\n"
        "        Mercury Geometry\n"
        "    Venus Orbit\n"
        "    Venus Holder\n"
        "        Venus Geometry\n"
        "    Earth Orbit\n"
        "    Earth Holder\n"
        "        Earth Geometry\n"
        "            Moon Holder\n"
        "                Moon Geometry\n"
        "            Moon Orbit\n"
        "            Camera 2\n"
        "    Mars Orbit\n"
        "    Mars Holder\n"
        "        Mars Geometry\n"
        "    Jupiter Orbit\n"
        "    Jupiter Holder\n"
        "        Jupiter Geometry\n"
        "    Saturn Orbit\n"
        "    Saturn Holder\n"
        "        Saturn Geometry\n"
        "    Uranus Orbit\n"
        "    Uranus Holder\n"
        "        Uranus Geometry\n"
        "    Neptune Orbit\n"
        "    Neptune Holder\n"
        "        Neptune Geometry\n";

    make_solar_scene();
    std::stringstream solar_print;
    solar_print << SceneGraph::getInstance();

    REQUIRE(SCENEGRAPH_EXPECTED == solar_print.str());

    REQUIRE(SceneGraph::find("Camera") != nullptr);
    REQUIRE(SceneGraph::find("Neptune Geometry") != nullptr);
}
