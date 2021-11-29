#ifndef CAMERA_NODE_HPP
#define CAMERA_NODE_HPP

#include <Node.hpp>

#define WORLD_UP glm::vec3{0, 1, 0}
#define WORLD_RIGHT glm::vec3{1, 0, 0}
#define WORLD_FORWARD glm::vec3{0, 0, -1}

/**
 * TODO:  -bool: isPerspective ✓
          -bool: isEnabled ✓
          -mat4: projectionMatrix ✓
          +getPerspective(): bool ✓
          +getEnabled(): bool ✓
          +setEnabled(bool): void ✓
          +getProjectionMatrix(): mat4 ✓
          +setProjectionMatrix(mat4): void ✓
*/

class CameraNode : public Node {
   public:
    CameraNode(std::shared_ptr<Node> parent, std::string const& name);
    bool getPerspective() const;
    bool getEnabled() const;
    glm::mat4 getProjectionMatrix() const;
    void setEnabled(bool isEnabled);
    void setProjectionMatrix(glm::mat4 const& projectionMatrix);
    glm::vec3 right() const;  // right direction, depending on orientation
    glm::vec3 forward() const;  // forward direction, depending on orientation
    void rotateCamera(float delta_pitch, float delta_yaw);
    glm::mat4 rotation_matrix() const;
    glm::mat4 getRotatedWorldTransform() const;

   private:
    bool isPerspective_;
    bool isEnabled_;
    glm::mat4 projectionMatrix_;
    float pitch_ = 0;  // rotation angle top/bottom
    float yaw_ = 0;  // rotation angle left/right
};

#endif  // CAMERA_NODE_HPP
