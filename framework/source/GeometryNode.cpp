#include <GeometryNode.hpp>

GeometryNode::GeometryNode() {}

model GeometryNode::getGeometry() const {
  return geometry_;
}

void GeometryNode::setGeometry(model const& geometry) {
  geometry_ = geometry;
}