#version 300 es

precision highp float;

uniform mat4 u_composedMatrix;

in vec3  a_vertex_position;
in vec2  a_vertex_uv;

out vec2 v_uv;

void main(void)
{
  gl_Position = u_composedMatrix * vec4(a_vertex_position, 1.0);

  v_uv = a_vertex_uv;
}
