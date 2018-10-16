#include "utils.hpp"

#include "pixel_data.hpp"
#include "structs.hpp"

#include <glbinding/gl/functions.h>
// use gl definitions from glbinding 
using namespace gl;

#include <glm/gtc/type_precision.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <sstream>
#include <fstream>

namespace utils {

texture_object create_texture_object(pixel_data const& tex) {
  texture_object t_obj{};

  throw std::logic_error("Texture Object creation not implemented yet");

  return t_obj;
}

void print_bound_textures() {
  GLint id1, id2, id3, active_unit, texture_units = 0;
  glGetIntegerv(GL_ACTIVE_TEXTURE, &active_unit);
  std::cout << "Active texture unit: " << active_unit - GLint(GL_TEXTURE0) << std::endl;

  glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texture_units);

  for(GLint i = 0; i < texture_units; ++i) {
    glActiveTexture(GL_TEXTURE0 + i);
    glGetIntegerv(GL_TEXTURE_BINDING_3D, &id3);
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &id2);
    glGetIntegerv(GL_TEXTURE_BINDING_1D, &id1);
    if(id1 != 0 || id2 != 0 || id3 != 0) {
      std::cout <<"Texture unit " << i << " - ";
      if(id1 != 0) std::cout << "1D: " << id1 << ", ";
      if(id2 != 0) std::cout << "2D: " << id2 << ", ";
      if(id3 != 0) std::cout << "3D: " << id3;
      std::cout << std::endl;
    }
  }
  // reactivate previously active unit
  glActiveTexture(GLenum(active_unit));
}

GLint glGetUniformLocation(GLuint program, const GLchar* name) {
  // use function from outer namespace to prevent recursion
  GLint loc = ::glGetUniformLocation(program, name);
  // if location invalid, output info similar to gl errors
  if (loc == -1) {
    std::cerr <<  "OpenGL Error: " << "glGetUniformLocation" << "(";
    std::cerr << program << ", " << name << ")";
    std::cerr << " -> " << loc;
    std::cerr  << ", " << name <<"  is not an active uniform variable in program" << std::endl;
    // dont throw, allow retrying
    // throw std::runtime_error("Execution of " + std::string("glGetUniformLocation"));
    // exit(EXIT_FAILURE);
  }
  return loc;
}

void validate_program(GLuint program) {
  glValidateProgram(program);
  // check if validation was successfull
  GLint success = 0;
  glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
  if(success == 0) {
    // get log length
    GLint log_size = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_size);
    // get log
    std::vector<GLchar> log_buffer(log_size);
    glGetProgramInfoLog(program, log_size, &log_size, log_buffer.data());
    // output errors
    std::cerr << std::string{log_buffer.begin(), log_buffer.end()};
    // free broken program
    glDeleteProgram(program);

    throw std::logic_error("Validation of program nr. " + std::to_string(program));
  }
}

GLint get_bound_VAO() {
  GLint array = -1;
  glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &array);

  return array;
}

std::string file_name(std::string const& file_path) {
  return file_path.substr(file_path.find_last_of("/\\") + 1);
}

std::string read_file(std::string const& name) {
  std::ifstream ifile(name);

  if(ifile) {
    std::string filetext;
    
    while(ifile.good()) {
      std::string line;
      std::getline(ifile, line);
      filetext.append(line + "\n");
    }
    
    return filetext; 
  }
  else {
    std::cerr << "File \'" << name << "\' not found" << std::endl;
    
    throw std::invalid_argument(name);
  }
}

std::string read_resource_path(int argc, char* argv[]) {
  std::string resource_path{};
  //first argument is resource path
  if (argc > 1) {
    resource_path = argv[1];
  }
  // no resource path specified, use default
  else {
    std::string exe_path{argv[0]};
    resource_path = exe_path.substr(0, exe_path.find_last_of("/\\"));
    resource_path += "/../../resources/";
  }

  return resource_path;
}

glm::fmat4 calculate_projection_matrix(float aspect) {
  // float aspect = float(width) / float(height);
  // base fov does not change
  static const float fov_y_base = glm::radians(60.0f);
  float fov_y = fov_y_base;
  // if width is smaller, extend vertical fov 
  if (aspect < 1.0f) {
    fov_y = 2.0f * glm::atan(glm::tan(fov_y * 0.5f) * (1.0f / aspect));
  }
  // projection is hor+ 
  return glm::perspective(fov_y, aspect, 0.1f, 100.0f);
}

}
