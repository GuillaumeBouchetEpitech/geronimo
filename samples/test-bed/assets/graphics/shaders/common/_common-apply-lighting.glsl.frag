
// const float k_ambiant = 0.2;
// const vec3 k_lightPos = vec3(100.0,100.0,100.0);
// const vec3 k_lightPos = vec3(0.0,0.0,0.0);
const vec3 k_specColor = vec3(1.0, 1.0, 1.0);

vec4 applyLighting2(vec3 lightPos, vec4 currentColor, vec3 normal, vec3 worldPos, float ambiantCoef)
{
  normal = normalize(normal);
  vec3 lightDir = normalize(lightPos - worldPos);

  float diffuseCoef = max(dot(lightDir, normal.xyz), 0.0);

  // lighting output

  vec3 ambiant_color = currentColor.xyz * ambiantCoef;
  vec3 diffuse_color = currentColor.xyz * diffuseCoef;

  return vec4(ambiant_color + diffuse_color, currentColor.a);
}

vec4 applyLighting(vec4 currentColor, vec3 normal, vec3 worldPos, float ambiantCoef)
{
  return applyLighting2(vec3(0.0), currentColor, normal, worldPos, ambiantCoef);
}

vec4 applyLightingWithSpecular(vec4 currentColor, vec3 normal, vec3 worldPos, float ambiantCoef)
{
  normal = normalize(normal);
  // vec3 lightDir = normalize(k_lightPos - worldPos);
  vec3 lightDir = normalize(-worldPos);
  // vec3 lightDir = normalize(vec3(1,1,1));

  float diffuseCoef = max(dot(lightDir, normal.xyz), 0.0);
  float specularCoef = 0.0;

  if (diffuseCoef > 0.0)
  {
    // lighting specular

    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 viewDir = normalize(-worldPos);

    float specAngle = max(dot(reflectDir, viewDir), 0.0);
    specularCoef = pow(specAngle, 64.0);
  }

  // lighting output

  vec3 ambiantColor = currentColor.xyz * ambiantCoef;
  vec3 diffuseColor = currentColor.xyz * diffuseCoef;
  vec3 specularColor = k_specColor * specularCoef;

  return vec4(ambiantColor + diffuseColor + specularColor, currentColor.a);
}
