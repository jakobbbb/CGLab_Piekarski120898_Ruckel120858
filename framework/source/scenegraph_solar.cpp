#include <cassert>

#include <scenegraph_solar.hpp>
#include <Node.hpp>
#include <CameraNode.hpp>
#include <GeometryNode.hpp>
#include <PointLightNode.hpp>

const std::string PLANET_NAMES[] = {
  "Mercury",
  "Venus",
  "Earth",
  "Mars",
  "Jupiter",
  "Saturn",
  "Uranus",
  "Neptune",
};

SceneGraph make_solar_scene() {
    SceneGraph s{};

    auto root = s.getRoot();

    auto camera = std::make_shared<CameraNode>(root, "Camera");
    root->addChild(camera);

    auto pointlight = std::make_shared<PointLightNode>(root, "PointLight");
    root->addChild(pointlight);
    auto sun_geom = std::make_shared<GeometryNode>(pointlight, "Sun Geometry");
    pointlight->addChild(sun_geom);

    for (auto const& planet_name : PLANET_NAMES) {
      auto holder = std::make_shared<Node>(root, planet_name + " Holder");
      root->addChild(holder);
      auto geom = std::make_shared<GeometryNode>(holder, planet_name + " Geometry");
      holder->addChild(geom);
    }
    auto earth = root->getChildren("Earth Holder");
    assert(earth != nullptr);

    auto moon_holder = std::make_shared<Node>(earth, "Moon Holder");
    earth->addChild(moon_holder);
    auto moon_geom = std::make_shared<GeometryNode>(moon_holder, "Moon Geometry");
    moon_holder->addChild(moon_geom);

    return s;
}
