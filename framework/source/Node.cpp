#include <Node.hpp>
#include <iostream>

/* constructor for Node */
Node::Node(std::shared_ptr<Node> parent, std::string const& name) :
    parent_{parent},
    name_{name},
    depth_{0} {}

/* return the name of a node */
std::string Node::getName() const {
  return name_;
}

/* return the path of a node */
std::string Node::getPath() const {
  return path_;
}

/* return the depth of a node */
int Node::getDepth() const {
  return depth_;
}

/* return localTransform_ of a node */
glm::mat4 Node::getLocalTransform() const {
  return localTransform_;
}

/* return worldTransform_ of a node */
glm::mat4 Node::getWorldTransform() const {
  return worldTransform_;
}

/* return the parent node */
std::shared_ptr<Node> Node::getParent() const {
  return parent_;
}

/* return a child node */
std::shared_ptr<Node> Node::getChildren(std::string const& name) const {
  for (auto const& child : children_) {
    if (child->name_ == name) {
      return child;
    } 
  } return nullptr;
}

/* remove a child node from the children list */
std::shared_ptr<Node> Node::removeChildren(std::string const& name) const {
  for (auto const& child : children_) {
    if (child->name_.compare(name) == 0) {
      auto parent = child->getParent();
      if (parent != nullptr) {
        child->depth_ = 0;
        parent->children_.remove(child);
      }
      return child;
    } 
  } 
  throw std::invalid_argument("Node " + name + " does not exist"); 
  return nullptr;
}

/* return a list of all children */
std::list<std::shared_ptr<Node>> Node::getChildrenList() const {
  return children_;
}

/* set a node as parent node */
void Node::setParent(std::shared_ptr<Node> node) {
  parent_ = node;
}

/* set localTransform_ of a node */
void Node::setLocalTransform(glm::mat4 const& localTransform) {
  localTransform_ = localTransform;
}

/* set worldTransform_ of a node */
void Node::setWorldTransform(glm::mat4 const& worldTransform) {
  worldTransform_ = worldTransform;
}

/* add a child node at the end of the list */
void Node::addChild(std::shared_ptr<Node> node) {
  node->depth_ = getDepth() + 1;
  children_.push_back(node);
}

/* print the complete graph */
void Node::print(std::ostream& os) const {
    for (int i = 0; i < depth_; ++i) {
        os << "    ";
    }
    os << name_ << "\n";

    for (auto const& c : children_) {
        c->print(os);
    }
}

/* store traverse function for children nodes */
void Node::traverse(node_traverse_func func) {
    for (auto const& c : children_) {
      func(c);
    }
}

/* destructor for Node */
Node::~Node() {}
