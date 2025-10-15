#version 330

in vec3 a_Position;
in float a_Value;
in vec4 a_Color;
in float a_sTime;
in vec3 a_Vel;
in float a_lifeTime;
in float a_Mass;
in float a_Period;

out vec4 v_Color;
out vec3 v_Force;

uniform float u_Time;
uniform vec3 u_Force;

const float c_PI = 3.141592;
const vec2 c_G = vec2(0, -9.8);

void fountain()
{
	float newAlpha = 1;
	float lifeTime = a_lifeTime;
	float newTime = u_Time - a_sTime;
	vec4 newPosition = vec4(a_Position, 1);

	if(newTime > 0)
	{
		float t = fract(newTime / lifeTime) * lifeTime;
		float tt = t*t;

		float x = a_Vel.x * t + 0.5 * c_G.x * tt;
		float y = 0.5 + a_Vel.y * t + 0.5 * c_G.y * tt;

		float fx = c_G.x * a_Mass + u_Force.x;
		float fy = c_G.y * a_Mass + u_Force.y;

		newPosition.xy += vec2(fx, fy);
		newAlpha = 1 - t / lifeTime;
	}
	else
	{
		newPosition.xy += vec2(-100000, 0);
	}

	
	gl_Position = newPosition;
	v_Color = vec4(a_Color.rgb, newAlpha);
}

void sinParticle()
{
	vec4 centerC = vec4(1,0,0,1);
	vec4 borderC = vec4(1,1,1,1);
	vec4 newColor = a_Color;

	vec4 newPosition = vec4(a_Position, 1);
	float newAlpha = 1;
	
	float newTime = u_Time - a_sTime;
	float lifeTime = a_lifeTime;
	float amp = 2 *a_Value - 1;
	float period = a_Period * 2;
	if(newTime > 0)
	{
		float t = fract(newTime / lifeTime) * lifeTime;
		float tt = t*t;
		float nTime = t/lifeTime;
		float x = nTime * 2 - 1;
		float y = nTime * sin(nTime*c_PI) * amp * sin(period*(2*c_PI*(t/lifeTime)));

		newPosition.xy += vec2(x, y);
		newAlpha = 1-t/lifeTime;

		float d = abs(y);
		newColor = mix(centerC , borderC , d*3);
	}
	else
	{
		newPosition.xy += vec2(-100000, 0);
	}

	gl_Position = newPosition;

	v_Color = vec4(newColor.rgb, newAlpha);
}

void circleParticle()
{
	vec4 newPosition = vec4(a_Position, 1);
	float lifeTime = a_lifeTime;
	float newAlpha = 1;

	float newTime = u_Time - a_sTime;

	if(newTime > 0)
	{
		float value = a_Value * c_PI * 2;
		float x = sin(value);
		float y = cos(value);

		float t = fract(newTime / lifeTime) * lifeTime;
		float tt = t*t;
		float nTime = t/lifeTime;

		x += t * c_G.x * tt;
		y += t * c_G.y * tt;

		newPosition.xy += vec2(x, y);
		newAlpha = 1-t/lifeTime;
	}
	else
	{
		newPosition.xy += vec2(-100000, 0);
	}

	gl_Position = newPosition;
	v_Color = vec4(a_Color.rgb, newAlpha);
}

void main()
{
	circleParticle();
}
