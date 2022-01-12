#include "application_solar.hpp"
#include "window_handler.hpp"

#include "model_loader.hpp"
#include "shader_loader.hpp"
#include "texture_loader.hpp"
#include "utils.hpp"

#include <GeometryNode.hpp>
#include <Node.hpp>
#include <SceneGraph.hpp>
#include <scenegraph_solar.hpp>

#include <cassert>
#include <regex>
#include <string>
#include <vector>

#include <glbinding/gl/gl.h>
// use gl definitions from glbinding
using namespace gl;

// dont load gl bindings from glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/color_space.hpp>
#include <glm/gtx/string_cast.hpp>

#include <functional>
#include <iostream>

#define ORBIT_NUM_LINE_SEGMENTS 340
#define STAR_NUM 5000
#define COLORS 255.f
#define BOX_LENGTH 50

ApplicationSolar::ApplicationSolar(std::string const& resource_path)
    : Application{resource_path}, planet_object{}, star_object{} {
    // Setup Scene
    make_solar_scene();

    // Setup Camera
    auto view_transform =
        glm::translate(glm::fmat4{}, glm::fvec3{0.0f, 0.0f, 4.0f});
    auto view_projection =
        utils::calculate_projection_matrix(initial_aspect_ratio);
    auto cam = SceneGraph::getActiveCamera();
    assert(cam != nullptr);
    cam->setLocalTransform(view_transform);
    cam->setProjectionMatrix(view_projection);

    skybox_texture = utils::loadSkyboxTexture(m_resource_path);

    initializeGeometry();
    initializeShaderPrograms();
    initializeTextures();
    initializeFramebuffer();
}

ApplicationSolar::~ApplicationSolar() {
    glDeleteBuffers(1, &planet_object.vertex_BO);
    glDeleteBuffers(1, &planet_object.element_BO);
    glDeleteVertexArrays(1, &planet_object.vertex_AO);
    glDeleteBuffers(1, &star_object.vertex_BO);
    glDeleteBuffers(1, &star_object.element_BO);
    glDeleteVertexArrays(1, &star_object.vertex_AO);
    glDeleteBuffers(1, &skybox_object.vertex_BO);
    glDeleteBuffers(1, &skybox_object.element_BO);
    glDeleteBuffers(1, &skybox_object.vertex_AO);
    glDeleteFramebuffers(1, &fbo);
    glDeleteTextures(1, &texture);
    glDeleteRenderbuffers(1, &rbo);
}

void ApplicationSolar::render() {
    auto planet_idx = 0;
    node_traverse_func render = [&](std::shared_ptr<Node> node) {
        auto geom_node = std::dynamic_pointer_cast<GeometryNode>(node);
        if (geom_node) {
            renderObject(geom_node);
            if (geom_node->getShaderName() == "planet" &&
                node->getName() != "Sun Geometry") {
                node->getParent()->rotate(10e-4f / ORBIT_PERIODS[planet_idx],
                                          SUN_AXIS);
                node->rotate(5e-2f / DAY_LENGTHS[planet_idx], SUN_AXIS);
                planet_idx++;
            }
        }
    };
    SceneGraph::getInstance().traverse(render);
    uploadView();
}

