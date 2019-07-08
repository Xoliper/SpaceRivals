###############################################################
##	1. Vertex Part				    	     
###############################################################

#vertex-start
#version 330 core

layout(location = 0) in vec3 VertexPosition;
uniform mat4 V;
uniform mat4 M;

void main(){
	gl_Position = M*V* vec4(VertexPosition,1);	
}
#vertex-end

###############################################################
##	2. Fragment Part				     
###############################################################

#fragment-start
#version 330 core

out vec4 color;
uniform vec4 PrimitiveColor;

void main(){
	color = vec4(PrimitiveColor.r, PrimitiveColor.g, PrimitiveColor.b, PrimitiveColor.a);
}
#fragment-end

