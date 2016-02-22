#version 150

uniform sampler2D coronaTex;
uniform sampler2D spectrumTex;

uniform float starColor;
uniform float power;

in vec2 vTexCoord0;
in vec4 vColor;

out vec4 oColor;

void main()
{
	vec3 spectrumCol	= texture( spectrumTex, vec2( starColor, 0.25f ) ).rgb;
	vec4 coronaCol		= texture( coronaTex, vTexCoord0 );

	float coronaAlpha	= coronaCol.a;

	vec3 offColor		= vec3( 0.0 );
	vec3 onColor		= vec3( spectrumCol );

	oColor.rgb	= mix( offColor, onColor, power );
    oColor.a    = coronaAlpha;
}