void ApplicationSolar::renderObject(std::shared_ptr<GeometryNode> node) {
    auto shader_name = node->getShaderName();
    auto model_matrix = node->getWorldTransform();
    auto geometry_object = node->getGeometry();
    auto color = node->getColor();
    auto light = SceneGraph::getLight();

    // bind shader to upload uniforms
    glUseProgram(m_shaders.at(shader_name).handle);

    if (shader_name == "orbit") {
        glUniform3f(m_shaders.at(shader_name).u_locs.at("Color"),
                    color.r / COLORS, color.g / COLORS, color.b / COLORS);
    }

    // assign a different color for each planet using glUniform3f
    if (shader_name == "planet" ) {
        //glUniform3f(m_shaders.at(shader_name).u_locs.at("PlanetColor"),
        //        color.r/COLORS, color.g/COLORS, color.b/COLORS);
        glUniform3f(m_shaders.at(shader_name).u_locs.at("AmbientColor"),
                1.0f, 1.0f, 1.0f);

        float ambient_intensity = 0.15f;
        if (node->getName() == "Sun Geometry") {
            ambient_intensity = 10.0f;
        } else if (node->getName() == "Saturn Rings") {
            ambient_intensity = 0.7f;
        }
        glUniform1f(m_shaders.at(shader_name).u_locs.at("AmbientIntensity"),
                    ambient_intensity);

        auto light_pos = light->getWorldTransform() * glm::vec4{0, 0, 0, 1};
        glUniform3fv(m_shaders.at(shader_name).u_locs.at("LightPosition"), 1,
                     glm::value_ptr(light_pos));
        glUniform1f(m_shaders.at(shader_name).u_locs.at("LightIntensity"),
                    light->getLightIntensity());
        auto l_color = light->getLightColor();
        glUniform3f(m_shaders.at(shader_name).u_locs.at("LightColor"),
                    l_color.r, l_color.g, l_color.b);
        auto loc_cel =
            glGetUniformLocation(m_shaders.at(shader_name).handle, "Cel");
        glUniform1i(loc_cel, bool_cel);

        auto cam_transform = SceneGraph::getActiveCamera()->getWorldTransform();
        glUniform3fv(m_shaders.at(shader_name).u_locs.at("CameraPosition"), 1,
                     glm::value_ptr(cam_transform * glm::vec4{0, 0, 0, 1}));
    }

    glUniformMatrix4fv(m_shaders.at(shader_name).u_locs.at("ModelMatrix"), 1,
                       false, glm::value_ptr(model_matrix));

    // extra matrix for normal transformation to keep them orthogonal to surface
    glm::fmat4 normal_matrix = glm::inverseTranspose(
        glm::inverse(SceneGraph::getActiveCamera()->getWorldTransform()) *
        model_matrix);
    //glUniformMatrix4fv(m_shaders.at(shader_name).u_locs.at("NormalMatrix"), 1,
                       //false, glm::value_ptr(normal_matrix));

    // bind the VAO to draw
    glBindVertexArray(geometry_object.vertex_AO);

    auto texture_diffuse = node->getDiffuseTexture();

    // draw bound vertex array using bound shader
    if (geometry_object.using_indices) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(texture_diffuse.target, texture_diffuse.handle);
        glUniform1i(m_shaders.at(shader_name).u_locs.at("Texture"), 0);

        if (node->getName() == "Earth Geometry") {
            auto texture_normal = node->getNormalTexture();
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(texture_normal.target, texture_normal.handle);
            glUniform1i(m_shaders.at(shader_name).u_locs.at("NormalMap"), 1);

            glUniform1b(m_shaders.at(shader_name).u_locs.at("UseNormalMap"),
                        true);
        } else {
            glUniform1b(m_shaders.at(shader_name).u_locs.at("UseNormalMap"),
                        false);
        }

        glDrawElements(geometry_object.draw_mode, geometry_object.num_elements,
                       model::INDEX.type, NULL);
    } else {
        glDrawArrays(geometry_object.draw_mode, 0,
                     geometry_object.num_elements);
    }
}

void ApplicationSolar::uploadView() {
    // vertices are transformed in camera space, so camera transform must be
    // inverted
    glm::fmat4 view_matrix =
        glm::inverse(SceneGraph::getActiveCamera()->getRotatedWorldTransform());
    //
    // upload matrix to gpu
    glUseProgram(m_shaders.at("planet").handle);
    glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ViewMatrix"), 1,
                       false, glm::value_ptr(view_matrix));

    glUseProgram(m_shaders.at("orbit").handle);
    glUniformMatrix4fv(m_shaders.at("orbit").u_locs.at("ViewMatrix"), 1,
                       false, glm::value_ptr(view_matrix));

    glUseProgram(m_shaders.at("stars").handle);
    glUniformMatrix4fv(m_shaders.at("stars").u_locs.at("ViewMatrix"), 1,
                       false, glm::value_ptr(view_matrix));

    glUseProgram(m_shaders.at("skybox").handle);
    glUniformMatrix4fv(m_shaders.at("skybox").u_locs.at("ViewMatrix"), 1,
                       false, glm::value_ptr(view_matrix));
}

