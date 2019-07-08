###############################################################
##	1. Vertex Part				    	     
###############################################################

#vertex-start
#version 330

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 squareVertices;
layout(location = 1) in vec4 xyzs; // Position of the center of the particule and size of the square
layout(location = 2) in vec2 VertexUV; // Position of the center of the particule and size of the square
layout(location = 3) in vec3 VertexNormal;


// Output data ; will be interpolated for each fragment.
out vec2 UV;
out vec3 PositionWorldspace;
out vec3 NormalCameraspace; // to wyjebac
out vec3 EyeDirectionCameraspace;
out vec3 LightDirectionCameraspace;


// Values that stay constant for the whole mesh.
uniform mat4 V; // Model-View-Projection matrix, but without the Model (the position is in BillboardPos; the orientation depends on the camera)
uniform mat4 P;
uniform vec3 LightPosition;


vec4 quat_from_axis_angle(vec3 axis, float rad_angle)
{ 
  vec4 qr;
  float half_angle = 0.5*rad_angle;
  qr.x = axis.x * sin(half_angle);
  qr.y = axis.y * sin(half_angle);
  qr.z = axis.z * sin(half_angle);
  qr.w = cos(half_angle);
  return qr;
}

vec3 rotate_vertex_position(vec3 position, vec3 axis, float rad_angle)
{ 
  vec4 q = quat_from_axis_angle(axis, rad_angle);
  vec3 v = position.xyz;
  return v + 2.0 * cross(q.xyz, cross(q.xyz, v) + q.w * v);
}

void main()
{

	// Output position of the vertex, in clip space : MVP * position
	PositionWorldspace =  squareVertices*xyzs.w + xyzs.xyz; //tutaj pomnozyc o rotacje?
	gl_Position =  P * V * vec4(PositionWorldspace, 1.0f);
		
	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	vec3 VertexPositionCameraspace = ( V * vec4(PositionWorldspace,1)).xyz;
	EyeDirectionCameraspace = vec3(0,0,0) - VertexPositionCameraspace;

	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	vec3 LightPositionCameraspace = ( V * vec4(LightPosition,1)).xyz;
	LightDirectionCameraspace = LightPositionCameraspace + EyeDirectionCameraspace;
	
	// Normal of the the vertex, in camera space
	NormalCameraspace = ( V * vec4(VertexNormal,0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
	
	// UV of the vertex. No special space for this one.
	UV = VertexUV;

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
	vec4 MaterialDiffuseColor = vec4(1.0, 1.0,1.0, 1.0) * texture( DiffuseTexture, UV ).rgba;
	vec4 MaterialAmbientColor = vec4(0.9, 0.9, 0.9, 1.0) * MaterialDiffuseColor;
	vec4 MaterialSpecularColor = vec4(0.2, 0.2, 0.2, 1.0);

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
		// Diffuse : "color" of the object
		MaterialDiffuseColor * vec4(LightColor,1.0f) * LightPower * cosTheta / (distance*distance) +
		// Specular : reflective highlight, like a mirror
		MaterialSpecularColor * vec4(LightColor,1.0f) * LightPower * pow(cosAlpha,5) / (distance*distance);

}


#fragment-end
