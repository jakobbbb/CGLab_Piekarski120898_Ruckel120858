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
    model_object getGeometry() const;
    void setGeometry(model_object const& geometry);

   private:
    model_object geometry_;
};

#endif  // GEOMETRY_NODE_HPP
