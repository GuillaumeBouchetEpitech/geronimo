#version 300 es

precision lowp float;

uniform sampler2D u_colorTexture;
uniform sampler2D u_positionTexture;
uniform sampler2D u_normalTexture;
uniform sampler2D u_depthTexture;
uniform sampler2D u_diffuseColorTexture;
// uniform sampler2D u_specularColorTexture;
uniform float u_ambientCoef;
uniform vec3 u_viewPos;
uniform vec4 u_sunLight;

// const vec3 k_specularColor = vec3(1.0);

in vec2 v_uv;

out vec4 out_color;

vec2 applySunLight(vec3 inWorldNormal, vec3 inWorldPos)
{
  inWorldNormal = normalize(inWorldNormal);

  vec3 lightDir = vec3(0,0,0);
  if (u_sunLight.w == 0.0) {
    // sunLight is a direction
    lightDir = u_sunLight.xyz;
  } else {
    // sunLight is a position
    vec3 preLightDirDiff = inWorldPos - u_sunLight.xyz;
    float magnitude = length(preLightDirDiff);
    if (magnitude == 0.0) {
      return vec2(0.0, 0.0);
    }
    lightDir = preLightDirDiff / magnitude; // normalize
  }

  float diffuseCoef = max(dot(-lightDir, inWorldNormal.xyz), 0.0);
  float specularCoef = 0.0;

  // if (diffuseCoef > 0.0)
  // {
  //   // lighting specular

  //   vec3 reflectDir = reflect(-lightDir, inWorldNormal);
  //   vec3 viewDir = normalize(u_viewPos - inWorldPos);

  //   float specAngle = max(dot(reflectDir, viewDir), 0.0);
  //   specularCoef = pow(specAngle, 64.0);
  // }

  // return vec2(max(u_ambientCoef, diffuseCoef), specularCoef);
  return vec2(diffuseCoef, specularCoef);
}

void main(void)
{
  vec4 tmpColor = texture(u_colorTexture, v_uv);
  vec4 tmpNormal = texture(u_normalTexture, v_uv);
  float tmpDepth = texture(u_depthTexture, v_uv).x;

  // if (tmpDepth == 1.0)
  // {
  //   // out of depth range
  //   discard;
  // }
  // else
  if (tmpNormal.w < 0.25)
  {
    // no light (emissive)
    out_color = tmpColor; // raw color
  }
  else
  {
    // got light (diffuse + specular)

    vec4 tmpPosition = texture(u_positionTexture, v_uv);

    vec2 defaultCoefs = applySunLight(tmpNormal.xyz * 2.0 - 1.0, tmpPosition.xyz);

    vec4 tmpDiffuseColor = texture(u_diffuseColorTexture, v_uv);
    // vec4 tmpSpecularColor = texture(u_specularColorTexture, v_uv);
    float tmpDiffuseCoef = tmpDiffuseColor.w;
    // float tmpSpecularCoef = tmpSpecularColor.w;

    float sunLightDiffuse = min(defaultCoefs.x, 1.0);
    float diffuseCoef = min(tmpDiffuseCoef, 1.0);
    // float specularCoef = max(defaultCoefs.y, tmpSpecularCoef * 8.0);

    float startCoef = max(u_ambientCoef, sunLightDiffuse);
    vec3 startColor = tmpColor.rgb * startCoef;

    float extraCoef = diffuseCoef - startCoef;
    startColor += tmpDiffuseColor.rgb * max(0.0, extraCoef);

    out_color = vec4(startColor /*+ tmpSpecularColor.rgb * specularCoef*/, tmpColor.a);
  }

  gl_FragDepth = tmpDepth;
}
