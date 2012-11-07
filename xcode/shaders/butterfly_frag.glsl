#version 110
uniform vec2 resolution;


#define PI 3.1415926
float lerp(float a, float b, float t);
vec3 lerp(vec3 a, vec3 b, float t);


void main()
{
    vec2 p = gl_FragCoord.xy / resolution.xy;
    float ar = resolution.y/resolution.x;

    p = p*2.0 - 1.0;;

    vec4 c = vec4(vec3(1.0),0.0);
    float r = length(p);
    float a = atan(p.x, p.y);

    //bottom
    float t1 = ( 3.0 * abs(a) - PI) / 2.0 ;
    float beta = max(t1, 0.0);	 
    float th = sin(beta)*sin(beta)* 0.5;

    //bottom - edges
    float adj = 1. + .02 * sin(43.0 * a);
    th *= adj;

    float e = -30. * abs( abs(a) - ( (4.0 * PI) + 5.0 * r) / 7.0 );
    float tailhook = .5 * exp(e);
    th += tailhook;


//    float red = step(th,r);//(beta < PI) ? 1.0 : 0.0;
//    c.x = 1.0 - step(th,r);
    c = step(th,r) == 0.0 ? vec4(1.0,0.0,0.0,1.0) : c;

    float t2 = (4.0 * abs(a) / 3.0  );
    float gamma = min(t2, PI);
    float th2 = sin(gamma)*sin(gamma)*0.5;

    float adj2 = 1. + .03 * sin(24.0 * a);
    th2 *= adj2;

//    float green = step(th2,r); //(gamma < PI) ? 1.0 : 0.0;
//        c.y = 1.0 - step(th2,r);
    if( c.x == 1.0 ) {
        c = step(th2,r) == 0.0 ? vec4( 0.0,0.0,1.0,1.0 ) : c;
    }
//    c.x = c.y == 1.0 ? 0.0 : c.x;


    //h = step(th2,red);
    //float  = step(th,r);	

    gl_FragColor = vec4( c);
}
