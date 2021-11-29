#include <CameraNode.hpp>

#include <iostream>

/* constructor for CameraNode */
CameraNode::CameraNode(std::shared_ptr<Node> parent, std::string const& name)
    : Node{parent, name} {}

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

glm::vec3 CameraNode::right() const {
    auto world_right_4 = glm::vec4{WORLD_RIGHT, 0.0f};
    return glm::normalize(glm::vec3(world_right_4 * glm::inverse(rotation_matrix())));
}

glm::vec3 CameraNode::forward() const {
    auto world_forward_4 = glm::vec4{WORLD_FORWARD, 0.0f};
    return glm::normalize(glm::vec3(world_forward_4 * glm::inverse(rotation_matrix())));
}

void CameraNode::rotateCamera(float delta_pitch, float delta_yaw) {
    pitch_ = glm::clamp(pitch_ + delta_pitch, -(float)M_PI/2.f, (float)M_PI/2.f);
    yaw_ += delta_yaw;
}

glm::mat4 CameraNode::rotation_matrix() const {
    auto rot = glm::rotate(glm::mat4{}, yaw_, WORLD_UP);
    rot = glm::rotate(rot, pitch_, WORLD_RIGHT);
    return rot;
}

glm::mat4 CameraNode::getRotatedWorldTransform() const {
    return getWorldTransform() * rotation_matrix();
}
