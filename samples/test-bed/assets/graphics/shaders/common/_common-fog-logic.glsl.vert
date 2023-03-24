
const float fogStart = 195.0;
// const float fogScale = 5.0;
// const float fogStart = 45.0;
const float fogScale = 15.0;

// float FogEyePlane(vec4 Rh)
// {
// 	vec4 Re = Rh / Rh.w;
// 	float z = -Re.z;
// 	return clamp((z - fogStart) * fogScale, 0.0, 1.0);
// }

// float FogEyeAbsolute(vec4 Rh)
// {
// 	vec4 Re = Rh / Rh.w;
// 	float z = abs(Re.z);
// 	return clamp((z - fogStart) * fogScale, 0.0, 1.0);
// }

float FogEyeRadial(vec4 Rh)
{
	vec4 Re = Rh / Rh.w;
	float z = length(Re);
	return clamp((z - fogStart) * fogScale, 0.0, 1.0);
}
