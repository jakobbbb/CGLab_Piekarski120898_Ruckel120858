#include <GeometryNode.hpp>

/* constructor for GeometryNode */
GeometryNode::GeometryNode(std::shared_ptr<Node> parent,
                           std::string const& name):
    Node{parent, name} {}

/* return the model of a geometry node */
model GeometryNode::getGeometry() const {
  return geometry_;
}

/* set the model of a geometry node */
void GeometryNode::setGeometry(model const& geometry) {
  geometry_ = geometry;
}