void ApplicationSolar::uploadProjection() {
    // upload matrix to gpu
    glUseProgram(m_shaders.at("planet").handle);
    glUniformMatrix4fv(
        m_shaders.at("planet").u_locs.at("ProjectionMatrix"), 1, false,
        glm::value_ptr(SceneGraph::getActiveCamera()->getProjectionMatrix()));
    glUseProgram(m_shaders.at("orbit").handle);
    glUniformMatrix4fv(
        m_shaders.at("orbit").u_locs.at("ProjectionMatrix"), 1, false,
        glm::value_ptr(SceneGraph::getActiveCamera()->getProjectionMatrix()));
    glUseProgram(m_shaders.at("stars").handle);
    glUniformMatrix4fv(
        m_shaders.at("stars").u_locs.at("ProjectionMatrix"), 1, false,
        glm::value_ptr(SceneGraph::getActiveCamera()->getProjectionMatrix()));
    glUseProgram(m_shaders.at("skybox").handle);
    glUniformMatrix4fv(
        m_shaders.at("skybox").u_locs.at("ProjectionMatrix"), 1, false,
        glm::value_ptr(SceneGraph::getActiveCamera()->getProjectionMatrix()));
}

// update uniform locations
void ApplicationSolar::uploadUniforms() {
    // upload uniform values to new locations
    uploadView();
    uploadProjection();
}

///////////////////////////// intialisation functions /////////////////////////
// load shader sources
void ApplicationSolar::initializeShaderPrograms() {
    // store shader program objects in container
    m_shaders.emplace(
        "planet",
        shader_program{
            {{GL_VERTEX_SHADER, m_resource_path + "shaders/simple.vert"},
             {GL_FRAGMENT_SHADER, m_resource_path + "shaders/simple.frag"}}});
    // request uniform locations for shader program
    //m_shaders.at("planet").u_locs["NormalMatrix"] = -1;
    m_shaders.at("planet").u_locs["ModelMatrix"] = -1;
    m_shaders.at("planet").u_locs["ViewMatrix"] = -1;
    m_shaders.at("planet").u_locs["ProjectionMatrix"] = -1;
    // m_shaders.at("planet").u_locs["PlanetColor"] = -1;
    m_shaders.at("planet").u_locs["AmbientColor"] = -1;
    m_shaders.at("planet").u_locs["AmbientIntensity"] = -1;
    m_shaders.at("planet").u_locs["LightPosition"] = -1;
    m_shaders.at("planet").u_locs["LightIntensity"] = -1;
    m_shaders.at("planet").u_locs["LightColor"] = -1;
    m_shaders.at("planet").u_locs["CameraPosition"] = -1;
    m_shaders.at("planet").u_locs["UseNormalMap"] = -1;
    m_shaders.at("planet").u_locs["Texture"] = -1;
    m_shaders.at("planet").u_locs["NormalMap"] = -1;

    m_shaders.emplace(
        "orbit",
        shader_program{
            {{GL_VERTEX_SHADER, m_resource_path + "shaders/orbit.vert"},
             {GL_FRAGMENT_SHADER, m_resource_path + "shaders/orbit.frag"}}});
    //m_shaders.at("orbit").u_locs["NormalMatrix"] = -1;
    m_shaders.at("orbit").u_locs["ModelMatrix"] = -1;
    m_shaders.at("orbit").u_locs["ViewMatrix"] = -1;
    m_shaders.at("orbit").u_locs["ProjectionMatrix"] = -1;
    m_shaders.at("orbit").u_locs["Color"] = -1;

    m_shaders.emplace(
        "stars",
        shader_program{
            {{GL_VERTEX_SHADER, m_resource_path + "shaders/vao.vert"},
             {GL_FRAGMENT_SHADER, m_resource_path + "shaders/vao.frag"}}});
    //m_shaders.at("stars").u_locs["NormalMatrix"] = -1;
    m_shaders.at("stars").u_locs["ModelMatrix"] = -1;
    m_shaders.at("stars").u_locs["ViewMatrix"] = -1;
    m_shaders.at("stars").u_locs["ProjectionMatrix"] = -1;

    m_shaders.emplace(
        "skybox",
        shader_program{
            {{GL_VERTEX_SHADER, m_resource_path + "shaders/skybox.vert"},
             {GL_FRAGMENT_SHADER, m_resource_path + "shaders/skybox.frag"}}});
    //m_shaders.at("skybox").u_locs["NormalMatrix"] = -1;
    m_shaders.at("skybox").u_locs["ModelMatrix"] = -1;
    m_shaders.at("skybox").u_locs["ViewMatrix"] = -1;
    m_shaders.at("skybox").u_locs["ProjectionMatrix"] = -1;
    m_shaders.at("skybox").u_locs["Texture"] = -1;
}

