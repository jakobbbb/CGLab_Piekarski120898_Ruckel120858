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

    for (auto const& planet_name : PLANET_NAMES) {
      auto node = std::make_shared<GeometryNode>(root, planet_name);
      root->addChild(node);
    }
    auto earth = root->getChildren("Earth");
    assert(earth != nullptr);

    auto moon = std::make_shared<GeometryNode>(earth, "Moon");
    earth->addChild(moon);

    return s;
}
