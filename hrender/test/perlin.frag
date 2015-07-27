#extension GL_LSGL_trace : enable

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D tex0;
uniform vec2 resolution;
varying float matID;
varying vec3 mnormal;
uniform vec4 color;

float hash( vec2 _v ){
  return fract( sin( dot( _v, vec2( 89.44, 19.36 ) ) ) * 22189.22 );
}

float iHash( vec2 _v, vec2 _r ){
  mat2 h = mat2(
    hash( vec2( floor( _v * _r + vec2( 0.0, 0.0 ) ) / _r ) ),
	  hash( vec2( floor( _v * _r + vec2( 1.0, 0.0 ) ) / _r ) ),
	  hash( vec2( floor( _v * _r + vec2( 0.0, 1.0 ) ) / _r ) ),
	  hash( vec2( floor( _v * _r + vec2( 1.0, 1.0 ) ) / _r ) )
  );
  vec2 ip = vec2( smoothstep( vec2( 0.0 ), vec2( 1.0 ), mod( _v * _r, 1.0 ) ) );
	return mix(
		mix( h[0][0], h[1][0], ip.y ),
		mix( h[0][1], h[1][1], ip.y ),
		ip.x
	);
}

float noise( vec2 _v ){
  float sum = 0.0;
  for( int i=1; i<7; i++ ){
    sum += iHash(
			_v + vec2( i ),
			vec2( 2.0 * pow( 2.0, float( i ) ) ) ) / pow( 2.0, float( i )
		);
  }
  return sum;
}

void main(){
	vec3 pos, nor, dir;
	isectinfo( pos, nor, dir );
	float rand = noise( pos.xy * 0.1 );
	gl_FragColor = vec4( vec3( rand ), 1.0 );
	return;
}
