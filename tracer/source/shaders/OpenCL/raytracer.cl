#define BACKGROUND_COLOUR (float4)(0,0,0,1)
#define PI 3.14159265358979
#define SPHERE_DATA_SIZE 20

bool _combFloat4(float4 f4a,float4 f4b)
{
	return (f4a.w == f4b.w && f4a.x == f4b.x && f4a.y == f4b.y && f4a.z == f4b.z);
}

//useful vector functions
float magnitude(float3 vec)
{
	return sqrt((vec.x*vec.x)+(vec.y*vec.y)+(vec.z*vec.z));
}

float3 reflect(float3 I, float3 N)
{
	return (I - 2.0f * dot(N,I) * N);
}

//IOR : indice of refraction
float3 refract(float3 I, float3 N, float ior1, float ior2)
{
	//printf("N = %f,%f,%f\n",N.x,N.y,N.z);
	//printf("I = %f,%f,%f\n",I.x,I.y,I.z);
	float part0 = ior1/ior2;
	float3 part1 = cross(N,cross(-N,I));
	float part2 = sqrt(1-((ior1/ior2)*(ior1/ior2))*(dot(cross(N,I),cross(N,I))));
	//printf("%f\n",(dot(cross(N,I),cross(N,I))));
	//printf("part2 = %f\n",part2);
	
	float3 temp = part0 * part1 - N;// * part2;
	//printf("temp = %f,%f,%f\n",temp.x,temp.y,temp.z);
	
	float3 returnVal = (ior1/ior2)*cross(N,cross(-N,I))-N*sqrt(1-((ior1/ior2)*(ior1/ior2))*dot(cross(N,I),cross(N,I)));
	//printf("return vec %f,%f,%f\n",returnVal.x,returnVal.y,returnVal.z);
	return returnVal;

	//return (float3)(0,0,0);
}

#define normalise(x) ({x/magnitude(x);})

//https://gamedev.stackexchange.com/questions/96459/fast-ray-sphere-collision-code
int raySphereIntersect(float3 point, float3 direction,float* t, float3* q, float3 spherePos, float sphereRadius)
{

	float3 m = point - spherePos;
	float b = dot(m,direction);
	float c = dot(m,m) - sphereRadius*sphereRadius;

	if(c > 0.0f && b > 0.0f){
		return 0;
		}
	float discr = b*b - c;

	if(discr < 0.0f)
		return 0;

	*t = -b - sqrt(discr);
	float tmax = -b + sqrt(discr);

	if (tmax < *t && tmax > 0.0)
		*t = tmax;

	//if(*t < 0.0f) *t = 0.0f;
	if (*t < 0.0f)
		return 0;

	*q = point + *t * direction;

	return 1;
}

//!useful vector functions

float4 calculateLighting(
	float4 inColour, float4 inColourReflected,float4 inColourRefracted, float3 inNormal,
	float3 eyePos, float3 lightDirection, float4 lightAmbient, float4 lightSpecular,
	float4 materialAmbient, float4 materialDiffuse, float4 materialSpecular , float materialShininess,
	bool inShadow, float opacity)
{
	float3 n = normalise(inNormal);
	float3 L = normalise(lightDirection);

	float3 v = normalise(-eyePos);
	float3 r = normalise(-reflect(L,n));

	float RdotV = max(0.0f,(float)dot(r,v));
	float NdotL = max((float)dot(n,L),0.0f);

	float4  colour = lightAmbient * materialAmbient;

	//printf("calculated colour %1.2v3hlf\n", colour);
	//printf("NdotL %1.2v8hlf\n", (float8)(n,0.69,0.42,L));
	//printf("NdotL %d\n", NdotL);

	float4 newColour = colour * inColour;

	if(NdotL > 0.0 && !inShadow)
	{
		colour += (lightAmbient * materialDiffuse * NdotL);
		colour += materialSpecular * lightSpecular * pow(RdotV, materialShininess);
		//only apply the reflection if the sphere is lit
		newColour = colour * ((inColourReflected * (NdotL)) + (inColour * (1-NdotL)));
	}


	if(opacity == 1){
		return newColour;
	} else {
		//return inColourRefracted
		return ((1-opacity)*inColourRefracted) + (opacity * newColour);
	}
}