void ApplicationSolar::initializeOrbitGeometry() {
    std::vector<float> points;
    for (int i = 0; i < ORBIT_NUM_LINE_SEGMENTS; ++i) {
        float theta = 2 * (float)M_PI * (float)i / ORBIT_NUM_LINE_SEGMENTS;
        points.push_back((float)sin(theta));
        points.push_back(0);
        points.push_back((float)cos(theta));
    }

    // generate vertex array object
    glGenVertexArrays(1, &orbit_object.vertex_AO);
    // bind the array for attaching buffers
    glBindVertexArray(orbit_object.vertex_AO);

    // generate generic buffer
    glGenBuffers(1, &orbit_object.vertex_BO);
    // bind this as an vertex array buffer containing all attributes
    glBindBuffer(GL_ARRAY_BUFFER, orbit_object.vertex_BO);
    // configure currently bound array buffer
    glBufferData(GL_ARRAY_BUFFER, GLsizei(points.size() * sizeof(float)),
                 points.data(), GL_STATIC_DRAW);

    // activate first attribute on gpu
    glEnableVertexAttribArray(0);
    // first attribute is 3 floats with no offset & stride
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float) * 3, 0);

    // store type of primitive to draw
    orbit_object.draw_mode = GL_LINE_LOOP;
    // transfer number of indices to model object
    orbit_object.num_elements = GLsizei(points.size() / 3);
}

void ApplicationSolar::initializeStarGeometry() {
    // instantiate empty container of floats
    std::vector<float> stars;

    // for each star push random position and color values
    for (int i = 0; i < STAR_NUM; ++i) {
        for (int p = 0; p < 3; ++p) {  // position
            stars.push_back(2 * (RAND_FLOAT() - 0.5f));
        }
        for (int c = 0; c < 3; ++c) {  // color
            stars.push_back(float(std::rand() % 255) / COLORS);
        }
    }

    // generate vertex array object
    glGenVertexArrays(1, &star_object.vertex_AO);
    // bind the array for attaching buffers
    glBindVertexArray(star_object.vertex_AO);
    // generate generic buffer
    glGenBuffers(1, &star_object.vertex_BO);
    // bind this as an vertex array buffer containing all attributes
    glBindBuffer(GL_ARRAY_BUFFER, star_object.vertex_BO);
    // configure currently bound array buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * STAR_NUM * 6, stars.data(),
                 GL_STATIC_DRAW);
    // activate first attribute on gpu
    glEnableVertexAttribArray(0);
    // position is the first attribute with 3 floats (XYZ)
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float) * 6, 0);
    // activate second attribute on gpu
    glEnableVertexAttribArray(1);
    // color is the second attribute with 3 floats (RGB)
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(float) * 6,
                          (void*)(sizeof(float) * 3));
    // set the draw_mode to GL_POINTS (each point represents a star)
    star_object.draw_mode = GL_POINTS;
    star_object.num_elements = GLsizei(STAR_NUM);
}

