#ifndef CAMERA_NODE_HPP
#define CAMERA_NODE_HPP

#include <Node.hpp>

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

   private:
    bool isPerspective_;
    bool isEnabled_;
    glm::mat4 projectionMatrix_;
};

#endif  // CAMERA_NODE_HPP
