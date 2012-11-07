#version 110
uniform vec2 resolution;

void main() {
	vec2 p = vec2( gl_FragCoord.xy  / resolution.xy) - 0.5 ;
    p.x *= resolution.x/resolution.y;
    float r = length(p);
    float d = 0.15;
	float c = ( r < d ) ? 1.0 : 0.0; 
	
    
	gl_FragColor = vec4(vec3(1.0 * c),1.0);
}