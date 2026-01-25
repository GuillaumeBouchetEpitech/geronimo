#version 300 es

precision lowp float;

flat in vec4 v_color;
in vec3 v_worldSpacePosition;
in vec3 v_worldSpaceNormal;
flat in float v_lightMode;

layout(location = 0) out vec4 out_color;
layout(location = 1) out vec4 out_position;
layout(location = 2) out vec4 out_normal;

void main(void)
{
  float invAlpha = max(0.0, 1.0 - v_color.a);
  invAlpha = invAlpha == 0.0 ? 1.0 : invAlpha * 0.5;
  out_color = v_color;
  out_position = vec4(v_worldSpacePosition, 1.0);
  out_normal = vec4(v_worldSpaceNormal * 0.5 + 0.5, v_lightMode * 1.0 / invAlpha);
}
