#ifndef GEOMETRY_NODE_HPP
#define GEOMETRY_NODE_HPP

#include <Node.hpp>
#include <model.hpp>

/**
 * TODO:  -model: geometry ✓
          +getGeometry(): model ✓
          +setGeometry(model): void ✓
*/

class GeometryNode : public Node {

  public:
    GeometryNode(std::shared_ptr<Node> parent,
        std::string const& name,
        std::string const& geometry_path);
    model getGeometry() const;
    void setGeometry(model const& geometry);
    std::string const& getGeometryPath() const;

  private:
    model geometry_;
    std::string geometry_path_;

};


#endif // GEOMETRY_NODE_HPP
