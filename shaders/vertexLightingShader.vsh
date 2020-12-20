#version 330 core


in vec3 posVertex;
in vec4 colorVertex;
in vec3 normalVertex;

out vec4 vertexColor;
out vec4 frontColor;
out vec4 backColor;


struct t_lightInfo
  {
    vec3 position;
    vec3 La;
    vec3 Ld;
    vec3 Ls;
  };

struct t_materialInfo
  {
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    float shininess;
  };

uniform mat4 proectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;
uniform vec3 frontCamera;
uniform t_lightInfo lights[3];
uniform t_materialInfo material;

vec4 phongModel(int lightIndex, vec4 position, vec3 normal) {
    vec3 s = normalize(lights[lightIndex].position - vec3(position));
    vec3 v = normalize(frontCamera - position.xyz);
    vec3 r = reflect(-s, normal);
    vec3 ambient = lights[lightIndex].La * material.Ka;
    float sDotN = max(dot(s, normal), 0.0f);
    vec3 diffuse = lights[lightIndex].Ld * material.Kd * sDotN;
    vec3 spec = vec3(0.0f);

    if (sDotN > 0.0)
        spec = lights[lightIndex].Ls * material.Ks * pow(max(dot(r, v), 0.0f), material.shininess);

    return vec4(ambient + diffuse + spec, 0.0f);
  }

void main()
  {
   vec3 eyeNorm = normalize(normalMatrix * normalVertex);
   vec4 eyePosition = modelViewMatrix * vec4(posVertex, 1.0f);

   frontColor = vec4(0.0f);
   backColor = vec4(0.0f);
   for (int i = 0; i < 3; i++) {
       frontColor += 3*phongModel(i, eyePosition, eyeNorm);
       backColor  += 3*phongModel(i, eyePosition, -eyeNorm);
   }

   vertexColor = colorVertex;
   gl_Position = proectionMatrix * vec4(posVertex, 1.0);
}
