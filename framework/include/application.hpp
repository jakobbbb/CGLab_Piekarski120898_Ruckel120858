#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "structs.hpp"

#include <glm/gtc/type_precision.hpp>

#include <map>

class GLFWwindow;
// gpu representation of model
class Application {
 public:
  template<typename T>
  static void run(int argc, char* argv[]);  

  // allocate and initialize objects
  Application(std::string const& resource_path);
  // free
  virtual ~Application();

  // update uniform locations and values
  inline virtual void uploadUniforms() {};
  virtual void uploadProjection() = 0;
  // react to key input
  inline virtual void keyCallback(int key, int scancode, int action, int mods) {};
  //handle delta mouse movement input
  inline virtual void mouseCallback(double pos_x, double pos_y) {};

  // update viewport and field of view
  void resize_callback(GLFWwindow* window, int width, int height);
  // handle key input
  void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
  //handle mouse movement input
  void mouse_callback(GLFWwindow* window, double pos_x, double pos_y);
  // draw all objects
  virtual void render() const = 0;

  void reloadShaders(bool throwing);

 protected:
  void updateUniformLocations();

  std::string m_resource_path; 

  // initial window dimensions
  unsigned m_window_width;
  unsigned m_window_height;

  glm::fmat4 m_view_transform;
  glm::fmat4 m_view_projection;

  // container for the shader programs
  std::map<std::string, shader_program> m_shaders{};
};

#include "utils.hpp"
#include "window_handler.hpp"

//dont load gl bindings from glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

template<typename T>
void Application::run(int argc, char* argv[]) {  
    std::string resource_path = utils::read_resource_path(argc, argv);

    GLFWwindow* window = window_handler::initialize(640u, 480u);
    auto application = new T{resource_path};

    window_handler::set_callback_object(window, application);

    application->reloadShaders(false);

    // enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    // rendering loop
    while (!glfwWindowShouldClose(window)) {
      // query input
      glfwPollEvents();
      // clear buffer
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      // draw geometry
      application->render();
      // swap draw buffer to front
      glfwSwapBuffers(window);
      // display fps
      window_handler::show_fps(window);
    }

    delete application;
    window_handler::close_and_quit(window, EXIT_SUCCESS);
}


#endif