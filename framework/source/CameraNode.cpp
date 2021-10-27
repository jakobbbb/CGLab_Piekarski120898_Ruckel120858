#include <CameraNode.hpp>

CameraNode::CameraNode(std::shared_ptr<Node> parent, std::string const& name) :
    Node{parent, name} {}

bool CameraNode::getPerspective() const { 
  return isPerspective_;
}

bool CameraNode::getEnabled() const {
  return isEnabled_;
}

glm::mat4 CameraNode::getProjectionMatrix() const {
  return projectionMatrix_;
}

void CameraNode::setEnabled(bool isEnabled) {
  isEnabled_ = isEnabled;
}

void CameraNode::setProjectionMatrix(glm::mat4 const& projectionMatrix) {
    projectionMatrix_ = projectionMatrix;
}
