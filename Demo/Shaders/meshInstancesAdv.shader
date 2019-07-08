###############################################################
##	1. Vertex Part				    	     
###############################################################

#vertex-start
#version 330

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertices; // vertices for mesh
layout(location = 1) in vec4 xyzs; // Position of the center of the particule and size of the square
layout(location = 2) in vec2 VertexUV; 
layout(location = 3) in vec3 VertexNormal;
layout(location = 4) in vec3 VertexTangent;
layout(location = 5) in vec3 VertexBitangent;

// Output data ; will be interpolated for each fragment.
out vec2 UV;
out vec3 PositionWorldspace;
out vec3 NormalCameraspace; 
out vec3 EyeDirectionCameraspace;
out vec3 LightDirectionCameraspace;
out vec3 LightDirectionTangentspace;
out vec3 EyeDirectionTangentspace;


// Values that stay constant for the whole mesh.
uniform mat4 V; 
uniform mat4 P;
uniform vec3 LightPosition;
uniform mat3 MV3x3;


 			   //xyzs.x,    xyzs.y,    xyzs.z,    1.0f);

void main()
{

	// Output position of the vertex, in clip space : MVP * position
	PositionWorldspace = vertices*xyzs.w + xyzs.xyz;
	vec3 rot = cross(PositionWorldspace, vec3(1,0,0));
	gl_Position =  P * V * vec4(PositionWorldspace,1) * vec4(rot,1.0f);


	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	vec3 vertexPosition_cameraspace = ( V * vec4(PositionWorldspace,1)).xyz;
	EyeDirectionCameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	vec3 LightPosition_cameraspace = ( V * vec4(LightPosition,1)).xyz;
	LightDirectionCameraspace = LightPosition_cameraspace + EyeDirectionCameraspace;

	// UV of the vertex. No special space for this one.
	UV = VertexUV;

	// model to camera = ModelView
	vec3 vertexTangent_cameraspace =  (V* (vec4(PositionWorldspace,1) * vec4(VertexTangent,1))).xyz;
	vec3 vertexBitangent_cameraspace =(V *(vec4(PositionWorldspace,1) * vec4(VertexBitangent,1))).xyz;
	vec3 vertexNormal_cameraspace = (V*(vec4(PositionWorldspace,1) * vec4(VertexNormal,1))).xyz;

	mat3 TBN = transpose(mat3(
		vertexTangent_cameraspace,
		vertexBitangent_cameraspace,
		vertexNormal_cameraspace
	)); // You can use dot products instead of building this matrix and transposing it. See References for details.

	LightDirectionTangentspace = TBN * LightDirectionCameraspace;
	EyeDirectionTangentspace =  TBN * EyeDirectionCameraspace;
	NormalCameraspace = ( V * vec4(VertexNormal*xyzs.w+xyzs.xyz,0)).xyz;
	
	//PositionWorldspace = (vec4(PositionWorldspace,1)).xyz;

}



#vertex-end

###############################################################
##	2. Fragment Part				     
###############################################################

#fragment-start
#version 330

// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 PositionWorldspace;
in vec3 EyeDirectionCameraspace;
in vec3 LightDirectionCameraspace;
in vec3 LightDirectionTangentspace;
in vec3 EyeDirectionTangentspace;
in vec3 NormalCameraspace;

// Ouput data
layout(location = 0) out vec4 color;
in vec4 xyzs; 

// Values that stay constant for the whole mesh.
uniform sampler2D DiffuseTexture;
uniform sampler2D NormalTexture;
uniform sampler2D SpecularTexture;
uniform float LightPower;
uniform vec3 LightColor;
uniform vec3 LightPosition;
uniform mat4 V; 


void main(){
	// Material properties
	vec4 MaterialDiffuseColor = vec4(1.0, 1.0,1.0, 1.0) * texture( DiffuseTexture, UV ).rgba;
	vec4 MaterialAmbientColor = vec4(0.5, 0.5, 0.5, 1.0) * MaterialDiffuseColor;
	vec4 MaterialSpecularColor = vec4(0.0, 0.0, 0.0, 0.0) * texture( SpecularTexture, UV ).rgba;

	// Local normal, in tangent space. V tex coordinate is inverted because normal map is in TGA (not in DDS) for better quality
	vec3 TextureNormal_tangentspace = normalize(texture( NormalTexture, vec2(UV.x, UV.y) ).rgb*2.0 - 1.0);

	// Distance to the light
	float distance = length( LightPosition - PositionWorldspace );

	// Normal of the computed fragment, in camera space
	vec3 n = TextureNormal_tangentspace;
	
	// Direction of the light (from the fragment to the light)
	vec3 l = normalize(LightDirectionTangentspace);
	
	// Cosine of the angle between the normal and the light direction,
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosTheta = clamp( dot( n,l ), 0,1 );

	// Eye vector (towards the camera)
	vec3 E = normalize(EyeDirectionTangentspace);
	
	// Direction in which the triangle reflects the light
	vec3 R = reflect(-l,n);
	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlpha = clamp( dot( E,R ), 0,1 );


	color =
		MaterialAmbientColor +
		MaterialDiffuseColor * vec4(LightColor, 1.0f) * LightPower * cosTheta / (distance) +
		MaterialSpecularColor * vec4(LightColor, 1.0f) * LightPower * pow(cosAlpha,5) / (distance*distance);
}


#fragment-end
