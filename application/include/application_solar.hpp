#ifndef APPLICATION_SOLAR_HPP
#define APPLICATION_SOLAR_HPP

#include <GeometryNode.hpp>
#include "application.hpp"
#include "model.hpp"
#include "structs.hpp"

// gpu representation of model
class ApplicationSolar : public Application {
   public:
    // allocate and initialize objects
    ApplicationSolar(std::string const& resource_path);
    // free allocated objects
    ~ApplicationSolar();

    // react to key input
    void keyCallback(int key, int action, int mods);
    // handle delta mouse movement input
    void mouseCallback(double pos_x, double pos_y);
    // handle resizing
    void resizeCallback(unsigned width, unsigned height);

    // draw all objects
    void render();

   protected:
    void initializeShaderPrograms();
    void initializeGeometry();
    // update uniform values
    void uploadUniforms();
    // upload projection matrix
    void uploadProjection();
    // upload view matrix
    void uploadView();
    // draw a planet
    void renderPlanet(std::shared_ptr<GeometryNode> geom,
                      std::string const& shader_name = "planet");

    // cpu representation of model
    model_object planet_object;
};

#endif
