#version 300 es

precision lowp float;

// uniform sampler2D u_colorTexture;
uniform sampler2D u_positionTexture;
uniform sampler2D u_normalTexture;
uniform sampler2D u_depthTexture;

uniform vec2 u_resolution;

in vec3 v_lightPos;
flat in float v_lightRadius;

//
//
//

float applyLighting(vec3 inWorldPos, vec3 inWorldNormal)
{
  vec3 preLightDirDiff = inWorldPos - v_lightPos;
  float magnitude = length(preLightDirDiff);
  if (magnitude == 0.0 || magnitude > v_lightRadius)
    return 0.0;
  vec3 lightDir = preLightDirDiff / magnitude; // normalize

  //

  float distanceCoef = 1.0 - (v_lightRadius * 1.1) / magnitude; // [1..0]

  return max(0.0, dot(lightDir, inWorldNormal) * distanceCoef);
}

//
//
//

void main(void)
{
  // vec2 uv = gl_FragCoord.xy;
  vec2 uv = vec2(gl_FragCoord.x / u_resolution.x, gl_FragCoord.y / u_resolution.y);

  // vec3 tmpColor = texture(u_colorTexture, uv).rgb;
  vec3 tmpPosition = texture(u_positionTexture, uv).xyz;
  vec4 tmpNormal = texture(u_normalTexture, uv);
  float tmpDepth = texture(u_depthTexture, uv).x;

  if (tmpDepth == 1.0)
  // if (tmpDepth == 1.0 || tmpDepth >= gl_FragCoord.z)
  {
    discard;
    // gl_FragDepth = 0.0;
  }
  else
  if (tmpNormal.w == 0.0)
  {
    // ignore light
    // gl_FragDepth = 1.0;
    // out_color = vec4(tmpColor, 1.0);
    discard;
  }
  else
  {
    gl_FragDepth = applyLighting(tmpPosition, tmpNormal.xyz * 2.0 - 1.0);
  }
}
