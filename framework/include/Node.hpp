#ifndef NODE_HPP
#define NODE_HPP

#include <glm/gtc/matrix_transform.hpp>
#include <structs.hpp>
#include <string>
#include <memory>
#include <list>

class Node;
typedef std::function<void(std::shared_ptr<Node>)> node_traverse_func;

class Node {

  public:
    Node(std::shared_ptr<Node> parent, std::string const& name);
    std::string getName() const;
    std::string getPath() const;
    int getDepth() const;
    glm::mat4 getLocalTransform() const;
    glm::mat4 getWorldTransform() const;
    std::shared_ptr<Node> getParent() const;
    std::shared_ptr<Node> getChildren(std::string const& name) const;
    std::shared_ptr<Node> removeChildren(std::string const& name) const;
    std::list<std::shared_ptr<Node>> getChildrenList() const;
    void setParent(std::shared_ptr<Node> node);
    void setLocalTransform(glm::mat4 const& localTransform);
    void addChild(std::shared_ptr<Node> node);
    void print(std::ostream& os) const;
    void traverse(node_traverse_func);
    virtual ~Node();
    // Transforms
    void translate(glm::vec3 const& t);
    void rotate(float angle, glm::vec3 const& axis);
    void scale(float s);

  private:
    std::shared_ptr<Node> parent_;
    std::list<std::shared_ptr<Node>> children_;
    std::string name_;
    std::string path_;
    int depth_;
    glm::mat4 localTransform_;  // initialized to identiry matrix

};


#endif // NODE_HPP
