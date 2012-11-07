uniform vec2 resolution;
uniform vec3 topWingC;
uniform vec3 stripeC1;
uniform vec3 stripeC2;
uniform float stripeT;
uniform int stripeFn;
uniform float tailShape;
uniform float topFringeFactor;
uniform float bottomFringeFactor;
uniform float topFringeEffect;
uniform float bottomFringeEffect;

#define PI 3.1415926
#define YELLOW vec3(0.8,0.8,0.0)
#define RED vec3(.8,0.0,0.0)
#define WHITE vec3(1.0)
#define BLACK vec3(0.0)

void initWingColorParams();
float edgePattern( float rn );
float topDetail(float a, float size, float fringeEffect, float factor );
float striping( vec2 p, float t, int mode );
float bottom( vec2 p, float r, float a );
float bottomDetail( float a, float size, float fringeEffect, float factor );
float top( vec2 p, float r, float a);
float topDetail(float a, float size, float fringeEffect );
float bottomTail( float angle, float r, float tailLength, float location, float shape);
float smooth( float c, float w, float t );

int MODE0 = 0;
int MODE1 = 1;
int MODE2 = 2;

float a[10];
float k[10];

void main()
{
	vec2 p = (gl_FragCoord.xy) / resolution.xy;
	//clamp
	p.x = p.x > 1.0 ? 1.0: p.x;
	p.y = p.y > 1.0 ? 1.0: p.y;
	p = p*2.0 - 1.0;
	
	float r = length( p );
	float angle = atan(p.x,p.y);
    
	initWingColorParams();
	
    
	//top
	float th2 = top(p,r, angle);
	th2 *= topDetail(angle, .6, topFringeEffect, topFringeFactor);
    float topRegion = 1.0 - ( step(th2,r) + smooth(th2,.03,r));
    
	//bottoms
	float th = bottom(p, r, angle);
	th *= bottomDetail(angle, .4, bottomFringeEffect, bottomFringeFactor);
	th += bottomTail(angle, r, .4, 4.0, tailShape);
    float bottomRegion = 1.0 - ( step(th,r) + smooth(th,.03,r) );
    
    //The first color controls the background; 
    //The second color side effect controls the fringe of top wing
	vec3 c = mix( WHITE * (1.0 - bottomRegion), topWingC, topRegion );
    
	float wingParam = striping( vec2(p.x,p.y + .25), stripeT, stripeFn );
	vec3 wingColor = mix(stripeC1, stripeC2, wingParam);
	float rn = r / th2;
	float edge = edgePattern( rn );
	wingColor *= edge;
	wingColor *= 1.0 - step(th2,r);
	
	wingColor += c;
	gl_FragColor = vec4(wingColor,1.0);
    //	gl_FragColor = vec4(c,1.0);
}

float edgePattern( float rn ) {
	float param = max(0.0, 1.0 - 2.0 * pow(rn,64.0) );
	return param;
}

void initWingColorParams() {
	a[0] = .1;
	a[1] = .1;
	a[2] = .1;
	a[3] = .1;
	a[4] = .1;
	a[5] = 0.;
	a[6] = 0.;
	a[7] = 0.;
	a[8] = 0.;
	a[9] = 0.;
	
	k[0] = 100.0;
	k[1] = 40.0;
	k[2] = 6.0;
	k[3] = 8.0;
	k[4] = 100.0;
	k[5] = 1.0;
	k[6] = 4.0;
	k[7] = 60.0;
	k[8] = 80.0;
	k[9] = 100.0;
}


float striping( vec2 p, float t, int mode ) {
	vec3 c;
	float xy = 0.0;
    
	if( mode == MODE0 ) {
		xy =  t * (1.0 - p.y)  * p.x; //param to change patterning
	} 
	else if( mode == MODE1 ) {
		xy =  t *  p.y  * p.x; //param to change patterning
	}
	else if( mode == MODE2 ) {
		xy  = t * ( p.y)  / p.x; //param to change patterning
	}
    
	float param = 0.0;
	int i = 0;
	for(;i < 5; ++i) {
		param += a[i] * cos(k[i]*xy);	//need to understand weighting better
	}
    
	param = smoothstep(-0.5,0.5,param);
	return param;
}

float bottom( vec2 p, float r, float a ) {
	vec3 c = vec3(0.0);
	float t = ( 3.0 * abs(a) - PI) / 2.0 ;
	float beta = max(t, 0.0);	 
	float th = sin(beta)*sin(beta);
    
	return th;
}

/*
 size = [0.5,2.0] ; defailt 0.5
 fringeEffect = [.01,.5]; default .01
 */
float bottomDetail( float a, float size, float fringeEffect, float factor ) {
	float detail = size + fringeEffect * sin(factor*a);
	return detail;
}

/*
 tailLength = < 1.0 ; default .25
 location = 3.5 - 6.25; default 4.0
 */
float bottomTail( float angle, float r, float tailLength, float location, float shape) {
	float e = -1.0 * shape * abs( abs(angle) - ( (location * PI) + 5.0 * r) / 7.0 );
	return tailLength * exp(e);
}


float top( vec2 p, float r, float a) {
	vec3 c;
	float t = (4.0 * abs(a) / 3.0  );
	float gamma = min(t, PI);
	float th = 1.5 * sin(gamma)*sin(gamma);
	
	return th;
}

/*
 size = [0.5,2.0] ; defailt 0.5
 fringeEffect = [.01,.5]; default .01
 */
float topDetail(float a, float size, float fringeEffect, float factor ) {
	float detail = size + fringeEffect * sin(factor*a);
	return detail;
}


float smooth( float c, float w, float t ) {
	t = abs(t - c);
	if( t > w ) return 0.0;
	t /= w;
	return 1.0 - t * t * ( 3.0 - 2.0 * t); //Ha! This is just cubic interpolation! ;-)
}

