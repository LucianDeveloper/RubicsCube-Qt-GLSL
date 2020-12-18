#version 150 core

in vec4 posVertex;
in vec4 colorVertex;
in vec4 normalVertex;

out vec3 vertexColor;
out vec3 frontColor;
out vec3 backColor;

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
uniform int countLights;
uniform t_lightInfo lights[countLights];
uniform t_materialInfo material;



vec3 phongModel(int lightIndex, vec4 position, vec3 normal)
  {

    vec3 s = normalize(lights[lightIndex].position - vec3(position));
    vec3 v = normalize(-position.xyz);
    vec3 r = reflect(-s, normal);
    vec3 ambient = lights[lightIndex].La * material.Ka;
    float sDotN = max(dot(s, normal), 0.0f);
    vec3 diffuse = lights[lightIndex].Ld * material.Kd * sDotN;
    vec3 spec = vec3(0.0f);

    if (sDotN > 0.0)
        spec = lights[lightIndex].Ls * material.Ks * pow(max(dot(r, v), 0.0f), material.shininess);

    return ambient + diffuse + spec;
  }

void main(void)
  {

   vec3 eyeNorm = normalize(normalMatrix * normalVertex);
   vec4 eyePosition = modelViewMatrix * vec4(posVertex, 1.0f);

   frontColor = vec3(0.0f);
   backColor = vec3(0.0f);
   for (int i = 0; i < countLights; i++)
     {
       frontColor += phongModel(0, eyePosition, eyeNorm);
       backColor += phongModel(0, eyePosition, -eyeNorm);
     }

   vertexColor = colorVertex;

   gl_Position = proectionMatrix * posVertex;
  }
