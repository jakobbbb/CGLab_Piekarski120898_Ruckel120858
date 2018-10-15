#include "application.hpp"

#include "utils.hpp"
#include "launcher.hpp"
#include "shader_loader.hpp"

#include <glbinding/gl/gl.h>
// use gl definitions from glbinding 
using namespace gl;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

//dont load gl bindings from glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

static glm::fmat4 calculate_projection_matrix(unsigned width, unsigned height);
static void update_shader_programs(std::map<std::string, shader_program>& shaders, bool throwing);


Application::Application(std::string const& resource_path)
 :m_resource_path{resource_path}
 ,m_window_width{640u}
 ,m_window_height{480u}
 ,m_view_transform{glm::translate(glm::fmat4{}, glm::fvec3{0.0f, 0.0f, 4.0f})}
 ,m_view_projection{calculate_projection_matrix(m_window_width, m_window_height)}
 ,m_shaders{}
{}

Application::~Application() {
  // free all shader program objects
  for (auto const& pair : m_shaders) {
    glDeleteProgram(pair.second.handle);
  }
}

void Application::reloadShaders(bool throwing) {
  update_shader_programs(m_shaders, throwing);
  // after shader programs are recompiled, uniform locations may change
  uploadUniforms();
}

void Application::resize_callback(GLFWwindow* m_window, int width, int height) {
  m_window_width = width;
  m_window_height = height;
  // resize framebuffer
  glViewport(0, 0, width, height);

  m_view_projection = calculate_projection_matrix(width, height);
  uploadProjection();
}

// handle key input
void Application::key_callback(GLFWwindow* m_window, int key, int scancode, int action, int mods) {
  if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(m_window, 1);
  }
  else if (key == GLFW_KEY_R && action == GLFW_PRESS) {
    reloadShaders(false);
  }
  keyCallback(key, scancode, action, mods);
}

//handle mouse movement input
void Application::mouse_callback(GLFWwindow* window, double pos_x, double pos_y) {
  mouseCallback(pos_x, pos_y);
  // reset cursor pos to receive position delta next frame
  glfwSetCursorPos(window, 0.0, 0.0);
}

// update shader uniform locations
void Application::updateUniformLocations() {
  for (auto& pair : m_shaders) {
    for (auto& uniform : pair.second.u_locs) {
      // store uniform location in map
      uniform.second = utils::glGetUniformLocation(pair.second.handle, uniform.first.c_str());
    }
  }
}


static glm::fmat4 calculate_projection_matrix(unsigned width, unsigned height) {
  float aspect = float(width) / float(height);
  // base fov does not change
  static const float fov_y_base = glm::radians(60.0f);
  float fov_y = fov_y_base;
  // if width is smaller, extend vertical fov 
  if (width < height) {
    fov_y = 2.0f * glm::atan(glm::tan(fov_y * 0.5f) * (1.0f / aspect));
  }
  // projection is hor+ 
  return glm::perspective(fov_y, aspect, 0.1f, 100.0f);
}

// update uniform locations
static void update_shader_programs(std::map<std::string, shader_program>& shaders, bool throwing) {
  // actual functionality in lambda to allow update with and without throwing
  auto update_lambda = [&](){
    // reload all shader programs
    for (auto& pair : shaders) {
      // throws exception when compiling was unsuccessfull
      GLuint new_program = shader_loader::program(pair.second.vertex_path,
                                                  pair.second.fragment_path);
      // free old shader program
      glDeleteProgram(pair.second.handle);
      // save new shader program
      pair.second.handle = new_program;
    }
  };

  if (throwing) {
    update_lambda();
  }
  else {
    try {
     update_lambda();
    }
    catch(std::exception&) {
      // dont crash, allow another try
    }
  }
}

