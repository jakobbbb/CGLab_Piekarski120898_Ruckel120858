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

    auto stars = std::make_shared<Node>(root, "Stars");
    root->addChild(stars);

    auto pointlight = std::make_shared<PointLightNode>(root, "PointLight");
    root->addChild(pointlight);
    auto sun_geom = std::make_shared<GeometryNode>(pointlight, "Sun Geometry");
    pointlight->addChild(sun_geom);

    // sun_geom->scale(0.1);

    float distance_to_sun = 10.0f;
    for (auto const& planet_name : PLANET_NAMES) {
        auto holder = std::make_shared<Node>(root, planet_name + " Holder");
        root->addChild(holder);
        auto geom =
            std::make_shared<GeometryNode>(holder, planet_name + " Geometry");
        holder->addChild(geom);

        holder->rotate(RAND_FLOAT(), SUN_AXIS);
        distance_to_sun += 2 + 2 * RAND_FLOAT();
        geom->translate({distance_to_sun, 0, 0});
        geom->scale(0.5f);
    }

    auto earth = root->getChildren("Earth Geometry");
    assert(earth != nullptr);

    auto moon_holder = std::make_shared<Node>(earth, "Moon Holder");
    earth->addChild(moon_holder);
    auto moon_geom =
        std::make_shared<GeometryNode>(moon_holder, "Moon Geometry");
    moon_holder->addChild(moon_geom);

    moon_geom->scale(.5f);
    moon_geom->translate({10.f, 0, 0});
}
