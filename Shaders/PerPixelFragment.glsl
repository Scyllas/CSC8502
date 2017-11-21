#version 150 core

uniform sampler2D diffuseTex;

uniform vec3 cameraPos;
uniform vec4 lightColour[];
uniform vec3 lightPos[];
uniform float lightRadius[];
uniform int LightNum;

in Vertex {
	vec3 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 worldPos;
} IN ;

out vec4 gl_FragColor;

void main (void) {

	vec4 diffuse = texture ( diffuseTex , IN . texCoord);
	
	vec3 incident[LightNum];
	float dist[LightNum];
	float max = 0;
	vec3 maxInc = (0,0,0);
	
	for(int i = 0; i < LightNum; i++){
	vec3 temp = lightPos[i] - IN.worldPos;
		incident[i] = normalize ( temp );
		dist[i] = length (temp);
		if(dist[i]> max){
		max = dist[i];
		maxInc = incident[i];
		}
	}
	
	

	
	float lambert = max (0.0 , dot ( maxInc , IN . normal));
	
	float atten = 1.0 - clamp (max / lightRadius , 0.0 , 1.0);
	
	vec3 viewDir = normalize (cameraPos - IN . worldPos );
	vec3 halfDir = normalize (maxInc + viewDir );
	
	float rFactor = max (0.0 , dot(halfDir , IN.normal));
	float sFactor = pow ( rFactor , 50.0);
	
	vec3 colour = ( diffuse.rgb * lightColour.rgb);
	colour += ( lightColour.rgb * sFactor) * 0.33;
	gl_FragColor = vec4 ( colour * atten * lambert, diffuse.a);
	gl_FragColor.rgb += (diffuse.rgb * lightColour.rgb) * 0.1;
}
