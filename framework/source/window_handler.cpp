#include "window_handler.hpp"

#include <glbinding/gl/gl.h>
// load glbinding extensions
#include <glbinding/Binding.h>
// load meta info extension
#include <glbinding/Meta.h>

// dont load gl bindings from glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_precision.hpp>

#include "application.hpp"

#include "shader_loader.hpp"
#include "utils.hpp"

#include <cstdlib>
#include <functional>
#include <iostream>

#include <glbinding/Version.h>
// use gl definitions from glbinding
using namespace gl;

// helper functions
static void glsl_error(int error, const char* description);
static void watch_gl_errors(bool activate = true);
static void APIENTRY openglCallbackFunction(GLenum source,
                                            GLenum type,
                                            GLuint id,
                                            GLenum severity,
                                            GLsizei length,
                                            const GLchar* message,
                                            const void* userParam);

namespace window_handler {

bool isCore() {
    // if (version<glbinding::Version(3,2))
    // {
    //     return false;
    // }

    GLint value = 0;

    glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &value);

    return (value & static_cast<unsigned int>(GL_CONTEXT_CORE_PROFILE_BIT)) > 0;
}

GLFWwindow* initialize(glm::uvec2 const& resolution,
                       unsigned ver_major,
                       unsigned ver_minor) {
    glfwSetErrorCallback(glsl_error);

    if (!glfwInit()) {
        std::exit(EXIT_FAILURE);
    }

    // set OGL version explicitly
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, ver_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, ver_minor);
    // enable deug support
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

// MacOS requires forward compat core profile
#ifdef __APPLE__
    if (ver_major > 2) {
        // required to prdouce core context on MacOS
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
        // disable deprecated functionality
        if (ver_minor > 1) {
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        }
    }
#endif

    // create m_window, if unsuccessfull, quit
    GLFWwindow* window = glfwCreateWindow(resolution.x, resolution.y,
                                          "OpenGL Framework", NULL, NULL);
    if (!window) {
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

    // use the windows context
    glfwMakeContextCurrent(window);
    // disable vsync
    glfwSwapInterval(0);
    // initialize glindings in this context
    glbinding::Binding::initialize();

    std::cout << "Created OpenGL profile with version "
              << glGetString(GL_VERSION) << std::endl;
    if (isCore()) {
        std::cout << " core" << std::endl;
    } else {
        std::cout << " compat" << std::endl;
    }
    // activate error checking after each gl function call
    watch_gl_errors();

    // Enable the debug callback
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(openglCallbackFunction, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr,
                          true);

    return window;
}

void set_callback_object(GLFWwindow* window, Application* app) {
    // set user pointer to access this instance statically
    glfwSetWindowUserPointer(window, app);
    // register key input function
    auto key_func = [](GLFWwindow* w, int a, int b, int c, int d) {
        static_cast<Application*>(glfwGetWindowUserPointer(w))
            ->key_callback(w, a, c, d);
    };
    glfwSetKeyCallback(window, key_func);
    // allow free mouse movement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    auto mouse_func = [](GLFWwindow* w, double a, double b) {
        static_cast<Application*>(glfwGetWindowUserPointer(w))
            ->mouse_callback(w, a, b);
    };
    glfwSetCursorPosCallback(window, mouse_func);
    // allow free mouse movement
    // register resizing function
    auto resize_func = [](GLFWwindow* w, int a, int b) {
        static_cast<Application*>(glfwGetWindowUserPointer(w))
            ->resize_callback(a, b);
    };
    glfwSetFramebufferSizeCallback(window, resize_func);
}

// calculate fps and show in m_window title
void show_fps(GLFWwindow* window) {
    // variables for fps computation
    static double m_last_second_time;
    static unsigned m_frames_per_second;

    ++m_frames_per_second;
    double current_time = glfwGetTime();
    if (current_time - m_last_second_time >= 1.0) {
        std::string title{"OpenGL Framework - "};
        title += std::to_string(m_frames_per_second) + " fps";

        glfwSetWindowTitle(window, title.c_str());
        m_frames_per_second = 0;
        m_last_second_time = current_time;
    }
}

void close_and_quit(GLFWwindow* window, int status) {
    // free glfw resources
    glfwDestroyWindow(window);
    glfwTerminate();

    std::exit(status);
}

}  // namespace window_handler

///////////////////////////// local helper functions //////////////////////////
static void glsl_error(int error, const char* description) {
    std::cerr << "GLSL Error " << error << " : " << description << std::endl;
}

static void APIENTRY openglCallbackFunction(GLenum source,
                                            GLenum type,
                                            GLuint id,
                                            GLenum severity,
                                            GLsizei length,
                                            const GLchar* message,
                                            const void* userParam) {
    (void)source;
    (void)type;
    (void)id;
    (void)severity;
    (void)length;
    (void)userParam;
    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION ||
        type == GL_DEBUG_TYPE_PERFORMANCE) {
        return;
    }
    std::cerr << glbinding::Meta::getString(severity) << " - "
              << glbinding::Meta::getString(type) << ": ";
    std::cerr << message << std::endl;
    // if (severity != GL_DEBUG_SEVERITY_NOTIFICATION && type !=
    // GL_DEBUG_TYPE_PERFORMANCE) {
    //   throw std::runtime_error{"OpenGL error"};
    // }
}

static void watch_gl_errors(bool activate) {
    if (activate) {
        // add callback after each function call
        glbinding::setCallbackMaskExcept(
            glbinding::CallbackMask::After |
                glbinding::CallbackMask::ParametersAndReturnValue,
            {"glGetError", "glBegin", "glVertex3f", "glColor3f"});
        glbinding::setAfterCallback([](glbinding::FunctionCall const& call) {
            GLenum error = glGetError();
            if (error != GL_NO_ERROR) {
                // print name
                std::cerr << "OpenGL Error: " << call.function->name() << "(";
                // parameters
                for (unsigned i = 0; i < call.parameters.size(); ++i) {
                    std::cerr << call.parameters[i]->asString();
                    if (i < call.parameters.size() - 1)
                        std::cerr << ", ";
                }
                std::cerr << ")";
                // return value
                if (call.returnValue) {
                    std::cerr << " -> " << call.returnValue->asString();
                }
                // error
                std::cerr << " - " << glbinding::Meta::getString(error)
                          << std::endl;
                // throw exception to allow for backtrace
                throw std::runtime_error("OpenGl error: " +
                                         std::string(call.function->name()));
                exit(EXIT_FAILURE);
            }
        });
    } else {
        glbinding::setCallbackMask(glbinding::CallbackMask::None);
    }
}
