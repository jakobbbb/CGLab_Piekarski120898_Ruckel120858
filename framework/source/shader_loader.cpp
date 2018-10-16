#include "shader_loader.hpp"

#include "utils.hpp"


#include <glbinding/gl/functions.h>
// load meta info extension
#include <glbinding/Meta.h>
// use gl definitions from glbinding 
using namespace gl;

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string.h>


static std::string file_name(std::string const& file_path) {
  return file_path.substr(file_path.find_last_of("/\\") + 1);
}

namespace shader_loader {

GLuint shader(std::string const& file_path, GLenum shader_type) {
  GLuint shader = 0;
  shader = glCreateShader(shader_type);

  std::string shader_source{utils::read_file(file_path)};
  // glshadersource expects array of c-strings
  const char* shader_chars = shader_source.c_str();
  glShaderSource(shader, 1, &shader_chars, 0);

  glCompileShader(shader);

  // check if compilation was successfull
  GLint success = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if(success == 0) {
    // get log length
    GLint log_size = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_size);
    // get log
    std::vector<GLchar> log_buffer(log_size);
    glGetShaderInfoLog(shader, log_size, &log_size, log_buffer.data());
    // output errors
    std::cerr << "OpenGl error: Compilation of " << glbinding::Meta::getString(shader_type).c_str() << " " << file_name(file_path) << ":\n";
    std::cerr << std::string{log_buffer.begin(), log_buffer.end()};
    // free broken shader
    glDeleteShader(shader);

    throw std::logic_error("OpenGL error: compilation of " + file_name(file_path));
  }

  return shader;
}

unsigned program(std::map<GLenum, std::string> const& stages) {
  unsigned program = glCreateProgram();

  std::vector<GLuint> shaders{};
  // load and compile vert and frag shader
  for (auto const& stage : stages) {
    GLuint shader_handle = shader(stage.second, stage.first);
    shaders.push_back(shader_handle);
    // attach the shader to program
    glAttachShader(program, shader_handle);
  }

  // link shaders
  glLinkProgram(program);

  // check if linking was successfull
  GLint success = 0;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if(success == 0) {
    // get log length
    GLint log_size = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_size);
    // get log
    std::vector<GLchar> log_buffer(log_size);
    glGetProgramInfoLog(program, log_size, &log_size, log_buffer.data());
    
    // output errors
    std::string names{};
    for(auto const& stage : stages) {
      names += file_name(stage.second) + " & ";
    }
    names.resize(names.size() - 3);
        // output errors
    std::cerr << "OpenGl error: Linking of " << names << ":\n";
    std::cerr << std::string{log_buffer.begin(), log_buffer.end()};

    // free broken program
    glDeleteProgram(program);

    throw std::logic_error("OpenGL error: linking of " + names);
  }

  for (auto shader_handle : shaders) {
    // detach shader
    glDetachShader(program, shader_handle);
    // and free it
    glDeleteShader(shader_handle);
  }

  return program;
}

}
