// Textures 0 -> 7 for internal use

layout(binding = 0) uniform sampler2D albedo_tex;
layout(binding = 1) uniform sampler2D normal_tex;
layout(binding = 2) uniform sampler2D roughness_tex;
layout(binding = 3) uniform sampler2D metalness_tex;
layout(binding = 4) uniform sampler2D ao_tex;
layout(binding = 7) uniform samplerCube environment_tex;
