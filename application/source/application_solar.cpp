#include "application_solar.hpp"
#include "window_handler.hpp"

#include "model_loader.hpp"
#include "shader_loader.hpp"
#include "utils.hpp"

#include <GeometryNode.hpp>
#include <Node.hpp>
#include <SceneGraph.hpp>
#include <scenegraph_solar.hpp>

#include <cassert>

#include <glbinding/gl/gl.h>
// use gl definitions from glbinding
using namespace gl;

// dont load gl bindings from glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>

#define ORBIT_NUM_LINE_SEGMENTS 64

ApplicationSolar::ApplicationSolar(std::string const& resource_path)
    : Application{resource_path}, planet_object{} {
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

    initializeGeometry();
    initializeShaderPrograms();
}

ApplicationSolar::~ApplicationSolar() {
    glDeleteBuffers(1, &planet_object.vertex_BO);
    glDeleteBuffers(1, &planet_object.element_BO);
    glDeleteVertexArrays(1, &planet_object.vertex_AO);
}

void ApplicationSolar::render() {
    auto planet_idx = 0;
    node_traverse_func render = [&](std::shared_ptr<Node> node) {
        auto geom_node = std::dynamic_pointer_cast<GeometryNode>(node);
        if (geom_node) {
            renderObject(geom_node);
            if (geom_node->getShaderName() == "planet" &&
                node->getName() != "Sun Geometry") {
                node->getParent()->rotate(
                    (float)glfwGetTime() / ORBIT_PERIODS[planet_idx] / 10e4f,
                    SUN_AXIS);
                planet_idx++;
            }
        }
    };
    SceneGraph::getInstance().traverse(render);
}

void ApplicationSolar::renderObject(std::shared_ptr<GeometryNode> node) {
    auto shader_name = node->getShaderName();
    auto model_matrix = node->getWorldTransform();
    auto geometry_object = node->getGeometry();

    // bind shader to upload uniforms
    glUseProgram(m_shaders.at(shader_name).handle);

    glUniformMatrix4fv(m_shaders.at(shader_name).u_locs.at("ModelMatrix"), 1,
                       GL_FALSE, glm::value_ptr(model_matrix));

    // extra matrix for normal transformation to keep them orthogonal to surface
    glm::fmat4 normal_matrix = glm::inverseTranspose(
        glm::inverse(SceneGraph::getActiveCamera()->getWorldTransform()) *
        model_matrix);
    glUniformMatrix4fv(m_shaders.at(shader_name).u_locs.at("NormalMatrix"), 1,
                       GL_FALSE, glm::value_ptr(normal_matrix));

    // bind the VAO to draw
    glBindVertexArray(geometry_object.vertex_AO);

    // draw bound vertex array using bound shader
    if (geometry_object.using_indices) {
        glDrawElements(geometry_object.draw_mode, geometry_object.num_elements,
                       model::INDEX.type, NULL);
    } else {
        glDrawArrays(geometry_object.draw_mode, 0,
                     geometry_object.num_elements);
    }
}

void ApplicationSolar::renderStars() {
    glUseProgram(m_shaders.at("stars").handle);
    glBindVertexArray(star_object.vertex_AO);
    glDrawArrays(star_object.draw_mode, 0, star_object.num_elements);
}

void ApplicationSolar::uploadView() {
    // vertices are transformed in camera space, so camera transform must be
    // inverted
    glm::fmat4 view_matrix =
        glm::inverse(SceneGraph::getActiveCamera()->getWorldTransform());
    // upload matrix to gpu
    glUseProgram(m_shaders.at("planet").handle);
    glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ViewMatrix"), 1,
                       GL_FALSE, glm::value_ptr(view_matrix));
    glUseProgram(m_shaders.at("orbit").handle);
    glUniformMatrix4fv(m_shaders.at("orbit").u_locs.at("ViewMatrix"), 1,
                       GL_FALSE, glm::value_ptr(view_matrix));
    glUseProgram(m_shaders.at("stars").handle);
    glUniformMatrix4fv(m_shaders.at("stars").u_locs.at("ModelViewMatrix"), 1,
                       GL_FALSE, glm::value_ptr(view_matrix));
}

