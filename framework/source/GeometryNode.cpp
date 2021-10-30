#include <GeometryNode.hpp>

/* constructor for GeometryNode */
GeometryNode::GeometryNode(std::shared_ptr<Node> parent,
                           std::string const& name,
                           std::string const& geometry_path) :
    Node{parent, name}, geometry_path_{geometry_path} {}

/* return the model of a geometry node */
model GeometryNode::getGeometry() const {
  return geometry_;
}

/* set the model of a geometry node */
void GeometryNode::setGeometry(model const& geometry) {
  geometry_ = geometry;
}

/* return the path of a geometry node */
std::string const& GeometryNode::getGeometryPath() const {
  return geometry_path_;
}
