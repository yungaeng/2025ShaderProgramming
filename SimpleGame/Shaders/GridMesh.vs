#version 330

in vec3 a_Position;
out vec4 v_Color;

uniform float u_Time;

const float c_PI = 3.141592;

void Flag() 
{
	vec4 newPosition = vec4(a_Position, 1);

	float value = (a_Position.x + 0.5) * 2 * c_PI;
	float value1 = a_Position.x + 0.5;
	float dx = 0;
	float dy = value1 * 0.5 * sin(value + u_Time);

	newPosition.y *= (1-value1);
	newPosition.xy += vec2(dx,dy);

	float shading = (sin(value- u_Time)+1) / 2;

	gl_Position = newPosition;

	v_Color = vec4(shading);
}

void Wave()
{
	vec4 newPosition = vec4(a_Position, 1);
	gl_Position = newPosition;

	float d = distance(newPosition.xy, vec2(0,0));

	/*
	if(d<5)
	{
		v_Color = vec4(1);
	}
	else
	{
		v_Color = vec4(0);
	}
	*/

	/*
	float value = clamp(0.5 - d, 0, 1);
	value = ceil(value);
	*/

	float value = sin(d * 4 * c_PI * 10 - u_Time * 10);
	float p = 1 - clamp(d*2,0,1);

	v_Color = vec4(value*p);
}

void main()
{
	//Flag();
	Wave();
}
