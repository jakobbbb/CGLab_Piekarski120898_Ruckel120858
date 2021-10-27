#ifndef SCENE_GRAPH_HPP
#define SCENE_GRAPH_HPP

/**
 * TODO:  -String: name
          -Node: root
          +getName(): String
          -setName(String): void
          +getRoot(): Node
          -setRoot(Node): void
          (+printGraph(): String)
          [singleton]
*/

class SceneGraph {

  public:
    SceneGraph();
    std::string getName() const;
    std::string printGraph() const;
    std::shared_ptr<Node> getRoot() const;
    ~SceneGraph();

  private:
    std::string name_;
    std::shared_ptr<Node> root_;
    void setName(std::string const& name);
    void setRoot(std::shared_ptr<Node> const& root);

};


#endif // SCENE_GRAPH_HPP