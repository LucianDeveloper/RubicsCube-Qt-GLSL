#version 330 core

in vec4 vertexColor;
in vec4 frontColor;
in vec4 backColor;


out vec4 fragColor;

void main() {
   if (gl_FrontFacing)
       fragColor = frontColor * vertexColor;
   else
       fragColor = backColor * vertexColor;
}
