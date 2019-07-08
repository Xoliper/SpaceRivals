###############################################################
##	1. Vertex Part				    	     
###############################################################

#vertex-start
#version 330

in vec3 vp;
uniform mat4 P, V, M;
out vec3 texcoords;

void main() {
  texcoords = vp;
  gl_Position = P * V * M * vec4(vp, 1.0);
}

#vertex-end

###############################################################
##	2. Fragment Part				     
###############################################################

#fragment-start
#version 330

in vec3 texcoords;
uniform samplerCube cube_texture;
out vec4 frag_colour;

void main() {
  frag_colour = texture(cube_texture, texcoords);
}

#fragment-end
