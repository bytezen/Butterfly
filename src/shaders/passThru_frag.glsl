#version 110
uniform sampler2D tex;

void main()
{	
    gl_FragColor = vec4(1.0) * texture2D( tex, gl_TexCoord[0].xy ); 
}
