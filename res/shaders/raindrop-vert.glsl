#version 430

layout(location = 0) in vec4 pos;
uniform vec2 uScreenDimensions = vec2(960.0, 540.0);
uniform vec2 uOffset = vec2(2050.0, 1000.0);
uniform float uTime = 0.0;
out vec2 varyingVert;

void main()
{
	vec2 pos2d = pos.xy;	

	const float angle = 3.14 / 6.0;
	mat2 rotationMat = mat2(
		cos(angle), -sin(angle),
		sin(angle), cos(angle)	
	);
	
	pos2d *= rotationMat;
	const float scale = 24.0f;	
	pos2d.x *= scale / uScreenDimensions.x;
	pos2d.y *= scale / uScreenDimensions.y;	

	int id = gl_InstanceID;
	float xOffset = float(id % 100) * 96.0 / uScreenDimensions.x - uOffset.x / uScreenDimensions.x,
		  yOffset = float(id / 100) * 192.0 / uScreenDimensions.y - uOffset.y / uScreenDimensions.y;

	float moveX = 192.0 / uScreenDimensions.x * uTime, 
		  moveY = 384.0 / uScreenDimensions.y * uTime;

	vec2 screenPos = vec2(pos2d.x + xOffset + moveX, pos2d.y + yOffset - moveY); 

	gl_Position = vec4(screenPos.xy, 0.0, 1.0);
	varyingVert = pos.xy;
}
