#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;
in vec4 VerColor;
  
uniform vec3 lightPos; 
uniform vec3 lightColor;
//uniform vec3 objectColor;

void main()
{
    // ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
            
    vec4 result = vec4((ambient + diffuse), 1.0) * VerColor;
    FragColor = result;
} 
