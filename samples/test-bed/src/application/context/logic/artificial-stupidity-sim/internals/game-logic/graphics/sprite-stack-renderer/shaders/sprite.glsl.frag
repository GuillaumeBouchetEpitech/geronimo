#version 300 es

precision lowp float;

uniform sampler2D u_texture;

in vec2 v_texCoord;
in vec3 v_color;
in vec3 v_worldSpacePosition;

layout(location = 0) out vec4 out_color;
layout(location = 1) out vec4 out_position;
layout(location = 2) out vec4 out_normal;

void main(void)
{
  vec4 texColor = texture(u_texture, v_texCoord);

  if (texColor.a < 0.1)
  {
    discard;
  }
  else
  {
    out_color = vec4(texColor.xyz * v_color.xyz, texColor.a);
    // out_color = vec4(texColor.xyz * v_color.xyz * 0.1 + vec3(0.9), texColor.a * 0.1 + 0.9);
  }

	out_color = vec4(v_color.xyz, 0.0);
	out_position = vec4(v_worldSpacePosition, 1.0);
	out_normal = vec4(0.0);
}
