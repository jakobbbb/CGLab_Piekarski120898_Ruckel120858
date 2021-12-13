#include <GeometryNode.hpp>

/* constructor for GeometryNode */
GeometryNode::GeometryNode(std::shared_ptr<Node> parent,
                           std::string const& name,
                           std::string const& shader_name)
    : Node{parent, name}, shader_name_{shader_name} {}

/* return the model of a geometry node */
model_object GeometryNode::getGeometry() const {
    return geometry_;
}

/* set the model of a geometry node */
void GeometryNode::setGeometry(model_object const& geometry) {
    geometry_ = geometry;
}

std::string GeometryNode::getShaderName() const {
    return shader_name_;
}

unsigned int GeometryNode::getTextureHandle() const {
    return texture_handle_;
}

void GeometryNode::setTextureHandle(unsigned int texture_handle) {
    texture_handle_ = texture_handle;
}
