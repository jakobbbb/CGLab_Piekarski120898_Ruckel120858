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
                 std::string const& shader_name);
    model_object getGeometry() const;
    void setGeometry(model_object const& geometry);
    std::string getShaderName() const;
    texture_object getDiffuseTexture() const;
    void setDiffuseTexture(texture_object texture);
    texture_object getNormalTexture() const;
    void setNormalTexture(texture_object texture);

   private:
    model_object geometry_;
    std::string shader_name_;
    texture_object diffuse_texture_;
    texture_object normal_texture_;
};

#endif  // GEOMETRY_NODE_HPP
