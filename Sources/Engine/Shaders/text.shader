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
uniform vec4 TextColor;

void main(){
  color =  vec4(TextColor.r,TextColor.g,TextColor.b,TextColor.a)*vec4(1,1,1,texture(DiffuseTexture, UV).r);
}

#fragment-end


