#ifndef SCENE_GRAPH_HPP
#define SCENE_GRAPH_HPP
#include <Node.hpp>
#include <CameraNode.hpp>

/**
 * TODO:  -String: name ✓
          -Node: root ✓
          +getName(): String ✓
          -setName(String): void ✓
          +getRoot(): Node ✓
          -setRoot(Node): void ✓
          (+printGraph(): String)
          [singleton]
*/

class SceneGraph {

  public:
    SceneGraph();
    std::string getName() const;
    std::string printGraph() const;
    std::shared_ptr<Node> getRoot() const;
    std::ostream& print(std::ostream& os) const;
    void traverse(node_traverse_func);
    ~SceneGraph();
    static SceneGraph& getInstance();
    static std::shared_ptr<CameraNode> getActiveCamera();

  private:
    std::string name_;
    std::shared_ptr<Node> root_;
    void setName(std::string const& name);
    void setRoot(std::shared_ptr<Node> const& root);

};

std::ostream& operator<<(std::ostream& os, SceneGraph const& s);

#endif // SCENE_GRAPH_HPP
