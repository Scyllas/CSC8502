#version 330 core

uniform sampler2D diffuseTex;

const int maxLight = 10;

uniform int lightNum;
uniform vec3 cameraPos;
uniform vec4 lightColour[maxLight];
uniform vec3 lightPos[maxLight];
uniform float lightRadius[maxLight];

in Vertex {
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 worldPos;
} IN ;

out vec4 gl_FragColor;

void main (void) {
	
	vec4 diffuse = texture ( diffuseTex , IN.texCoord);
	
	vec3 viewDir = normalize (cameraPos - IN.worldPos );
	
	gl_FragColor = vec4(0.0,0.0,0.0,1);
	
	for(int i = 0; i < lightNum; i++){
		
		vec3 temp = lightPos[i] - IN.worldPos;
		vec3 incident = normalize ( temp );
		float dist = length (temp);
		float lambert = max (0.0 , dot ( incident, abs(IN.normal)));
		float atten = 1.0 - clamp (dist / lightRadius[i], 0.0 , 1.0);
		vec3 halfDir = normalize (incident + viewDir );
		
		float rFactor = max (0.0 , dot(halfDir , IN.normal));
		float sFactor = pow ( rFactor , 50.0);
		
		vec3 colour = ( diffuse.rgb * lightColour[i].rgb);
		colour += ( lightColour[i].rgb * sFactor) * 0.33;
		gl_FragColor += vec4 ( colour * atten * lambert, diffuse.a);
		gl_FragColor.rgb += (diffuse.rgb * lightColour[i].rgb) * 0.1;
	//	gl_FragColor.rgb += vec3(IN.normal);
	}
	gl_FragColor.a = 1;
}










