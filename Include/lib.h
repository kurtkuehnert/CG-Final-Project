#if defined(_WIN32) && defined(__APPLE__)

#include "glew/glew.h"
#include "glut/freeglut.h"
#include "glm/glm.hpp"

#elif defined(__linux__)

#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>

#endif

using namespace std;
using namespace glm;