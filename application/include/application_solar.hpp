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
    void initializePlanetGeometry();
    void initializeOrbitGeometry();
    void initializeStarGeometry();
    void initializeGeometry();
    void initializeTextures();
    void initializeSkyboxGeometry();
    // update uniform values
    void uploadUniforms();
    // upload projection matrix
    void uploadProjection();
    // upload view matrix
    void uploadView();
    // draw a planet
    void renderObject(std::shared_ptr<GeometryNode> geom);
    void renderStars();
    
    

    // cpu representation of model
    model_object planet_object;
    model_object star_object;
    model_object orbit_object;
    model_object skybox_object;

    // Skybox
    texture_object skybox_texture;

    // Cel-shading
    GLboolean bool_cel = GL_FALSE;
    static GLuint loc_cel;

    float movement_speed = 1.0f;
};

#endif
