#include <GeometryNode.hpp>

GeometryNode::GeometryNode(std::shared_ptr<Node> parent,
                           std::string const& name,
                           std::string const& geometry_path) :
    Node{parent, name}, geometry_path_{geometry_path} {}

model GeometryNode::getGeometry() const {
  return geometry_;
}

void GeometryNode::setGeometry(model const& geometry) {
  geometry_ = geometry;
}

std::string const& GeometryNode::getGeometryPath() const {
  return geometry_path_;
}
