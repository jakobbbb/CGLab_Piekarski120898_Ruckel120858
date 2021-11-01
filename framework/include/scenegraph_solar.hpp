#ifndef SCENEGRAPH_SOLAR_HPP
#define SCENEGRAPH_SOLAR_HPP

#include "SceneGraph.hpp"

#define RAND_FLOAT() ((float)std::rand() / (float)RAND_MAX)

const std::string PLANET_NAMES[] = {
  "Mercury",
  "Venus",
  "Earth",
  "Mars",
  "Jupiter",
  "Saturn",
  "Uranus",
  "Neptune",
};
const float ORBIT_PERIODS[] = {
  .240846f,
  .615f,
  1.0f,
  .748f, // Moon
  1.881f,
  11.86f,
  29.46f,
  85.01f,
  164.8f,
};
const glm::vec3 SUN_AXIS{0, 1, 0};

SceneGraph make_solar_scene();

#endif  // SCENEGRAPH_SOLAR_HPP
