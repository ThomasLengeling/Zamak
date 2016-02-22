#version 150

uniform sampler2D nebulaTex;
uniform sampler2D gridTex;
uniform sampler2D spectrumTex;

uniform float starRadius;
uniform float color;
uniform float alpha;
uniform float power;

in vec2 vTexCoord0;
in vec4 vColor;
in vec4 vVertex;

out vec4 oColor;

void main()
{
	// FADE NEBULA CLOSE TO STAR SURFACE TO PREVENT VISIBLE CLIPPING
	float dist = length( vVertex.xyz );
	if( dist < starRadius ) discard;

	float maxAlpha = 1.0;
	float distThresh = starRadius * 1.1;
	float distDelta  = distThresh - starRadius;
	if( dist < distThresh ){
		maxAlpha = ( dist - starRadius )/distDelta;
	}
	// END FADE

	vec3 spectrumCol	= texture( spectrumTex, vec2( color, 0.25 ) ).rgb;
	vec4 nebulaCol		= texture( nebulaTex, vTexCoord0 );
	vec4 gridCol		= texture( gridTex, vTexCoord0 );

	float nebulaAlpha	= nebulaCol.a * alpha;

	vec3 offColor		= vec3( mix( gridCol.r, power, nebulaAlpha ) );
	vec3 onColor		= vec3( spectrumCol );

	float offAlpha		= clamp( gridCol.a, 0.0, 1.0 );
	float onAlpha		= nebulaAlpha * maxAlpha;

	oColor.rgb	= mix( offColor, onColor, power );
	oColor.a		= mix( offAlpha, onAlpha, power );
}
