#version 150

uniform sampler2D spectrumTex;
uniform sampler2D venusTex;
uniform sampler2D gridTex;

uniform float radius;
uniform float starRadius;
uniform float power;
uniform float time;
uniform vec2 randSeed;
uniform int	randIterations;
uniform int unit;
uniform float color;
uniform float brightness;

in vec2 vTexCoord0;
in vec4 vColor;
in vec4 vVertex;

out vec4 oColor;

const float PI = 3.14159265;

float rand( vec2 co ){
    return fract( sin( dot( co.xy ,vec2( 12.9898,78.233 ) ) ) * 43758.5453 );
}

float drawCircle( float d, float r, float per )
{
	float rSub		= r * per;
	float invPer	= 1.0 - per;

	if( d < rSub )		return 1.0;
	else if( d < r )	return 1.0 - pow( ( d - rSub )/( r * invPer ),0.6 );
	else				return 0.0;
}

void main()
{
	vec3 spectrumCol	= texture( spectrumTex, vec2( color, 0.25 ) ).rgb;

  vec4 texColor     = texture(venusTex, vec2(vTexCoord0.x + time * 0.005, vTexCoord0.y - time * 0.001 ) );

	float dist			= length( vVertex.xyz );
	float distPer		= dist/radius;
	float alpha			= 0.0;

	vec3 offColor	= vec3( 0.0 );
	vec3 onColor	= vec3( 0.0 );
	float offAlpha	= 0.0;
	float onAlpha	= 0.0;

  float radiusOffset	= radius;

	vec4 gridCol		= texture( gridTex, vTexCoord0 );
  float newStarRadius = starRadius + 2.0;

  if( unit == 2 ){
    if( dist < newStarRadius ){
      alpha		= drawCircle( dist, newStarRadius * 2.0, 0.98 );
    }
  } else {

    if( unit == 1 ){ // MIDDLE PANEL
      newStarRadius	*= 0.8;
      float newRadius	= radius - newStarRadius;
      float newDist	= max( dist - newStarRadius, 0.0 );
      float distPer	= clamp( newDist/newRadius, 0.0, 1.0 );
      alpha			= pow( 1.0 - distPer, 1.0 + rand( randSeed ) * 3.0 ) * 0.5;
    } else { // CLOSEST PANEL
      newStarRadius	*= 0.5;
      float newRadius	= radius - newStarRadius;
      float newDist	= max( dist - newStarRadius, 0.0 );
//				float distPer	= clamp( newDist/newRadius, 0.0, 1.0 );
      alpha			= pow( max( 1.0 - distPer, 0.0 ), 1.0 + rand( randSeed ) * 5.0 );
    }
  }

  offColor	= vec3( gridCol );

  if(unit == 2){
    onColor		= vec3( brightness * spectrumCol) +  vec3(texColor * 0.3);
  }else{
    onColor		= vec3( brightness * spectrumCol);
  }
//		if( alpha > 0.925 ) onColor *= 1.5;

  offAlpha	= clamp( gridCol.a + alpha, 0.0, 1.0 );
  onAlpha		= alpha * 0.75;

	oColor.rgb	= mix( offColor, onColor, power );
	oColor.a		= mix( offAlpha, onAlpha, power );
}
