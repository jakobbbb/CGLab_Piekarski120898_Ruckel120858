#include <cassert>
#include <cstdlib>
#include <iostream>

#include <CameraNode.hpp>
#include <GeometryNode.hpp>
#include <Node.hpp>
#include <PointLightNode.hpp>
#include <scenegraph_solar.hpp>

void make_solar_scene() {
    SceneGraph s = SceneGraph::getInstance();

    auto root = s.getRoot();

    auto camera = std::make_shared<CameraNode>(root, "Camera");
    camera->setEnabled(true);
    root->addChild(camera);

    auto stars = std::make_shared<GeometryNode>(root, "Stars", "stars");
    root->addChild(stars);
    stars->scale(60.0f);  // spread out

    auto pointlight = std::make_shared<PointLightNode>(root, "PointLight");
    root->addChild(pointlight);
    auto sun_geom =
        std::make_shared<GeometryNode>(pointlight, "Sun Geometry", "planet");
    pointlight->addChild(sun_geom);

    float distance_to_sun = 10.0f;
    for (auto const& planet_name : PLANET_NAMES) {
        distance_to_sun += 4 + 6 * RAND_FLOAT();

        auto orbit = std::make_shared<GeometryNode>(
            root, planet_name + " Orbit", "orbit");
        root->addChild(orbit);
        orbit->scale(distance_to_sun);

        auto holder = std::make_shared<Node>(root, planet_name + " Holder");
        root->addChild(holder);
        auto geom = std::make_shared<GeometryNode>(
            holder, planet_name + " Geometry", "planet");
        holder->addChild(geom);

        holder->rotate(RAND_FLOAT(), SUN_AXIS);
        geom->translate({distance_to_sun, 0, 0});
        geom->scale(0.5f);
    }

    auto earth = root->getChildren("Earth Geometry");
    assert(earth != nullptr);

    auto moon_holder = std::make_shared<Node>(earth, "Moon Holder");
    earth->addChild(moon_holder);
    auto moon_geom =
        std::make_shared<GeometryNode>(moon_holder, "Moon Geometry", "planet");
    moon_holder->addChild(moon_geom);

    float moon_earth_distance = 10.f;
    moon_geom->scale(.5f);
    moon_geom->translate({moon_earth_distance, 0, 0});

    auto moon_orbit =
        std::make_shared<GeometryNode>(earth, "Moon Orbit", "orbit");
    earth->addChild(moon_orbit);
    moon_orbit->scale(moon_earth_distance * 0.5f);
}
