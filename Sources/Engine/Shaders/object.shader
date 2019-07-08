###############################################################
##	1. Vertex Part				    	     
###############################################################

#vertex-start
#version 330 core

// Input vertex data, different for all executions of this shader (Binded data by C++).
layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec2 VertexUV;
layout(location = 2) in vec3 VertexNormal;

// Output data (will go to fragment shader)
out vec2 UV;
out vec3 PositionWorldspace;
out vec3 NormalCameraspace; // to wyjebac
out vec3 EyeDirectionCameraspace;
out vec3 LightDirectionCameraspace;

// Values that stay constant for the whole mesh. (Passed by C++)
uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform vec3 LightPosition;

void main(){

	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  MVP * vec4(VertexPosition,1);
	
	// Position of the vertex, in worldspace : M * position
	PositionWorldspace = (M * vec4(VertexPosition,1)).xyz;
	
	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	vec3 VertexPositionCameraspace = ( V * M * vec4(VertexPosition,1)).xyz;
	EyeDirectionCameraspace = vec3(0,0,0) - VertexPositionCameraspace;

	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	vec3 LightPositionCameraspace = ( V * vec4(LightPosition,1)).xyz;
	LightDirectionCameraspace = LightPositionCameraspace + EyeDirectionCameraspace;
	
	// Normal of the the vertex, in camera space
	NormalCameraspace = ( V * M * vec4(VertexNormal,0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
	
	// UV of the vertex. No special space for this one.
	UV = VertexUV;
}
#vertex-end

###############################################################
##	2. Fragment Part				     
###############################################################

#fragment-start
#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 PositionWorldspace;
in vec3 NormalCameraspace;
in vec3 EyeDirectionCameraspace;
in vec3 LightDirectionCameraspace;

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2D DiffuseTexture;
uniform float LightPower;
uniform vec3 LightColor;
uniform vec3 LightPosition;

void main(){
	
	// Material properties
	vec4 MaterialDiffuseColor = texture( DiffuseTexture, UV ).rgba;
	vec4 MaterialAmbientColor = vec4(5.0, 5.0, 5.0, 5.0) * MaterialDiffuseColor;
	vec4 MaterialSpecularColor = vec4(0.3, 0.3, 0.3, 1.0);

	// Distance to the light
	float distance = length( LightPosition - PositionWorldspace );

	// Normal of the computed fragment, in camera space
	vec3 n = normalize( NormalCameraspace );
	// Direction of the light (from the fragment to the light)
	vec3 l = normalize( LightDirectionCameraspace );
	// Cosine of the angle between the normal and the light direction, 
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosTheta = clamp( dot( n,l ), 0,1 );
	
	// Eye vector (towards the camera)
	vec3 E = normalize(EyeDirectionCameraspace);
	// Direction in which the triangle reflects the light
	vec3 R = reflect(-l,n);
	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlpha = clamp( dot( E,R ), 0,1 );
	
	color = 
		// Ambient : simulates indirect lighting
		MaterialAmbientColor +
		//Diffuse : "color" of the object
		MaterialDiffuseColor * vec4(LightColor,1.0f) * LightPower * cosTheta / (distance*distance) +
		// Specular : reflective highlight, like a mirror
		MaterialSpecularColor * vec4(LightColor,1.0f) * LightPower * pow(cosAlpha,5) / (distance*distance);
}
#fragment-end

