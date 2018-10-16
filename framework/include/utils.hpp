#ifndef UTILS_HPP
#define UTILS_HPP

#include <glbinding/gl/types.h>
// use gl definitions from glbinding 
using namespace gl;

#include <glm/gtc/type_precision.hpp>

#include <map>
#include <vector>

struct pixel_data;
struct texture_object;

namespace utils {
  // generate texture object from texture struct
  texture_object create_texture_object(pixel_data const& tex);
  // print bound textures for all texture units
  void print_bound_textures();

  // get uniform location, throwing exception if name describes no active uniform variable
  GLint glGetUniformLocation(GLuint, const GLchar*);

  // test program for drawing validity
  void validate_program(GLuint program);

  // return handle of bound vertex array object
  GLint get_bound_VAO();

  // read file and write content to string
  std::string read_file(std::string const& name);

  // return path to resources depending on cmdline args
  std::string read_resource_path(int argc, char* argv[]);

  // calculate Vert+ FOV projection matrix
  glm::fmat4 calculate_projection_matrix(float aspect);
}

#endif