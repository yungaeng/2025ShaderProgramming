#version 330

in vec3 a_Position;
in float a_Value;
in vec4 a_Color;
in float a_sTime;
in vec3 a_Vel;

out vec4 v_Color;

uniform float u_Time;

const float c_PI = 3.141592;
const vec2 c_G = vec2(0, -9.8);

void main()
{
	// float radius = a_Value;
	// float value = 2 * fract(u_Time) - 1;	// -1 ~ 1
	// float rad = (value + 1) * c_PI;			// 0 ~ 2PI
	// float x = cos(rad);
	// float y = sin(rad);
	
	float newTime = u_Time - a_sTime;
	vec4 newPosition = vec4(a_Position, 1);
	if(newTime > 0)
	{
		float t = fract(newTime);
		float tt = t*t;
		float x = 0;
		float y = 0.5 * c_G.y * tt;
		newPosition.xy += vec2(x, y);
	}
	else
	{
		newPosition.xy += vec2(-100000, 0);
	}
	gl_Position = newPosition;
	v_Color = a_Color;
}
