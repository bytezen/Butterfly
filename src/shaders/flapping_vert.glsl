#version 120

uniform float t;

float noise(float p);

float noise(float t) {
    float f = 0.0;
    f += 0.500 * noise1(2.0*t + 0.03 );
    f += 0.250 * noise1(4.0*t + 0.03 );
    f += 0.125 * noise1(8.0*t + 0.03);

    f /= 0.975;

    return f;
}


void main()
{
    gl_FrontColor = gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0;

	float alpha = 3.1459 * 0.25 * sin( 24.0 * 3.1459 * t );	
	vec4 p = ftransform(); 

    float n = noise(t);
	p.x *= cos(alpha) * 0.5 + n + 0.5;
	p.y *= sin(alpha) * 0.5 + n + 0.5;
    p.z += n;

	gl_Position = p;

}

