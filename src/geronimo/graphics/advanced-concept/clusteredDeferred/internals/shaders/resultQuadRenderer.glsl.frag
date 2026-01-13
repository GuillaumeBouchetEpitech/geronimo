#version 300 es

precision lowp float;

uniform sampler2D u_colorTexture;
uniform sampler2D u_positionTexture;
uniform sampler2D u_normalTexture;
uniform sampler2D u_depthTexture;
uniform sampler2D u_clusterDataTexture;
uniform sampler2D u_lightsDataTexture;

uniform mat4 u_viewMatrix;
uniform float u_ambientCoef;
uniform vec3 u_viewPos;
uniform vec3 u_sunLightDirection;

uniform float u_screenWidth;
uniform float u_screenHeight;
uniform float u_camNear;
uniform float u_zStride;

uniform int u_numXSlices;
uniform int u_numYSlices;
uniform int u_maxLightsPerCluster;

const vec3 k_specularColor = vec3(1.0);

in vec2 v_uv;

out vec4 out_color;

float applySunLight_diffuse(vec3 inWorldNormal, vec3 inWorldPos)
{
  return max(dot(-u_sunLightDirection, inWorldNormal), 0.0);
}

float applySunLight_specular(vec3 inWorldNormal, vec3 inWorldPos)
{
  vec3 reflectDir = reflect(u_sunLightDirection, inWorldNormal);
  vec3 viewDir = normalize(u_viewPos - inWorldPos);
  float specAngle = max(dot(reflectDir, viewDir), 0.0);
  return pow(specAngle, 64.0);
}

// Cubic approximation of gaussian curve so we falloff to exactly 0 at the light radius
float cubicGaussian(float h)
{
  if (h < 1.0)
    return 0.25 * pow(2.0 - h, 3.0) - pow(1.0 - h, 3.0);
  if (h < 2.0)
    return 0.25 * pow(2.0 - h, 3.0);
  return 0.0;
}

void main(void)
{
  vec4 tmpColor4 = texture(u_colorTexture, v_uv);
  vec4 tmpNormal4 = texture(u_normalTexture, v_uv);
  float tmpDepth = texture(u_depthTexture, v_uv).x;

  if (tmpDepth >= 1.0)
  {
    discard;
  }

  if (tmpNormal4.w < 0.25)
  {
    // not lighting
    out_color = vec4(tmpColor4.rgb, 1.0);
  }
  else
  {
    vec3 tmpWorldPosition = texture(u_positionTexture, v_uv).xyz;

    bool specularEnabled = tmpNormal4.w > 0.5;

    vec3 rawNormal = normalize(tmpNormal4.xyz * 2.0 - 1.0);

    vec3 fpos = vec3(u_viewMatrix * vec4(tmpWorldPosition, 1.0)); //f_position in viewspace
    fpos.z = -fpos.z;


    float xStride = float(u_screenWidth) / float(u_numXSlices);
    float yStride = float(u_screenHeight) / float(u_numYSlices);
    float zStride = float(u_zStride);

    int clusterX_index = int( floor(v_uv.x * u_screenWidth / xStride) );
    int clusterY_index = int( floor(v_uv.y * u_screenHeight / yStride) );
    int clusterZ_index = int( floor( (fpos.z - u_camNear) / zStride ) );



    // int clusterId = clusterZ_index * u_numXSlices * u_numYSlices * (u_maxLightsPerCluster + 1) + clusterY_index * u_numXSlices + clusterX_index;



    int clusterIdX = clusterY_index * u_numXSlices + clusterX_index;
    int clusterIdY = clusterZ_index * (1 + u_maxLightsPerCluster);
    // int clusterId = clusterZ_index * u_numXSlices * u_numYSlices * (u_maxLightsPerCluster + 1) + clusterY_index * u_numXSlices + clusterX_index;



    // int clusterNumLights = int(texelFetch(u_clusterDataTexture, ivec2(clusterId, 0), 0).x);
    int clusterNumLights = int(texelFetch(u_clusterDataTexture, ivec2(clusterIdX, clusterIdY), 0).x);



    float bestDiffuseCoef = u_ambientCoef;
    float bestSpecularCoef = 0.0;

    int maxIterations = min(clusterNumLights, u_maxLightsPerCluster);
    for (int ii = 0; ii < maxIterations; ++ii)
    {
      // int lightIndex = int(texelFetch(u_clusterDataTexture, ivec2(clusterId, (1 + ii) * u_numXSlices * u_numYSlices), 0).x);
      int lightIndex = int(texelFetch(u_clusterDataTexture, ivec2(clusterIdX, clusterIdY + 1 + ii), 0).x);

      vec4 rawLightData = texelFetch(u_lightsDataTexture, ivec2(lightIndex, 0), 0);
      vec3 lightPos = rawLightData.xyz;
      float lightRadius = rawLightData.w;

      vec3 lightDiff = tmpWorldPosition - lightPos;
      float lightDistance = length(lightDiff);


      // if (lightDistance > lightRadius)
      //   continue;


      vec3 lightDir = lightDiff / lightDistance;

      // float lightIntensity = cubicGaussian(2.0 * lightDistance / lightRadius) * 4.0;
      // float lightIntensity = 8.0;
      // if (lightDistance > lightRadius)
      //   lightIntensity = 0.0;
      // float lightIntensity = 1.0 - (lightRadius * 1.1) / lightDistance; // [1..0]
      float lightIntensity = (1.0 - lightDistance / lightRadius) * 2.0; // [1..0]


      float lambertTerm = max(dot(-lightDir, rawNormal), 0.0);


      if (lambertTerm > 0.0)
      {

        bestDiffuseCoef = max(bestDiffuseCoef, lambertTerm * lightIntensity);


        if (specularEnabled)
        {
          vec3 reflectDir = reflect(lightDir, rawNormal);
          vec3 viewDir = normalize(u_viewPos - tmpWorldPosition);
          float specAngle = max(dot(reflectDir, viewDir), 0.0);
          float specularCoef = pow(specAngle, 64.0);

          bestSpecularCoef = max(bestSpecularCoef, specularCoef);
        }

      }

    }

    float sunDiffuseCoef = applySunLight_diffuse(rawNormal, tmpWorldPosition);
    float sunSpecularCoef = 0.0;
    if (specularEnabled && sunDiffuseCoef > 0.0)
      sunSpecularCoef = applySunLight_specular(rawNormal, tmpWorldPosition);

    float finalDiffuseCoef = min(max(sunDiffuseCoef, bestDiffuseCoef), 5.0);
    float finalSpecularCoef = max(sunSpecularCoef, bestSpecularCoef * 8.0);


    // fragColor += albedo * lambertTerm * light.color * vec3(lightIntensity) + specular*specColor;

    // int tmpClusterId = (2 * u_numXSlices * u_numYSlices + 5 * u_numXSlices + 5) * (u_maxLightsPerCluster + 1);

    // if (clusterNumLights > 0)
    //   tmpColor4.rgb += vec3(0.2, 0.0, 0.0);

    // if (clusterNumLights > 0)
    // // if (clusterId == tmpClusterId)
    //   out_color = vec4(1.0, 1.0, 1.0, 1.0);
    // else
    out_color = vec4(tmpColor4.rgb * finalDiffuseCoef + k_specularColor * finalSpecularCoef, 1.0);

  }

}