#version 300 es

precision lowp float;

uniform sampler2D u_colorTexture;
uniform sampler2D u_positionTexture;
uniform sampler2D u_normalTexture;
uniform sampler2D u_depthTexture;
uniform sampler2D u_diffuseCoefTexture;
uniform sampler2D u_specularCoefTexture;
uniform float u_ambiantCoef;
uniform vec3 u_viewPos;
uniform vec3 u_sunLightDirection;

const vec3 k_specularColor = vec3(1.0);

in vec2 v_uv;

out vec4 out_color;

vec2 applySunLight(vec3 inWorldNormal, vec3 inWorldPos)
{
  inWorldNormal = normalize(inWorldNormal);

  float diffuseCoef = max(dot(-u_sunLightDirection, inWorldNormal.xyz), 0.0);
  float specularCoef = 0.0;

  if (diffuseCoef > 0.0)
  {
    // lighting specular

    vec3 reflectDir = reflect(-u_sunLightDirection, inWorldNormal);
    vec3 viewDir = normalize(u_viewPos - inWorldPos);

    float specAngle = max(dot(reflectDir, viewDir), 0.0);
    specularCoef = pow(specAngle, 64.0);
  }

  return vec2(max(u_ambiantCoef, diffuseCoef), specularCoef);
}

void main(void)
{
  vec4 tmpColor = texture(u_colorTexture, v_uv);
  vec4 tmpNormal = texture(u_normalTexture, v_uv);
  float tmpDepth = texture(u_depthTexture, v_uv).x;

  if (tmpNormal.w < 0.25)
  {
    out_color = vec4(tmpColor.rgb, 1.0); // raw color
  }
  else if (tmpDepth == 1.0)
  {
    out_color = vec4(tmpColor.rgb * u_ambiantCoef, 1.0); // ambiant color
  }
  else
  {
    vec4 tmpPosition = texture(u_positionTexture, v_uv);

    vec2 defaultCoefs = applySunLight(tmpNormal.xyz * 2.0 - 1.0, tmpPosition.xyz);

    float tmpDiffuseCoef = texture(u_diffuseCoefTexture, v_uv).x;
    float tmpSpecularCoef = texture(u_specularCoefTexture, v_uv).x;

    float diffuseCoef = min(max(defaultCoefs.x, tmpDiffuseCoef), 1.0);
    float specularCoef = max(defaultCoefs.y, tmpSpecularCoef * 8.0);

    out_color = vec4(tmpColor.rgb * diffuseCoef + k_specularColor * specularCoef, 1.0);
  }
}