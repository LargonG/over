#include "Common.cg"
vertout main(float4 gl_Vertex: POSITION,
               uniform float3 v3CameraPos,
               // The camera's current position
               uniform float3 v3LightDir,
               // Direction vector to the light source
               uniform float3 v3InvWavelength, // 1 / pow(wavelength, 4) for RGB
               uniform float fCameraHeight,
               // The camera's current height
               uniform float fCameraHeight2,
               // fCameraHeight^2
               uniform float fOuterRadius,
               // The outer (atmosphere) radius
               uniform float fOuterRadius2,
               // fOuterRadius^2
               uniform float fInnerRadius,
               // The inner (planetary) radius
               uniform float fInnerRadius2,
               // fInnerRadius^2
               uniform float fKrESun,
               // Kr * ESun
               uniform float fKmESun,
               // Km * ESun
               uniform float fKr4PI,
               // Kr * 4 * PI
               uniform float fKm4PI,
               // Km * 4 * PI
               uniform float fScale,
               // 1 / (fOuterRadius - fInnerRadius)
               uniform float fScaleOverScaleDepth) // fScale / fScaleDepth
{
  // Get the ray from the camera to the vertex and its length (which
  // is the far point of the ray passing through the atmosphere)
  float3 far = gl_Vertex.xyz;
  float3 direction = normalize(far - camera.position);

  // Calculate the closest intersection of the ray with
  // the outer atmosphere (point A in Figure 16-3)
  float fNear =
      getNearIntersection(v3CameraPos, direction, length(camera.position - planet_center) ** 2, planet_radius2);
  // Calculate the ray's start and end positions in the atmosphere,
  
  // then calculate its scattering offset
  float3 near = camera.position + direction * fNear;

  float3 ray = far - near;

  float start_angle = dot(ray, near) / sky_radius;
  float start_depth = exp(- 1 / h0); // exp(-fInvScaleDepth);
  float start_offset = start_depth * lookup[1, start_angle];

  // Initialize the scattering loop variables
  float ds = length(ray) / samples;
  float dscale = normalize(ray) / thickness; // ok...
  // float fSampleLength = fFar / fSamples;
  // float fScaledLength = fSampleLength * fScale;
  
  vec3 sample_step = normalize(ray) * ds;
  vec3 sample_point = near + sample_step * 0.5;
  
  // float3 v3SampleRay = v3Ray * fSampleLength;
  //float3 v3SamplePoint = v3Start + v3SampleRay * 0.5;
  // Now loop through the sample points
  float3 result = float3(0.0, 0.0, 0.0);
  for (int i = 0; i < nSamples; i++)
  {
    float len = length(sample_point);
    float depth = exp(- (1 / (h0 * (sky_radius - planet_radius)) * (len - planet_radius));

    float light_angle = dot(light_dir, sample_point) / len;
    float camera_angle = dot(normalize(ray), sample_point) / len;

    float scatter = lookup[h, light_angle] - lookup[h, camera_angle];

    vec3 attenuate = exp( - scatter * 4 * pi * (kernel_rayleigh + kernel_mie));
    // float fHeight = length(v3SamplePoint);
    // float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fHeight));
    // float fLightAngle = dot(v3LightDir, v3SamplePoint) / fHeight;
    // float fCameraAngle = dot(v3Ray, v3SamplePoint) / fHeight;
    //float fScatter =
     //   (fStartOffset + fDepth * (scale(fLightAngle) Ð scale(fCameraAngle)));
    //float3 v3Attenuate = exp(-fScatter * (v3InvWavelength * fKr4PI + fKm4PI));
    result += attenuate * depth * dscale;
    v3FrontColor += v3Attenuate * (fDepth * fScaledLength);
    v3SamplePoint += v3SampleRay;
  }
  // Finally, scale the Mie and Rayleigh colors
  vertout OUT;
  OUT.pos = mul(gl_ModelViewProjectionMatrix, gl_Vertex);
  OUT.c0.rgb = v3FrontColor * (v3InvWavelength * fKrESun);
  OUT.c1.rgb = v3FrontColor * fKmESun;
  OUT.t0 = v3CameraPos - v3Pos;
  return OUT;
}