void dataStreamToFloats(__global float* sphereData, int i, float4* sphereColour, float4* lightAmbient, float4* lightSpecular, float4* materialAmbient, float4* materialDiffuse, float4* materialSpecular)
{
	*sphereColour = (float4)(
		sphereData[(i*SPHERE_DATA_SIZE)+4],
		sphereData[(i*SPHERE_DATA_SIZE)+5],
		sphereData[(i*SPHERE_DATA_SIZE)+6],
		1
	);
	*materialAmbient = (float4)(
		sphereData[(i*SPHERE_DATA_SIZE)+7],
		sphereData[(i*SPHERE_DATA_SIZE)+8],
		sphereData[(i*SPHERE_DATA_SIZE)+9],
		1
	);
	*materialDiffuse = (float4)(
		sphereData[(i*SPHERE_DATA_SIZE)+10],
		sphereData[(i*SPHERE_DATA_SIZE)+11],
		sphereData[(i*SPHERE_DATA_SIZE)+12],
		1
	);
	*materialSpecular = (float4)(
		sphereData[(i*SPHERE_DATA_SIZE)+13],
		sphereData[(i*SPHERE_DATA_SIZE)+14],
		sphereData[(i*SPHERE_DATA_SIZE)+15],
		1
	);
}

void findClosestSphere(__global float* sphereData, int numSpheres, float3 startPos, float3 direction, int* closestSphere,float3* closestIntesect, float* closestT, int skipID)
{
	*closestSphere = -1;
	
	for(int i = 0; i < numSpheres; i++){
		float t;
		float3 q;

		float3 spherePos = (float3)(
			sphereData[(i*SPHERE_DATA_SIZE)+0],
			sphereData[(i*SPHERE_DATA_SIZE)+1],
			sphereData[(i*SPHERE_DATA_SIZE)+2]
		);
		//printf("sphere pos %4.0v3hlf\n", spherePos);
		if(raySphereIntersect(startPos, direction, &t, &q, spherePos, sphereData[(i*SPHERE_DATA_SIZE)+3])){
			if(i == skipID) continue;
			if(*closestSphere != -1){
				//need to switch to t value
				//if(q.z < closestIntesect->z){
				if(t < *closestT){
					*closestSphere = i;
					*closestIntesect = q;
					*closestT = t;
				}
			}else{
				*closestSphere = i;
				*closestIntesect = q;
				*closestT = t;
			}
		}
	}
}

bool calculateShadow(float3 intersectPoint, float3 lightPos, float3 normalVec, float3 spherePos, int sphereId,
	__global float* sphereData, int numSpheres)
{
	int closestSphere = -1;
	float3 closestIntersect;
	float closestT = 999999;

	float3 shadowVec = lightPos - intersectPoint;
	shadowVec = normalise(shadowVec);

	for(int i = 0; i < numSpheres; i++){
		if(i == sphereId) continue; //a sphere cant cast a shadow on its self
		float3 rSpherePos= (float3)(
			sphereData[(i*SPHERE_DATA_SIZE)+0],
			sphereData[(i*SPHERE_DATA_SIZE)+1],
			sphereData[(i*SPHERE_DATA_SIZE)+2]
		);
		float4 rSphereColour,rLightAmbient,rLightSpecular,rMaterialAmbient,rMaterialDiffuse,rMaterialSpecular;

		float rt;
		float3 rq;

		dataStreamToFloats(sphereData, i,&rSphereColour,&rLightAmbient,&rLightSpecular,&rMaterialAmbient,&rMaterialDiffuse,&rMaterialSpecular);
		
		if(raySphereIntersect(intersectPoint, shadowVec, &rt, &rq, rSpherePos, sphereData[(i*SPHERE_DATA_SIZE)+3])){
			closestSphere = i;
		}
	}

	return closestSphere > -1;
}