model_object ApplicationSolar::loadObject(std::string const& path) {
    model_object model_obj;
    model model =
        model_loader::obj(m_resource_path + path,
                model::NORMAL | model::TEXCOORD);

    // generate vertex array object
    glGenVertexArrays(1, &model_obj.vertex_AO);
    // bind the array for attaching buffers
    glBindVertexArray(model_obj.vertex_AO);

    // generate generic buffer
    glGenBuffers(1, &model_obj.vertex_BO);
    // bind this as an vertex array buffer containing all attributes
    glBindBuffer(GL_ARRAY_BUFFER, model_obj.vertex_BO);
    // configure currently bound array buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * model.data.size(),
                 model.data.data(), GL_STATIC_DRAW);

    // First Attribute: Position
    glEnableVertexAttribArray(0);
    // first attribute is 3 floats with no offset & stride
    glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type,
                          false, model.vertex_bytes,
                          model.offsets[model::POSITION]);

    // Second Attribute: Position
    glEnableVertexAttribArray(1);
    // second attribute is 3 floats with no offset & stride
    glVertexAttribPointer(1, model::NORMAL.components, model::NORMAL.type,
                          false, model.vertex_bytes,
                          model.offsets[model::NORMAL]);

    // Third Attribute: Texcoord
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, model::TEXCOORD.components, model::TEXCOORD.type,
                          false, model.vertex_bytes,
                          model.offsets[model::TEXCOORD]);


    // generate generic buffer
    glGenBuffers(1, &model_obj.element_BO);
    // bind this as an vertex array buffer containing all attributes
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model_obj.element_BO);
    // configure currently bound array buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 model::INDEX.size * model.indices.size(),
                 model.indices.data(), GL_STATIC_DRAW);

    // store type of primitive to draw
    model_obj.draw_mode = GL_TRIANGLES;
    // transfer number of indices to model object
    model_obj.num_elements = GLsizei(model.indices.size());

    model_obj.using_indices = true;

    return model_obj;
}

// load models
void ApplicationSolar::initializeGeometry() {
    initializeOrbitGeometry();
    initializeStarGeometry();
    initializeSkyboxGeometry();

    ring_object = loadObject("models/rings.obj");
    if (std::getenv("NORMAL_DEBUG") == nullptr) {
        planet_object = loadObject("models/sphere.obj");
    } else {
        planet_object = loadObject("models/cube.obj");
    }

    node_traverse_func set_geometry = [&](std::shared_ptr<Node> node) {
        auto geom_node = std::dynamic_pointer_cast<GeometryNode>(node);
        if (!geom_node) {
            return;
        }

        if (geom_node->getShaderName() == "orbit") {
            // node is orbit
            geom_node->setGeometry(orbit_object);
        } else if (geom_node->getShaderName() == "stars") {
            // node is star
            geom_node->setGeometry(star_object);
        }   // node is skybox
          else if (geom_node->getShaderName() == "skybox") {
            geom_node->setGeometry(skybox_object);
        } else {
            if (geom_node->getName() == "Saturn Rings") {
                geom_node->setGeometry(ring_object);
            } else {
                // node is planet
                geom_node->setGeometry(planet_object);
            }
        }
    };
    SceneGraph::getInstance().traverse(set_geometry);
}

void ApplicationSolar::initializeTextures() {
    node_traverse_func load_textures = [&](std::shared_ptr<Node> node) {
        auto geom_node = std::dynamic_pointer_cast<GeometryNode>(node);
        if (!geom_node || (geom_node->getShaderName() != "planet")) {
            return;
        }
        auto planet_name = std::regex_replace(geom_node->getName(),
                                              std::regex(" Geometry"), "");
        auto texture_path =
            m_resource_path + "textures/planets/" + planet_name + ".png";
        pixel_data pixels = texture_loader::file(texture_path);
        texture_object texture = utils::create_texture_object(pixels);
        geom_node->setDiffuseTexture(texture);

        if (planet_name == "Earth") {
            auto texture_path =
                m_resource_path + "textures/planets/" + planet_name + "Normal.png";
            pixel_data pixels_normal = texture_loader::file(texture_path);
            texture_object texture_normal =
                utils::create_texture_object(pixels_normal);
            geom_node->setNormalTexture(texture_normal);

            if (std::getenv("NORMAL_DEBUG") != NULL) {
                std::cout << "Using normal debug textures\n";
                auto texture_path = m_resource_path + "textures/tests/test.png";
                pixel_data pixels = texture_loader::file(texture_path);
                texture_object texture = utils::create_texture_object(pixels);
                geom_node->setDiffuseTexture(texture);
                auto texture_path_normal =
                    m_resource_path + "textures/tests/normal_test.png";
                pixel_data pixels_normal =
                    texture_loader::file(texture_path_normal);
                texture_object texture_normal =
                    utils::create_texture_object(pixels_normal);
                geom_node->setNormalTexture(texture_normal);
            }
        }
    };
    SceneGraph::getInstance().traverse(load_textures);
}

