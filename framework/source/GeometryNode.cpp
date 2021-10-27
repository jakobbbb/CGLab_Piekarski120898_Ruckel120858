#include <GeometryNode.hpp>

GeometryNode::GeometryNode(std::shared_ptr<Node> parent,
                           std::string const& name) :
    Node{parent, name} {}

model GeometryNode::getGeometry() const {
  return geometry_;
}

void GeometryNode::setGeometry(model const& geometry) {
  geometry_ = geometry;
}