float4 reflectColour(float3 intersectPoint, float3 cameraPos, float3 normalVec, float3 spherePos, int sphereId,
	__global float* sphereData, int numSpheres)
{
	float3 reflectVec = reflect((intersectPoint-cameraPos), normalVec);
	reflectVec = normalise(reflectVec);
	int closestSphere = -1;
    float3 closestIntesect;
	float closestT = 9999999;

	//printf("numspheres %d\n",numSpheres);
	for(int j = 0; j < numSpheres; j++){
		//printf("%d: j = %d\n",sphereId,j);
		if(j == sphereId) continue; // no point reflecting the sphere with its self
		float3 rSpherePos= (float3)(
			sphereData[(j*SPHERE_DATA_SIZE)+0],
			sphereData[(j*SPHERE_DATA_SIZE)+1],
			sphereData[(j*SPHERE_DATA_SIZE)+2]
		);
		float4 rSphereColour,rLightAmbient,rLightSpecular,rMaterialAmbient,rMaterialDiffuse,rMaterialSpecular;

		float rt;
		float3 rq;

		dataStreamToFloats(sphereData, j,&rSphereColour,&rLightAmbient,&rLightSpecular,&rMaterialAmbient,&rMaterialDiffuse,&rMaterialSpecular);
		if(raySphereIntersect(intersectPoint, reflectVec, &rt, &rq, rSpherePos, sphereData[(j*SPHERE_DATA_SIZE)+3])){
				if(rt < closestT){
					closestSphere = j;
					closestIntesect = rq;
					closestT = rt;
				}
		}
	}

	float reflectVal = sphereData[(sphereId*SPHERE_DATA_SIZE)+17];
	
	//store the reflectiveness of the sphere 
	if(closestSphere != -1){
		/*
		float rvr = reflectVal * sphereData[(sphereId*SPHERE_DATA_SIZE)+4];
		float rvg = reflectVal * sphereData[(sphereId*SPHERE_DATA_SIZE)+5];
		float rvb = reflectVal * sphereData[(sphereId*SPHERE_DATA_SIZE)+6];
		
		return (float4)(
    		rvr * sphereData[(closestSphere*SPHERE_DATA_SIZE)+4] + (1-rvr) * sphereData[(sphereId*SPHERE_DATA_SIZE)+4],
    		rvg * sphereData[(closestSphere*SPHERE_DATA_SIZE)+5] + (1-rvg) * sphereData[(sphereId*SPHERE_DATA_SIZE)+5],
    		rvb * sphereData[(closestSphere*SPHERE_DATA_SIZE)+6] + (1-rvb) * sphereData[(sphereId*SPHERE_DATA_SIZE)+6],
    		1
    	);*/
		
		return (float4)(
    		reflectVal * clamp(sphereData[(closestSphere*SPHERE_DATA_SIZE)+4],0.0f,sphereData[(sphereId*SPHERE_DATA_SIZE)+4]) + (1-reflectVal) * sphereData[(sphereId*SPHERE_DATA_SIZE)+4],
    		reflectVal * clamp(sphereData[(closestSphere*SPHERE_DATA_SIZE)+5],0.0f,sphereData[(sphereId*SPHERE_DATA_SIZE)+5]) + (1-reflectVal) * sphereData[(sphereId*SPHERE_DATA_SIZE)+5],
    		reflectVal * clamp(sphereData[(closestSphere*SPHERE_DATA_SIZE)+6],0.0f,sphereData[(sphereId*SPHERE_DATA_SIZE)+6]) + (1-reflectVal) * sphereData[(sphereId*SPHERE_DATA_SIZE)+6],
    		1
    	);
		/*
		return (float4)(
    		reflectVal * sphereData[(closestSphere*SPHERE_DATA_SIZE)+4] * sphereData[(sphereId*SPHERE_DATA_SIZE)+4],
    		reflectVal * sphereData[(closestSphere*SPHERE_DATA_SIZE)+5] * sphereData[(sphereId*SPHERE_DATA_SIZE)+5],
    		reflectVal * sphereData[(closestSphere*SPHERE_DATA_SIZE)+6] * sphereData[(sphereId*SPHERE_DATA_SIZE)+6],
    		1
    	);
		*/
		/*
		return (float4)(
    		reflectVal * sphereData[(closestSphere*SPHERE_DATA_SIZE)+4] * sphereData[(sphereId*SPHERE_DATA_SIZE)+4],
    		reflectVal * sphereData[(closestSphere*SPHERE_DATA_SIZE)+5] * sphereData[(sphereId*SPHERE_DATA_SIZE)+5],
    		reflectVal * sphereData[(closestSphere*SPHERE_DATA_SIZE)+6] * sphereData[(sphereId*SPHERE_DATA_SIZE)+6],
    		1
    	);*/
	}
	return (float4)(
			sphereData[(sphereId*SPHERE_DATA_SIZE)+4],
			sphereData[(sphereId*SPHERE_DATA_SIZE)+5],
			sphereData[(sphereId*SPHERE_DATA_SIZE)+6],
			1
		);
}

