// #version 300 es

// precision lowp float;

// uniform sampler2D u_positionTexture;
// uniform sampler2D u_normalTexture;
// // uniform sampler2D u_diffuseCoefTexture;
// uniform vec3 u_viewPos;
// uniform sampler2D u_diffuseColorTexture;

// uniform vec2 u_resolution;

// flat in vec3 v_lightPos;
// flat in vec3 v_lightColor;
// flat in float v_lightRadius;

// out vec4 out_color;

// //
// //
// //

// float applyLighting(vec3 inWorldPos, vec3 inWorldNormal)
// {
//   vec3 preLightDirDiff = inWorldPos - v_lightPos;
//   float magnitude = length(preLightDirDiff);
//   if (magnitude == 0.0 || magnitude > v_lightRadius)
//     return 0.0;
//   vec3 lightDir = preLightDirDiff / magnitude; // normalize

//   //

//   // float distanceCoef = 1.0 - v_lightRadius / magnitude; // [1..0]
//   // float distanceCoef = 1.0;

//   vec3 reflectDir = reflect(lightDir, inWorldNormal);
//   vec3 viewDir = normalize(u_viewPos - inWorldPos);

//   float specAngle = max(dot(reflectDir, viewDir), 0.0);
//   return max(0.0, pow(specAngle, 8.0) * 0.125);
// }

// //
// //
// //

// void main(void)
// {
//   vec2 uv = vec2(gl_FragCoord.x / u_resolution.x, gl_FragCoord.y / u_resolution.y);

//   vec4 tmpNormal = texture(u_normalTexture, uv);
//   // float tmpDiffuseCoef = texture(u_diffuseCoefTexture, uv).x;
//   float tmpDiffuseCoef = texture(u_diffuseColorTexture, uv).a;

//   if (
//     // no diffuse light intensity to enhance
//     tmpDiffuseCoef == 0.0 ||
//     // specular light disabled
//     tmpNormal.w < 0.75 ||
//     // out of depth range
//     gl_FragCoord.z == 1.0
//   ) {
//     discard;
//   }

//   vec3 tmpPosition = texture(u_positionTexture, uv).xyz;

//   float lightIntensity = applyLighting(tmpPosition, tmpNormal.xyz * 2.0 - 1.0);

//   // this let the graphic pipeline keep the brightest light
//   gl_FragDepth = lightIntensity;

//   out_color = vec4(v_lightColor, lightIntensity);
// }
