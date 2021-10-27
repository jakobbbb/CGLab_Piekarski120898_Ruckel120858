#include <Node.hpp>


Node::Node(std::shared_ptr<Node> parent, std::string const& name) :
    parent_{parent},
    name_{name},
    depth_{0} {}

std::string Node::getName() const {
  return name_;
}

std::string Node::getPath() const {
  return path_;
}

int Node::getDepth() const {
  return depth_;
}

glm::mat4 Node::getLocalTransform() const {
  return localTransform_;
}

glm::mat4 Node::getWorldTransform() const {
  return worldTransform_;
}

std::shared_ptr<Node> Node::getParent() const {
  return parent_;
}

std::shared_ptr<Node> Node::getChildren(std::string const& name) const {
  return 0; // TODO
}

std::shared_ptr<Node> Node::removeChildren(std::string const& name) const {
  return 0; // TODO
}

std::list<std::shared_ptr<Node>> Node::getChildrenList() const {
  return children_;
}

void Node::setParent(std::shared_ptr<Node> const& node) {
  parent_ = node;
}

void Node::setLocalTransform(glm::mat4 const& localTransform) {
  localTransform_ = localTransform;
}

void Node::setWorldTransform(glm::mat4 const& worldTransform) {
  worldTransform_ = worldTransform;
}

void Node::addChildren(std::shared_ptr<Node> const& node) {
  node->depth_ = getDepth() + 1;
  children_.push_back(node);
}

Node::~Node() {}
