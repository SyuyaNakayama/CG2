#include "Basic.hlsli"

VSOutPut main(float4 pos : POSITION,float2 uv : TEXCOORD )
{
	VSOutPut output;
	output.svpos = pos;
	output.uv = uv;
	return output;
}