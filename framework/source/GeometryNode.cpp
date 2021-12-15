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

texture_object GeometryNode::getDiffuseTexture() const {
    return diffuse_texture_;
}

void GeometryNode::setDiffuseTexture(texture_object texture) {
    diffuse_texture_ = texture;
}

texture_object GeometryNode::getNormalTexture() const {
    return normal_texture_;
}

void GeometryNode::setNormalTexture(texture_object texture) {
    normal_texture_ = texture;
}
