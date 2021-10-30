#include <Node.hpp>
#include <iostream>


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
  for (auto const& child : children_) {
    if (child->name_ == name) {
      return child;
    } 
  } return nullptr;
}

std::shared_ptr<Node> Node::removeChildren(std::string const& name) {
  for (auto const& child : children_) {
    if (child->name_.compare(name) == 0) {
      auto parent = child->getParent();
      if (parent != nullptr) {
        child->depth_ = 0;
        parent->children_.remove(child);
      }
    } return child;
  } 
  throw std::invalid_argument("Node " + name + " does not exist"); 
  return nullptr;
}

std::list<std::shared_ptr<Node>> Node::getChildrenList() const {
  return children_;
}

void Node::setParent(std::shared_ptr<Node> node) {
  parent_ = node;
}

void Node::setLocalTransform(glm::mat4 const& localTransform) {
  localTransform_ = localTransform;
}

void Node::setWorldTransform(glm::mat4 const& worldTransform) {
  worldTransform_ = worldTransform;
}

void Node::addChild(std::shared_ptr<Node> node) {
  node->depth_ = getDepth() + 1;
  children_.push_back(node);
}

void Node::print(std::ostream& os) const {
    for (int i = 0; i < depth_; ++i) {
        os << "    ";
    }
    os << name_ << "\n";

    for (auto const& c : children_) {
        c->print(os);
    }
}

void Node::traverse(node_traverse_func func) {
    for (auto const& c : children_) {
      func(c);
    }
}
Node::~Node() {}
