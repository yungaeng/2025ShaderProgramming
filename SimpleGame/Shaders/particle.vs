#version 330

in vec3 a_Position;
in float a_Value;
in vec4 a_Color;
in float a_sTime;
in vec3 a_Vel;
in float a_lifeTime;
in float a_Mass;

out vec4 v_Color;
out vec3 v_Force;

uniform float u_Time;
uniform vec3 u_Force;

const float c_PI = 3.141592;
const vec2 c_G = vec2(0, -9.8);

void main()
{
	// float radius = a_Value;
	// float value = 2 * fract(u_Time) - 1;	// -1 ~ 1
	// float rad = (value + 1) * c_PI;			// 0 ~ 2PI
	// float x = cos(rad);
	// float y = sin(rad);
	
	float newAlpha = 1;
	float newTime = u_Time - a_sTime;
	vec4 newPosition = vec4(a_Position, 1);
	if(newTime > 0)
	{
		float t = fract(newTime / a_lifeTime) * a_lifeTime;
		float tt = t*t;

		float x = a_Vel.x * t + 0.5 * c_G.x * tt;
		float y = 0.5 + a_Vel.y * t + 0.5 * c_G.y * tt;

		float fx = c_G.x * a_Mass + u_Force.x;
		float fy = c_G.y * a_Mass + u_Force.y;

		newPosition.xy += vec2(x, y);
		newAlpha = 1 - t / a_lifeTime;
	}
	else
	{
		newPosition.xy += vec2(-100000, 0);
	}

	

	gl_Position = newPosition;
	v_Color = vec4(a_Color.rgb, newAlpha);
	v_Force =  vec3(fx, fy, 0);
}