void ApplicationSolar::uploadProjection() {
    // upload matrix to gpu
    glUseProgram(m_shaders.at("planet").handle);
    glUniformMatrix4fv(
        m_shaders.at("planet").u_locs.at("ProjectionMatrix"), 1, GL_FALSE,
        glm::value_ptr(SceneGraph::getActiveCamera()->getProjectionMatrix()));
    glUseProgram(m_shaders.at("orbit").handle);
    glUniformMatrix4fv(
        m_shaders.at("orbit").u_locs.at("ProjectionMatrix"), 1, GL_FALSE,
        glm::value_ptr(SceneGraph::getActiveCamera()->getProjectionMatrix()));
    glUseProgram(m_shaders.at("stars").handle);
    glUniformMatrix4fv(
        m_shaders.at("stars").u_locs.at("ProjectionMatrix"), 1, GL_FALSE,
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
    m_shaders.at("planet").u_locs["NormalMatrix"] = -1;
    m_shaders.at("planet").u_locs["ModelMatrix"] = -1;
    m_shaders.at("planet").u_locs["ViewMatrix"] = -1;
    m_shaders.at("planet").u_locs["ProjectionMatrix"] = -1;

    m_shaders.emplace(
        "orbit",
        shader_program{
            {{GL_VERTEX_SHADER, m_resource_path + "shaders/orbit.vert"},
             {GL_FRAGMENT_SHADER, m_resource_path + "shaders/orbit.frag"}}});
    m_shaders.at("orbit").u_locs["NormalMatrix"] = -1;
    m_shaders.at("orbit").u_locs["ModelMatrix"] = -1;
    m_shaders.at("orbit").u_locs["ViewMatrix"] = -1;
    m_shaders.at("orbit").u_locs["ProjectionMatrix"] = -1;

    m_shaders.emplace(
        "stars", 
        shader_program{
            {{GL_VERTEX_SHADER, m_resource_path + "shaders/simple.vert"},
             {GL_FRAGMENT_SHADER, m_resource_path + "shaders/simple.frag"}}});
    //m_shaders.at("stars").u_locs["NormalMatrix"] = -1;
    //m_shaders.at("stars").u_locs["ModelMatrix"] = -1;
    m_shaders.at("stars").u_locs["ModelViewMatrix"] = -1;
    m_shaders.at("stars").u_locs["ProjectionMatrix"] = -1;
}

void ApplicationSolar::initializePlanetGeometry() {
    model planet_model =
        model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL);

    // generate vertex array object
    glGenVertexArrays(1, &planet_object.vertex_AO);
    // bind the array for attaching buffers
    glBindVertexArray(planet_object.vertex_AO);

    // generate generic buffer
    glGenBuffers(1, &planet_object.vertex_BO);
    // bind this as an vertex array buffer containing all attributes
    glBindBuffer(GL_ARRAY_BUFFER, planet_object.vertex_BO);
    // configure currently bound array buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * planet_model.data.size(),
                 planet_model.data.data(), GL_STATIC_DRAW);

    // activate first attribute on gpu
    glEnableVertexAttribArray(0);
    // first attribute is 3 floats with no offset & stride
    glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type,
                          GL_FALSE, planet_model.vertex_bytes,
                          planet_model.offsets[model::POSITION]);
    // activate second attribute on gpu
    glEnableVertexAttribArray(1);
    // second attribute is 3 floats with no offset & stride
    glVertexAttribPointer(1, model::NORMAL.components, model::NORMAL.type,
                          GL_FALSE, planet_model.vertex_bytes,
                          planet_model.offsets[model::NORMAL]);

    // generate generic buffer
    glGenBuffers(1, &planet_object.element_BO);
    // bind this as an vertex array buffer containing all attributes
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planet_object.element_BO);
    // configure currently bound array buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 model::INDEX.size * planet_model.indices.size(),
                 planet_model.indices.data(), GL_STATIC_DRAW);

    // store type of primitive to draw
    planet_object.draw_mode = GL_TRIANGLES;
    // transfer number of indices to model object
    planet_object.num_elements = GLsizei(planet_model.indices.size());

    planet_object.using_indices = true;
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

    // generate generic buffer
    glGenBuffers(1, &orbit_object.element_BO);
    // bind this as an vertex array buffer containing all attributes
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, orbit_object.element_BO);
    // configure currently bound array buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 GLsizei(sizeof(float) * points.size()), points.data(),
                 GL_STATIC_DRAW);

    // store type of primitive to draw
    orbit_object.draw_mode = GL_LINE_LOOP;
    // transfer number of indices to model object
    orbit_object.num_elements = GLsizei(points.size() / 3);
}

void ApplicationSolar::initializeStarGeometry() {
    // Coming soon...
}

// load models
void ApplicationSolar::initializeGeometry() {
    initializePlanetGeometry();
    initializeOrbitGeometry();

    node_traverse_func set_geometry = [&](std::shared_ptr<Node> node) {
        auto geom_node = std::dynamic_pointer_cast<GeometryNode>(node);
        if (!geom_node) {
            return;
        }

        if (geom_node->getShaderName() == "orbit") {
            // node is orbit
            geom_node->setGeometry(orbit_object);
        } else {
            // node is planet
            geom_node->setGeometry(planet_object);
        }
    };
    SceneGraph::getInstance().traverse(set_geometry);
}

///////////////////////////// callback functions for window events ////////////
// handle key input
// - press W / S == zoom in / out
// - press A / D == move left / right
void ApplicationSolar::keyCallback(int key, int action, int mods) {
    auto cam = SceneGraph::getActiveCamera();
    if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        cam->translate(glm::fvec3{0.0f, 0.0f, -0.1f});
        uploadView();
    } else if (key == GLFW_KEY_S &&
               (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        cam->translate(glm::fvec3{0.0f, 0.0f, 0.1f});
        uploadView();
    } else if (key == GLFW_KEY_A &&
               (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        cam->translate(glm::fvec3{-0.1f, 0.0f, 0.0f});
        uploadView();
    } else if (key == GLFW_KEY_D &&
               (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        cam->translate(glm::fvec3{0.1f, 0.0f, 0.0f});
        uploadView();
    }
}

// handle delta mouse movement input
void ApplicationSolar::mouseCallback(double pos_x, double pos_y) {
    auto cam = SceneGraph::getActiveCamera();
    // mouse handling, x position movement
    cam->rotate(glm::radians(float(pos_x / 50)),  // angle
                glm::vec3{0.0f, -1.0f, 0.0f}      // axis
    );
    // mouse handling, y position movement
    cam->rotate(glm::radians(float(pos_y / 50)),  // angle
                glm::vec3{-1.0f, 0.0f, 0.0f}      // axis
    );
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
}

// exe entry point
int main(int argc, char* argv[]) {
    Application::run<ApplicationSolar>(argc, argv, 3, 2);
}
