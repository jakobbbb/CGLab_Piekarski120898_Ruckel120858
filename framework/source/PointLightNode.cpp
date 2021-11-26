#include <PointLightNode.hpp>

PointLightNode::PointLightNode(std::shared_ptr<Node> parent, std::string name, 
                               Color const& lightColor, float lightIntensity)
    : Node{parent, name},
      lightColor_ {lightColor},
      lightIntensity_ {lightIntensity} {}

void PointLightNode::setLightColor(Color const& lightColor) {
    lightColor_ = lightColor;
}

void PointLightNode::setLightIntensity(float lightIntensity) {
    lightIntensity_ = lightIntensity;
}

float PointLightNode::getLightIntensity() const {
    return lightIntensity_;
}

Color PointLightNode::getLightColor() const {
    return lightColor_;
}
