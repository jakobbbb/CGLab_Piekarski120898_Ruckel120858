#ifndef POINTLIGHT_NODE_HPP
#define POINTLIGHT_NODE_HPP

#include <Node.hpp>
#include <structs.hpp>

class PointLightNode : public Node {
    public:
     PointLightNode(std::shared_ptr<Node> parent, std::string name,
                    Color const& lightColor, float lightIntensity);    
     void setLightColor(Color const& lightColor);
     void setLightIntensity(float lightIntensity);
     float getLightIntensity() const;
     Color getLightColor() const;

    private:
     Color lightColor_;
     float lightIntensity_;
};

#endif  // POINTLIGHT_NODE_HPP
