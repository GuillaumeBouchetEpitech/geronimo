#version 300 es

precision highp float;

uniform mat4 u_composedMatrix;

in vec3  a_vertex_position;

in vec3  a_offset_lightPosition;
in vec3  a_offset_lightColor;
in float a_offset_lightRadius;

flat out vec3 v_lightPos;
flat out vec3 v_lightColor;
flat out float v_lightRadius;

void main(void)
{
  vec3 worldSpacePosition = a_offset_lightPosition + a_vertex_position * a_offset_lightRadius;

  gl_Position = u_composedMatrix * vec4(worldSpacePosition, 1.0);

	v_lightPos = a_offset_lightPosition;
  v_lightColor = a_offset_lightColor;
  v_lightRadius = a_offset_lightRadius;

}
