#version 150 core

in vec4 vertexColor;
in vec4 frontColor;
in vec4 backColor;

varying vec2 fragTex;

uniform sampler2D Tex;

out vec4 fragColor;

void main() {
   vec4 texColor = texture2D(Tex, fragTex);
   if (gl_FrontFacing)
       fragColor = frontColor * vertexColor;
   else
       fragColor = backColor * vertexColor;
  }