void ApplicationSolar::initializeSkyboxGeometry() {

    // instantiate empty container of floats
    std::vector<float> v = utils::BoxVertices(500);

    // generate vertex array object
    glGenVertexArrays(1, &skybox_object.vertex_AO);
    // bind the array for attaching buffers
    glBindVertexArray(skybox_object.vertex_AO);
    // generate generic buffer
    glGenBuffers(1, &skybox_object.vertex_BO);
    // bind this as an vertex array buffer containing all attributes
    glBindBuffer(GL_ARRAY_BUFFER, skybox_object.vertex_BO);
    // configure currently bound array buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * v.size(), v.data(),
                 GL_STATIC_DRAW);
    // activate first attribute on gpu
    glEnableVertexAttribArray(0);
    // position is the first attribute with 3 floats (XYZ)
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float) * 3, 0);
    // activate second attribute on gpu
    skybox_object.draw_mode = GL_TRIANGLES;
    skybox_object.num_elements = GLsizei(v.size());
}

void ApplicationSolar::initializeFramebuffer() {  
    // generate and bind framebuffer object
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    // generate and bind texture
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, 
                 GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    // attach texture to framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
                           GL_TEXTURE_2D, texture, 0);
    // define renderbuffer object
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    // define a depth and stencil renderbuffer object
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    // attach renderbuffer object to depth and stencil attachment of framebuffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, 
                              GL_RENDERBUFFER, rbo);
    // always check if framebuffer is complete
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



///////////////////////////// callback functions for window events ////////////
// handle key input
void ApplicationSolar::keyCallback(int key, int action, int mods) {
    if (key == GLFW_KEY_LEFT_SHIFT) {
        if (action == GLFW_PRESS) {
            movement_speed = 1.5f;
        } else if (action == GLFW_RELEASE) {
            movement_speed = 0.2f;
        }
    }

    auto cam = SceneGraph::getActiveCamera();
    if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        cam->translate(movement_speed * cam->forward());
        uploadView();
    } else if (key == GLFW_KEY_S &&
               (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        cam->translate(movement_speed * -cam->forward());
        uploadView();
    } else if (key == GLFW_KEY_A &&
               (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        cam->translate(movement_speed * -cam->right());
        uploadView();
    } else if (key == GLFW_KEY_D &&
               (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        cam->translate(movement_speed * cam->right());
        uploadView();
    } else if (key == GLFW_KEY_1 &&
               (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        bool_cel = !bool_cel;
    } else if (key == GLFW_KEY_2 &&
               (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        uploadView();
    }

    if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
        auto cam1 =
            std::dynamic_pointer_cast<CameraNode>(SceneGraph::find("Camera"));
        auto cam2 =
            std::dynamic_pointer_cast<CameraNode>(SceneGraph::find("Camera 2"));
        assert(cam1 != nullptr);
        assert(cam2 != nullptr);
        cam1->setEnabled(cam2->getEnabled());
        cam2->setEnabled(!cam1->getEnabled());
    }
}

// handle delta mouse movement input
void ApplicationSolar::mouseCallback(double pos_x, double pos_y) {
    auto cam = SceneGraph::getActiveCamera();
    cam->rotateCamera(-(float)(pos_y * 10e-4), -(float)(pos_x * 10e-4));
    uploadView();
}

// handle resizing
void ApplicationSolar::resizeCallback(unsigned width, unsigned height) {
    // recalculate projection matrix for new aspect ration
    auto view_projection =
        utils::calculate_projection_matrix(float(width) / float(height));
    SceneGraph::getActiveCamera()->setProjectionMatrix(view_projection);
    // upload new projection matrix
    uploadProjection();
    initializeFramebuffer();
}

// exe entry point
int main(int argc, char* argv[]) {
    Application::run<ApplicationSolar>(argc, argv, 3, 2);
}
