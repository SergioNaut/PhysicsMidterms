#pragma once


// include glm
#define GLM_ENABLE_EXPERIMENTAL  // for euler_angles.hpp

//#include "glm\fwd.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/euler_angles.hpp>
//#include <glm/gtx/projection.hpp>
//#include <glm/gtx/matrix_interpolation.hpp>

// glm extensions
//#include <glm/ext/matrix_transform.hpp>  // glm::translate, glm::rotate, glm::scale
//#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
//#include <glm/ext/scalar_constants.hpp>  // glm::pi, glm::epsilon

// personal glm "extensions"
#include <glm/james/mat4stack.h>
#include <glm/james/inline_helpers.h>
#include <glm/james/aabb.h>
#include <glm/james/rotation_from_to.h>




// include glew first
#define GLEW_STATIC
#include <GL\glew.h>
// include glfw
#include <GLFW\glfw3.h>

// FreeImage
#ifndef FREEIMAGE_LIB
#define FREEIMAGE_LIB
#endif // !FREEIMAGE_LIB

#include <FreeImage.h>