###############################################################
##	1. Vertex Part
###############################################################

#vertex-start
#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec2 VertexUV;
layout(location = 2) in vec3 VertexNormal;
layout(location = 3) in vec3 VertexTangent;
layout(location = 4) in vec3 VertexBitangent;

// Output data ; will be interpolated for each fragment.
out vec2 UV;
out vec3 Position;
out vec3 EyeDirectionCameraspace;
out vec3 LightDirectionCameraspace;
out vec3 LightDirectionTangentspace;
out vec3 EyeDirectionTangentspace;
out vec3 NormalCameraspace; 

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform mat3 MV3x3;
uniform vec3 LightPosition;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;

void main(){

	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  MVP * vec4(VertexPosition,1);

	// Position of the vertex, in worldspace : M * position
	Position = (M * vec4(VertexPosition,1)).xyz;

	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	vec3 vertexPosition_cameraspace = ( V * M * vec4(VertexPosition,1)).xyz;
	EyeDirectionCameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	vec3 LightPosition_cameraspace = ( V * vec4(LightPosition,1)).xyz;
	LightDirectionCameraspace = LightPosition_cameraspace + EyeDirectionCameraspace;

	// UV of the vertex. No special space for this one.
	UV = VertexUV;

	// model to camera = ModelView
	vec3 vertexTangent_cameraspace = MV3x3 * VertexTangent;
	vec3 vertexBitangent_cameraspace = MV3x3 * VertexBitangent;
	vec3 vertexNormal_cameraspace = MV3x3 * VertexNormal;

	mat3 TBN = transpose(mat3(
		vertexTangent_cameraspace,
		vertexBitangent_cameraspace,
		vertexNormal_cameraspace
	)); // You can use dot products instead of building this matrix and transposing it. See References for details.

	LightDirectionTangentspace = TBN * LightDirectionCameraspace;
	EyeDirectionTangentspace =  TBN * EyeDirectionCameraspace;
	NormalCameraspace = ( V * M * vec4(VertexNormal,0)).xyz;
}

#vertex-end


###############################################################
##	2. Fragment Part
###############################################################

#fragment-start
#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 Position;
in vec3 EyeDirectionCameraspace;
in vec3 LightDirectionCameraspace;

in vec3 LightDirectionTangentspace;
in vec3 EyeDirectionTangentspace;
in vec3 NormalCameraspace;

// Ouput data
layout(location = 0) out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2D DiffuseTexture;
uniform sampler2D NormalTexture;
uniform sampler2D SpecularTexture;
uniform mat4 V;
uniform mat4 M;
uniform mat3 MV3x3;
uniform float LightPower;
uniform vec3 LightColor;
uniform vec3 LightPosition;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;


void main(){

	// Material properties
	vec4 MaterialDiffuseColor = vec4(Kd, 1.0) * texture( DiffuseTexture, UV ).rgba;
	vec4 MaterialAmbientColor = vec4(Ka, 1.0) * MaterialDiffuseColor;
	vec4 MaterialSpecularColor = vec4(Ks, 1.0) * texture( SpecularTexture, UV ).rgba;


	// Local normal, in tangent space. V tex coordinate is inverted because normal map is in TGA (not in DDS) for better quality
	vec3 TextureNormal_tangentspace = normalize(texture( NormalTexture, vec2(UV.x, UV.y) ).rgb*2.0 - 1.0);

	// Distance to the light
	float distance = length( LightPosition - Position );

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
		// Ambient : simulates indirect lighting
		MaterialAmbientColor +
		// Diffuse : "color" of the object
		MaterialDiffuseColor * vec4(LightColor, 1.0f) * LightPower * cosTheta / (distance) +
			// Specular : reflective highlight, like a mirror
		MaterialSpecularColor * vec4(LightColor, 1.0f) * LightPower * pow(cosAlpha,5) / (distance*distance);

}

#fragment-end
