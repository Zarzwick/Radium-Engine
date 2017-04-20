#include "Structs.glsl"

layout (location = 0) in vec3 in_position;
layout (location = 5) in vec4 in_uc_ul;
layout (location = 6) in vec4 in_uq_p;

out vec4 out_color;

uniform Material material;

//-----------------------------------------------------
void main()
{
    // color
    out_color = vec4(1.0, 0.0, 0.0, 1.0);
}
