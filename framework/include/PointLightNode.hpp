#ifndef POINTLIGHT_NODE_HPP
#define POINTLIGHT_NODE_HPP

#include <Node.hpp>

class PointLightNode : public Node {
  public:
    PointLightNode(std::shared_ptr<Node> parent, std::string name);
};


#endif  // POINTLIGHT_NODE_HPP
