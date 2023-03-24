#version 300 es

precision lowp float;

in vec4 v_color;
in vec3 v_worldSpacePosition;
in float v_worldSpaceNormalLength;
in vec3 v_worldSpaceNormal;

layout(location = 0) out vec4 out_color;
layout(location = 1) out vec4 out_position;
layout(location = 2) out vec4 out_normal;

void main(void)
{
  out_color = vec4(v_color.xyz, 0.0);
  out_position = vec4(v_worldSpacePosition, 1.0);
  out_normal = vec4(v_worldSpaceNormal * 0.5 + 0.5, v_worldSpaceNormalLength);
}
