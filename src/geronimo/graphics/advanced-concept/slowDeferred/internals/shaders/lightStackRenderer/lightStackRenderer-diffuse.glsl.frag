#version 300 es

precision lowp float;

uniform sampler2D u_positionTexture;
uniform sampler2D u_normalTexture;

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
  vec2 uv = vec2(gl_FragCoord.x / u_resolution.x, gl_FragCoord.y / u_resolution.y);

  vec4 tmpNormal = texture(u_normalTexture, uv);

  if (
    tmpNormal.w < 0.25 ||
    gl_FragCoord.z == 1.0
  ) {
    discard;
  }

  vec3 tmpPosition = texture(u_positionTexture, uv).xyz;

  gl_FragDepth = applyLighting(tmpPosition, tmpNormal.xyz * 2.0 - 1.0);
}

