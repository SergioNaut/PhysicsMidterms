#version 430 core

layout(location=0) in vec4 in_Position;
layout(location=1) in vec4 in_Normal;
layout(location=2) in vec4 in_TexCoord;

out vec3 Normal;
out vec2 TexCoords;
out vec3 FragPos;

// per frame
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
// per item
uniform mat4 ModelMatrix;

void main()
{
  FragPos = vec3(ModelMatrix * in_Position);
  Normal = vec3(transpose(inverse(ModelMatrix)) * in_Normal);
  TexCoords = in_TexCoord.xy;
  gl_Position = ProjectionMatrix * ViewMatrix * vec4(FragPos, 1.0);
}
