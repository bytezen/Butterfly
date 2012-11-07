#pragma once
#include "cinder/CinderResources.h"

//#define RES_MY_RES			CINDER_RESOURCE( ../resources/, image_name.png, 128, IMAGE )
#define RES_PASSTHRU_VERT       CINDER_RESOURCE( ../src/shaders/, passThru_vert.glsl, 128, GLSL )
#define RES_CIRCLE_FRAG         CINDER_RESOURCE( ../src/shaders/, circle_frag.glsl, 129, GLSL )
#define RES_BUTTERFLY_FRAG      CINDER_RESOURCE( ../src/shaders/, butterfly_frag.glsl, 130, GLSL )
#define RES_FLAPPING_VERT      CINDER_RESOURCE( ../src/shaders/, flapping_vert.glsl, 131, GLSL )
#define RES_PASSTHRU_FRAG      CINDER_RESOURCE( ../src/shaders/, passThru_frag.glsl, 132, GLSL )
#define RES_BUTTERFLY_DECORATED_FS      CINDER_RESOURCE( ../src/shaders/, butterfly_decorated.fs, 133, GLSL )