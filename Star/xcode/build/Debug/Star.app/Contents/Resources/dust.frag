#version 150

uniform sampler2D spectrumTex;
uniform float color;
uniform float power;

in vec2 vTexCoord0;
in vec4 vColor;

out vec4 oColor;

void main()
{

	vec3 col		= texture( spectrumTex, vec2( color, 0.25 ) ).rgb;

	oColor.rgb	= mix( vec3( 0.0 ), vec3( col ), power );
	oColor.a		= vColor.a;
}
