###############################################################
##	1. Vertex Part				    	     
###############################################################

#vertex-start
#version 330 core

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec2 VertexUV;
out vec2 UV;

uniform mat4 V;
uniform mat4 M;
uniform mat4 MVP;

void main(){
	gl_Position = M*V* vec4(VertexPosition,1);
	UV = VertexUV;
}
#vertex-end

###############################################################
##	2. Fragment Part				     
###############################################################

#fragment-start
#version 330 core

in vec2 UV;
out vec4 color;

uniform sampler2D DiffuseTexture;
uniform sampler2D depthBuffTex;
uniform vec2 TexOffset;

float kernel[9] = float[](
	0.0204001988,
	0.0577929595,
	0.1215916882,
	0.1899858519,
	0.2204586031,
	0.1899858519,
	0.1215916882,
	0.0577929595,
	0.0204001988
);


void main(){
	
	float depth = texture(depthBuffTex, UV).r;
	if(depth > 0.83 && depth < 0.9999){
 		kernel[5] = 0.4504586031 * pow(depth, 5);
    }
    
    vec4 sum = vec4(0.0);
    for (int i = -4; i <= 4; i++){
  		sum += texture(DiffuseTexture, UV + i * TexOffset) * kernel[i + 4];
    }
  	color = sum;

}
#fragment-end
