#include <scenegraph_solar.hpp>
#include <Node.hpp>
#include <CameraNode.hpp>
#include <GeometryNode.hpp>

SceneGraph make_solar_scene() {
    SceneGraph s{};

    auto root = s.getRoot();

    auto camera = std::make_shared<CameraNode>(root, "Camera");
    root->addChild(camera);

    auto pointlight = std::make_shared<Node>(root, "PointLight");
    root->addChild(pointlight);

    return s;
}
