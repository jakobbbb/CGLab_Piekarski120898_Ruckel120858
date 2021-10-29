#ifndef NODE_HPP
#define NODE_HPP

#include <glm/gtc/matrix_transform.hpp>
#include <structs.hpp>
#include <string>
#include <memory>
#include <list>

/**
 * TODO:  -Node: parent ✓
          -List<Node>: children ✓
          -String: name ✓
          -String: path ✓
          -int: depth ✓
          -mat4: localTransform ✓
          -mat4: worldTransform ✓
          +getParent(): Node ✓
          +setParent(Node): void ✓
          +getChildren(String): Node
          +getChildrenList(): List<Node> ✓
          +getName(): String ✓
          +getPath(): String ✓
          +getDepth(): int ✓
          +getLocalTransform(): mat4 ✓
          +setLocalTransform(mat4): void ✓
          +getWorldTransform(): mat4 ✓
          +setWorldTransform(mat4): void ✓
          +addChildren(Node): void ✓
          +removeChildren(String): Node
*/


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
    std::shared_ptr<Node> removeChildren(std::string const& name);
    std::list<std::shared_ptr<Node>> getChildrenList() const;
    void setParent(std::shared_ptr<Node> node);
    void setLocalTransform(glm::mat4 const& localTransform);
    void setWorldTransform(glm::mat4 const& worldTransform);
    void addChild(std::shared_ptr<Node> node);
    void print(std::ostream& os) const;
    void traverse(node_traverse_func);
    virtual ~Node();

  private:
    std::shared_ptr<Node> parent_;
    std::list<std::shared_ptr<Node>> children_;
    std::string name_;
    std::string path_;
    int depth_;
    glm::mat4 localTransform_;
    glm::mat4 worldTransform_;

};


#endif // NODE_HPP
