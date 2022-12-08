#version 330 core

in vec4 position; // raw position in the model coord
in vec3 normal;   // raw normal in the model coord

uniform mat4 modelview; // from model coord to eye coord
uniform mat4 view;      // from world coord to eye coord

// Material parameters
uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform vec4 emision;
uniform float shininess;

// Light source parameters
const int maximal_allowed_lights = 10;
uniform bool enablelighting;
uniform int nlights;
uniform vec4 lightpositions[ maximal_allowed_lights ];
uniform vec4 lightcolors[ maximal_allowed_lights ];

// Output the frag color
out vec4 fragColor;


void main (void){
    if (!enablelighting){
        // Default normal coloring (you don't need to modify anything here)
        vec3 N = normalize(normal);
        fragColor = vec4(0.5f*N + 0.5f , 1.0f);
    } else {
        vec4 sumColor = vec4(0.0f);     //sum of the color values for each light source

        for (int i = 0; i < nlights; i++) {
            //get necessary points for calculating v,n,l

            vec4 p_cam = modelview * position;       //position in camera coords
            vec4 q_cam = view * lightpositions[i];   //lightpos in camera coords

            //get the vectors v (model to cam), n (model's normal), l (model to light) in eye (camera) coords
            vec3 l_cam = normalize( (p_cam.w * q_cam.xyz) - (q_cam.w * p_cam.xyz) );
            vec3 v_cam = normalize( -p_cam.xyz );
            vec3 n_cam = normalize( mat3(transpose(inverse(modelview))) * normal );
            vec3 h_cam = normalize( v_cam + l_cam );
            
            //add ambient, diffuse, and specular
            vec4 tempColor = vec4(0.0f);
            tempColor   += ambient;
            tempColor   += diffuse  * max( 0.0f, dot(l_cam, n_cam) );
            tempColor   += specular * pow( max(0.0f, dot(n_cam, h_cam)), shininess );   

            //multiply by light color and add to sumColor
            sumColor += lightcolors[i] * tempColor;
        }

        fragColor = sumColor + emision;
    }
}

