#version 300 es

precision lowp float;

uniform sampler2D u_depthTexture_opaque;
uniform sampler2D u_depthTexture_transparency;

uniform sampler2D u_colorTexture_opaque;
uniform sampler2D u_colorTexture_transparency;

in vec2 v_uv;

out vec4 out_color;

void main(void)
{
  float opaqueDepth = texture(u_depthTexture_opaque, v_uv).x;
  float transparencyDepth = texture(u_depthTexture_transparency, v_uv).x;

  vec4 opaqueColor = texture(u_colorTexture_opaque, v_uv);
  vec4 transparencyColor = texture(u_colorTexture_transparency, v_uv);

  float ratio = (opaqueDepth < transparencyDepth)
    // is opaque
    ? 0.0
    // is transparent
    : transparencyColor.a;

  out_color = vec4(mix(opaqueColor.rgb, transparencyColor.rgb, ratio), 1.0);
}
