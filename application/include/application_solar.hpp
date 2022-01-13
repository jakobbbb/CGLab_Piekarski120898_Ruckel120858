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
    void initializeStarGeometry();
    void initializeOrbitGeometry();
    model_object loadObject(std::string const& path);
    void initializeGeometry();
    void initializeTextures();
    void initializeSkyboxGeometry();
    void initializeFramebuffer();
    void initializeQuad();
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
    model_object ring_object;
    model_object quad_object;

    // Skybox
    texture_object skybox_texture;

    // Cel-shading
    bool bool_cel = false;

    float movement_speed = 1.0f;

    // framebuffer
    unsigned int fbo;
    // texture
    unsigned int texture;
    // render buffer
    unsigned int rbo;
    // screen size
    unsigned int width = 800;
    unsigned int height = 600;

    // post-processing effects
    bool bool_horizontal_mirror = false;
    bool bool_vertical_mirror = false;
    bool bool_gray = false;
    bool bool_blur = false;
};

#endif