float4 refractColour(float3 intersectPoint, float3 cameraPos, float3 normalVec, float ior,float3 spherePos, int sphereId,
	__global float* sphereData, int numSpheres)
{
	float3 exitPoint = intersectPoint;
	//printf("refracting\n");
	float rt;
	float3 rq;
	//step 1: get the intersect point and calculate the refeaction ray
	//float3 refractVec = refract((intersectPoint-cameraPos), normalVec,1,ior);
	float3 inVec = cameraPos-intersectPoint;
	inVec = -normalise(inVec);
	float3 refractVec = refract(inVec, normalVec,1,ior);
	//printf("in vec %f,%f,$f\nout vec %f,%f,%f\n",temp.x,temp.y,temp.z,refractVec.x,refractVec.y,refractVec.z);
	refractVec = normalise(refractVec);
	//step 2: calculate where the refection ray exits the sphere and calculate the refraction ray
	if((raySphereIntersect(intersectPoint, refractVec, &rt, &rq, spherePos, sphereData[(sphereId*SPHERE_DATA_SIZE)+3]))){
		/*inVec = intersectPoint-rq;
		inVec = normalise(inVec);
		float3 outNormal = spherePos - rq;
		outNormal = normalise(outNormal);
		refractVec = refract(inVec,outNormal,ior,1);
		refractVec = normalise(refractVec);
		exitPoint = rq;*/
		//return (float4)(1,0,0,1);
	}
	//step 3: see if it hits another sphere and calculate the colour




	
	
	int closestSphere = -1;
    float3 closestIntesect;
	float closestT = 9999999;

	//printf("numspheres %d\n",numSpheres);
	for(int j = 0; j < numSpheres; j++){
		//printf("%d: j = %d\n",sphereId,j);
		if(j == sphereId) continue; // no point refracting the sphere with its self
		float3 rSpherePos= (float3)(
			sphereData[(j*SPHERE_DATA_SIZE)+0],
			sphereData[(j*SPHERE_DATA_SIZE)+1],
			sphereData[(j*SPHERE_DATA_SIZE)+2]
		);
		float4 rSphereColour,rLightAmbient,rLightSpecular,rMaterialAmbient,rMaterialDiffuse,rMaterialSpecular;

		float rt;
		float3 rq;

		dataStreamToFloats(sphereData, j,&rSphereColour,&rLightAmbient,&rLightSpecular,&rMaterialAmbient,&rMaterialDiffuse,&rMaterialSpecular);
		if(raySphereIntersect(exitPoint, refractVec, &rt, &rq, rSpherePos, sphereData[(j*SPHERE_DATA_SIZE)+3])){
			if(rt < closestT){
				
				closestSphere = j;
				closestIntesect = rq;
				closestT = rt;
				
			}
		}
	}

	//float reflectVal = sphereData[(sphereId*SPHERE_DATA_SIZE)+17];

	//store the reflectiveness of the sphere 
	if(closestSphere != -1){
		return (float4)(
    		sphereData[(closestSphere*SPHERE_DATA_SIZE)+4],
    		sphereData[(closestSphere*SPHERE_DATA_SIZE)+5],
    		sphereData[(closestSphere*SPHERE_DATA_SIZE)+6],
    		1
    	);
		/*
		return (float4)(
    		reflectVal * sphereData[(closestSphere*SPHERE_DATA_SIZE)+4] + (1-reflectVal) * sphereData[(sphereId*SPHERE_DATA_SIZE)+4],
    		reflectVal * sphereData[(closestSphere*SPHERE_DATA_SIZE)+5] + (1-reflectVal) * sphereData[(sphereId*SPHERE_DATA_SIZE)+5],
    		reflectVal * sphereData[(closestSphere*SPHERE_DATA_SIZE)+6] + (1-reflectVal) * sphereData[(sphereId*SPHERE_DATA_SIZE)+6],
    		1
    	);*/
	}
	

	return BACKGROUND_COLOUR;
}

