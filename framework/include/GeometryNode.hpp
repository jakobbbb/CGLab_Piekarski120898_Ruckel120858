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
    GeometryNode(std::shared_ptr<Node> parent, std::string const& name);
    model getGeometry() const;
    void setGeometry(model const& geometry);

   private:
    model geometry_;
};

#endif  // GEOMETRY_NODE_HPP
