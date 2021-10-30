#include <CameraNode.hpp>

/* constructor for CameraNode */
CameraNode::CameraNode(std::shared_ptr<Node> parent, std::string const& name) :
    Node{parent, name} {}

/* return true if perspective is given */
bool CameraNode::getPerspective() const { 
  return isPerspective_;
}

/* return true if camera is enabled */
bool CameraNode::getEnabled() const {
  return isEnabled_;
}

/* return the projection matrix */
glm::mat4 CameraNode::getProjectionMatrix() const {
  return projectionMatrix_;
}

/* set the camera as enabled */
void CameraNode::setEnabled(bool isEnabled) {
  isEnabled_ = isEnabled;
}

/* set the projection matrix for the camera */
void CameraNode::setProjectionMatrix(glm::mat4 const& projectionMatrix) {
    projectionMatrix_ = projectionMatrix;
}
