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
  // free shader resources
  virtual ~Application();

  // update viewport and field of view
  void resize_callback(unsigned width, unsigned height);
  // handle key input
  void key_callback(GLFWwindow* window, int key, int action, int mods);
  //handle mouse movement input
  void mouse_callback(GLFWwindow* window, double pos_x, double pos_y);
  // recompile shaders form source files
  void reloadShaders(bool throwing);

// functiosn which are implemented in derived classes
  // update uniform locations and values
  inline virtual void uploadUniforms() {};
  // react to key input
  inline virtual void keyCallback(int key, int action, int mods) {};
  //handle delta mouse movement input
  inline virtual void mouseCallback(double pos_x, double pos_y) {};
  // update framebuffer textures
  inline virtual void resizeCallback(unsigned width, unsigned height) {};
  // draw all objects
  virtual void render() const = 0;

 protected:
  void updateUniformLocations();

  std::string m_resource_path; 

  // container for the shader programs
  std::map<std::string, shader_program> m_shaders{};

  // resolution when 
  static const unsigned C_INITIAL_WINDOW_WIDTH = 640u;
  static const unsigned C_INITIAL_WINDOW_HEIGHT = 480u;
};


#include "utils.hpp"
#include "window_handler.hpp"

//dont load gl bindings from glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

template<typename T>
void Application::run(int argc, char* argv[]) {  

    GLFWwindow* window = window_handler::initialize(C_INITIAL_WINDOW_WIDTH, C_INITIAL_WINDOW_HEIGHT);
    
    std::string resource_path = utils::read_resource_path(argc, argv);
    T* application = new T{resource_path};

    window_handler::set_callback_object(window, application);

    // do intial shader load an uniform upload
    application->reloadShaders(true);

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