#version 450

// vsg default light
layout(constant_id = 3) const int LIGHT_DATA_SIZE = 256;
layout(set = 0, binding = 0) uniform LightData {
    vec4 values[LIGHT_DATA_SIZE];
} light_data;

//layout(set = 1, binding = 0) uniform sampler2D diffuse_sampler;
//layout(set = 1, binding = 1) uniform sampler2D normal_sampler;

//layout(set = 1, binding = 2) uniform MaterialData {
//    vec4  ambient_color;
//    vec4  diffuse_color;
//    vec4  specular_color;
//    vec4  emissive_color;
//    float shininess;
//    float alpha_mask;
//    float alpha_mask_cutoff;
//} material;

layout(location = 0) in vec3 v_pos_vs;
layout(location = 1) in vec3 v_view_dir_vs;
layout(location = 2) in vec3 v_normal_vs;
layout(location = 3) in vec2 v_UV;
layout(location = 4) in vec4 v_color;

layout(location = 0) out vec4 out_color;

const float BRIGHTNESS_CUTOFF = 0.001;

//vec3 calculate_normal(void)
//{
//    // sample tangent-space normal
//    vec3 t_normal = texture(normal_sampler, v_UV).xyz * 2.0 - 1.0;
//
//    vec3 N = normalize(v_normal_vs);
//
//    // derivative–based tangent basis
//    vec3 q1 = dFdx(v_pos_vs);
//    vec3 q2 = dFdy(v_pos_vs);
//    vec2 st1 = dFdx(v_UV);
//    vec2 st2 = dFdy(v_UV);
//
//    vec3 T = normalize(q1 * st2.t - q2 * st1.t);
//    vec3 B = normalize(cross(N, T));
//    mat3 TBN = mat3(T, B, N);
//
//    return normalize(TBN * t_normal);
//}
//
//vec3 blinn_phong(vec3 L, vec3 N, vec3 V, vec3 light_color, float brightness)
//{
//    // diffuse
//    float NdotL = max(dot(N, L), 0.0);
//    vec3 diffuse = light_color * NdotL;
//
//    // specular
//    vec3 H = normalize(L + V);
//    float NdotH = max(dot(N, H), 0.0);
//    vec3 specular = light_color * pow(NdotH, material.shininess);
//
//    return brightness * (diffuse * material.diffuse_color.rgb + specular * material.specular_color.rgb);
//}

void main(void)
{
//    // get base color
//    vec4 texture_diffuse = texture(diffuse_sampler, v_UV);
//    vec4 base_color      = v_color * material.diffuse_color * texture_diffuse;
//
//    // alpha-test
//    if (material.alpha_mask > 0.5 && base_color.a < material.alpha_mask_cutoff)
//        discard;
//
//    vec3 N = calculate_normal();
//    vec3 V = normalize(v_view_dir_vs);
//
//    // light stuff
//    vec3 accum = vec3(0.0);
//
//    vec4 counters        = light_data.values[0];
//    int  num_ambient     = int(counters.x);
//    int  num_directional = int(counters.y);
//    int  num_point       = int(counters.z);
//    int  num_spot        = int(counters.w);
//
//    int idx = 1;                // running pointer inside values[]
//
//    //-------------------- ambient --------------------
//    for (int i = 0; i < num_ambient; ++i)
//    {
//        vec4 lc = light_data.values[idx++];          // rgb, a=intensity
//        accum  += lc.rgb * lc.a * material.ambient_color.rgb * base_color.rgb;
//    }
//
//    //-------------------- directional ----------------
//    for (int i = 0; i < num_directional; ++i)
//    {
//        vec4 lc  = light_data.values[idx++];          // color + brightness
//        vec3 dir = -light_data.values[idx++].xyz;    // stored as -L
//
//        float brightness = lc.a;
//        // shadow map count (ignore shadows in this minimal shader)
//        int sm_count = int(light_data.values[idx].r);
//        idx += 1 + 8 * sm_count;
//
//        if (brightness <= BRIGHTNESS_CUTOFF) 
//            continue;
//
//        vec3 L = normalize(dir);
//        accum += blinn_phong(L, N, V, lc.rgb * base_color.rgb, brightness);
//    }
//
//    //-------------------- point ----------------------
//    for (int i = 0; i < num_point; ++i)
//    {
//        vec4 lc  = light_data.values[idx++];
//        vec3 pos = light_data.values[idx++].xyz;
//
//        vec3 delta  = pos - v_pos_vs;
//        float dist2 = dot(delta, delta);
//        float brightness = lc.a / dist2;
//        if (brightness <= BRIGHTNESS_CUTOFF)
//            continue;
//
//        vec3 L = normalize(delta);
//        accum += blinn_phong(L, N, V, lc.rgb * base_color.rgb, brightness);
//    }
//
//    //-------------------- spot -----------------------
//    for (int i = 0; i < num_spot; ++i)
//    {
//        vec4 lc   = light_data.values[idx++];
//        vec4 posI = light_data.values[idx++];      // xyz + cosInner
//        vec4 dirO = light_data.values[idx++];      // xyz + cosOuter
//
//        vec3 delta  = posI.xyz - v_pos_vs;
//        float dist  = length(delta);
//        float dist2 = dist * dist;
//        vec3  L     = delta / dist;
//
//        float cos_angle = dot(dirO.xyz, -L);
//        float falloff   = smoothstep(dirO.w, posI.w, cos_angle);
//
//        float brightness = lc.a * falloff / dist2;
//
//        int sm_count = int(light_data.values[idx].r);
//        idx += 1 + 8 * sm_count;
//
//        if (brightness <= BRIGHTNESS_CUTOFF) 
//            continue;
//
//        accum += blinn_phong(L, N, V, lc.rgb * base_color.rgb, brightness);
//    }
//
//    // final color
//    vec3 rgb = accum + material.emissive_color.rgb;
//    out_color = vec4(rgb, base_color.a);
    out_color = v_color;
}
