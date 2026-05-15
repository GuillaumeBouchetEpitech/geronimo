#version 300 es

precision highp float;

uniform mat4 u_composedMatrix;

in vec3 a_vertexPosition;
in vec2 a_vertexTexCoord;
in vec3 a_vertexColor;

out vec2 v_texCoord;
out vec3 v_color;
out vec3 v_worldSpacePosition;

void main(void)
{
  gl_Position = u_composedMatrix * vec4(a_vertexPosition, 1.0);

  v_texCoord = a_vertexTexCoord;
  v_color = a_vertexColor;
	v_worldSpacePosition = a_vertexPosition;
}