float4 calculatePixelColour(float3 cameraPos, float screenDist, __global float* sphereData, int numSpheres, __global float* lighting)
{
	float3 screenPos;
	//ray code goes here
	screenPos.x = (get_global_id(0));
	screenPos.y = (get_global_id(1));
	screenPos.z = screenDist;
	
	float3 direction = screenPos - cameraPos;
	direction = (direction)/magnitude(direction);


	int closestSphere = -1;
	float3 closestIntesect;

	for(int i = 0; i < numSpheres; i++){
		float t;
		float3 q;

		float3 spherePos = (float3)(
			sphereData[(i*SPHERE_DATA_SIZE)+0],
			sphereData[(i*SPHERE_DATA_SIZE)+1],
			sphereData[(i*SPHERE_DATA_SIZE)+2]
		);
		//printf("sphere pos %4.0v3hlf\n", spherePos);
		if(raySphereIntersect(cameraPos, direction, &t, &q, spherePos, sphereData[(i*SPHERE_DATA_SIZE)+3])){
			if(closestSphere != -1){
				if(q.z < closestIntesect.z){
					closestSphere = i;
					closestIntesect = q;
				}
			}else{
				closestSphere = i;
				closestIntesect = q;
			}
		}
	}


	if(closestSphere != -1){
		float3 spherePos = (float3)(
			sphereData[(closestSphere*SPHERE_DATA_SIZE)+0],
			sphereData[(closestSphere*SPHERE_DATA_SIZE)+1],
			sphereData[(closestSphere*SPHERE_DATA_SIZE)+2]
		);
		//printf("spherePos %4.9v3hlf\n", spherePos);
		if(spherePos.x == 650 && spherePos.y == 360 && spherePos.z == 30){
			//printf("the sphere is here\n");
		}
		//get all the sphere data
		float4 sphereColour,lightAmbient,lightSpecular,materialAmbient,materialDiffuse,materialSpecular;
			dataStreamToFloats(sphereData, closestSphere,&sphereColour,&lightAmbient,&lightSpecular,&materialAmbient,&materialDiffuse,&materialSpecular);

		lightAmbient = (float4)(
			lighting[0],lighting[1],lighting[2],1);
		lightSpecular = (float4)(
			lighting[3],lighting[4],lighting[5],1);

		//get the light direction vector
		float3 lightPos = (float3)(640,1000,60);
		float3 lightDirection = lightPos - closestIntesect;
		float3 normalVec = closestIntesect - spherePos;
		normalVec = normalise(normalVec);

		//printf("closestIntersect %4.0v3hlf\n", closestIntesect);
		//using the green sphere as a reflective sphere
		
		bool inShadow = calculateShadow(closestIntesect,lightPos,normalVec, spherePos, closestSphere, sphereData, numSpheres);
		//bool inShadow = false;
	
		float4 reflectedColour = sphereColour;
		float4 refractiveColour = sphereColour;

		float reflectVal = sphereData[(closestSphere*SPHERE_DATA_SIZE)+17];

		if(!inShadow && reflectVal > 0){
			reflectedColour = reflectColour(closestIntesect, cameraPos, normalVec, spherePos, closestSphere, sphereData, numSpheres);
		}

		//testing the refractive code
		float opacity = sphereData[(closestSphere*SPHERE_DATA_SIZE)+18];
		if(opacity<1){
			refractiveColour = refractColour(closestIntesect, cameraPos, normalVec, sphereData[(closestSphere*SPHERE_DATA_SIZE)+19],spherePos, closestSphere, sphereData, numSpheres);
			//temporaraly return reftacted colour mixed with sphere colour 
			/*
			return (float4)(
				(1-opacity) * refractiveColour.x + (opacity) * sphereColour.x,
				(1-opacity) * refractiveColour.y + (opacity) * sphereColour.y,
				(1-opacity) * refractiveColour.z + (opacity) * sphereColour.z,
				1
			);*/
		}

		//clculate lighting will probably need the opacity of the object
		return calculateLighting(sphereColour, reflectedColour,refractiveColour ,normalVec,closestIntesect - cameraPos,
			lightDirection,lightAmbient,lightSpecular,//light //direction ambiant specular
			materialAmbient,materialDiffuse,materialSpecular,sphereData[(closestSphere*SPHERE_DATA_SIZE)+16],inShadow,opacity);//material //ambient diffuse specular shininess
	}

	return BACKGROUND_COLOUR;
}

__kernel void raytracer(__write_only image2d_t image, float3 cameraPos, float screenDist, __global float* sphereData, int numSpheres, __global float* lighting)
{
	int2 pos = (int2)(get_global_id(0),get_global_id(1));
	//float4 colour = (float4)(inColour,1);
	//printf("numSpheres %d\n", numSpheres);
	//printf("sphere pos %d,%d,%d\n", sphereData[0],sphereData[1],sphereData[2]);
	float4 colour = calculatePixelColour(cameraPos, screenDist, sphereData, numSpheres, lighting);
	write_imagef(image,pos,colour);
}
