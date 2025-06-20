#version 450
layout(push_constant) uniform PushConstants {
    mat4 projection;
    mat4 modelview;
} pc;

layout(location = 0) in vec3 in_vertex;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_UV;
layout(location = 3) in vec4 in_color;

layout(location = 0) out vec3 v_pos_vs;
layout(location = 1) out vec3 v_view_dir_vs;
layout(location = 2) out vec3 v_normal_vs;
layout(location = 3) out vec2 v_UV;
layout(location = 4) out vec4 v_color;

void main()
{
    // vertex position in view-space
    vec4 pos_vs = pc.modelview * vec4(in_vertex, 1.0);
    gl_Position = pc.projection * pos_vs;
    
    v_pos_vs    = pos_vs.xyz;
    v_normal_vs = normalize(mat3(transpose(inverse(pc.modelview))) * in_normal);
    v_UV        = in_UV;
    v_color     = in_color;
    
    // get viwe dir in ortho and perspective
    if(abs(pc.projection[2][3]) < 1e-6)
        v_view_dir_vs = vec3(0.0, 0.0, 1.0);
    else
        v_view_dir_vs = - v_pos_vs;
}