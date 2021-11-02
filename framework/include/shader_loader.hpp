#ifndef SHADER_LOADER_HPP
#define SHADER_LOADER_HPP

#include <map>
#include <string>

#include <glbinding/gl/enum.h>
using namespace gl;

namespace shader_loader {
// compile shader
unsigned shader(std::string const& file_path, GLenum shader_type);
// create program from given list of stages
unsigned program(std::map<GLenum, std::string> const&);
}  // namespace shader_loader

#endif
