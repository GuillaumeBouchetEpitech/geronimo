#version 300 es

precision highp float;

uniform mat4 u_composedMatrix;

in vec3 a_vertexPosition;
in vec3 a_vertexNormal;

in vec3 a_offsetPosition;
in vec4 a_offsetOrientation; // quaternion
in vec3 a_offsetScale;
in vec4 a_offsetColor;

out vec4 v_color;
out vec3 v_worldSpacePosition;
out float v_worldSpaceNormalLength;
out vec3 v_worldSpaceNormal;

#include "./assets/graphics/shaders/common/_common-quat-rotations.glsl.vert"

void main(void)
{
	vec3 worldSpacePosition = a_offsetPosition + apply_quat_to_vec3(a_vertexPosition * a_offsetScale, a_offsetOrientation);
	vec3 worldSpaceNormal = apply_quat_to_vec3(a_vertexNormal, a_offsetOrientation);

	gl_Position = u_composedMatrix * vec4(worldSpacePosition, 1.0);

	v_color = a_offsetColor;
	v_worldSpacePosition = worldSpacePosition;
	v_worldSpaceNormalLength = length(worldSpaceNormal);
	if (v_worldSpaceNormalLength > 0.0)
		v_worldSpaceNormal = worldSpaceNormal / v_worldSpaceNormalLength;
	else
		v_worldSpaceNormal = vec3(0.0);
}
