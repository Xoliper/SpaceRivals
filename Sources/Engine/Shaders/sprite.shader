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
	gl_Position = MVP * vec4(VertexPosition,1);
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

void main(){
	color = texture(DiffuseTexture, UV).rgba;
}
#fragment-end
