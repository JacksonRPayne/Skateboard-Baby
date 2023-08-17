#version 330 core

layout (location=0) out vec4 color;

in vec4 v_color;
in vec2 v_texCoord;
in vec2 v_tileCount;
in vec4 v_texCoordBounds;
in vec2 v_stretchFactor;
flat in float v_texSlot;

uniform sampler2D[16] textures;

// TODO: Thought of this one in the shower:
// Have some uniforms (or maybe calculate) for size of the quad ur rendering,
// and also the max uv val. Then you can use modular arithmetic to tile the texture
// all while working within a sprite sheet. I think the formula would be:
// new_uv = (uv * size/(texsize/128)) % max_uv
//                ^ this term is a		^ this is the upper bound
//					texture to world	  of the subtexture, its the
//					space conversion	  value to wrap around
//					sorta or someth

vec4 fat_pixel_sample(sampler2D tex, vec2 uv){
	// https://jorenjoestar.github.io/post/pixel_art_filtering/


	// -- CURRENT USE -- y'know... maybe this is just fine/correct
	// Update: I think its my monitor causing the blurryness... lol
	//https://www.shadertoy.com/view/MlB3D3
	ivec2 texSize = textureSize(tex, 0);
	vec2 pixel = uv * texSize;
	pixel = floor(pixel) + min(fract(pixel) / fwidth(pixel), 1.0) - 0.5;
	//pixel = floor(pixel) + smoothstep(0.0, 1.0, fract(pixel) / fwidth(pixel)) - 0.5; // <-- described as "extra sharp"??
	return texture(tex, pixel / texSize);

	// -- ATTEMPT AT CASEY STYLE --
	// STEPS:
	// 1: get uv in texture space (multiply by size of texture)
	// 2: get span of uv with  u-0.5 dx & u+0.5 dx
	// --- ^ take these coords and multiply them out by texture size to get it in texture space
	// 3: "if" (for now use an actual if) the floor of the min and max vals are equal: return floor + 0.5 (middle)
	// -- If not -- need to linear sample -> take floor(max) and find distance to it from min and max. Those are 
	// -- the unnormalized sample ratios. 

	//// STEP 1
	//ivec2 texSize = textureSize(tex, 0);
	//vec2 pixel = uv * texSize;
	//// STEP 2
	////vec2 deriv = vec2(dFdx(pixel).x, dFdy(pixel).y);
	//vec2 pixMin = floor(pixel)+0.5 - fwidth(pixel);
	//vec2 pixMax = floor(pixel)+0.5 + fwidth(pixel);
	//// STEP 3 / 4??
	//if(floor(pixMin) == floor(pixMax)) pixel = floor(pixMin)+0.5; // If sampling within one texel
	//else pixel =  floor(pixMin) + 0.5 + max(((floor(pixMax) - pixMin) / (pixMax - pixMin)), 0.0); // this approach is interesting and produces interestring results
	////pixel =  floor(pixMin) + 0.5 + (floor(pixMax) - pixMin) / (pixMax - pixMin);
	//vec2 haha = pixel / texSize;
	//return texture(tex, haha);

}

void main(){
	//vec2 tiledUV = mod((v_texCoord - v_texCoordBounds.xy)* v_tileCount, (v_texCoordBounds.zw-v_texCoordBounds.xy)) + v_texCoordBounds.xy;
	// ^This slows things WAY down, not the right solution
	vec2 tiledUV = v_texCoord;
	switch(int(v_texSlot)){
		case -1: color = v_color; break;
		case 0: color = fat_pixel_sample(textures[0], tiledUV) * v_color; break;
		case 1: color = fat_pixel_sample(textures[1], tiledUV) * v_color; break;
		case 2: color = fat_pixel_sample(textures[2], tiledUV) * v_color; break;
		case 3: color = fat_pixel_sample(textures[3], tiledUV) * v_color; break;
		case 4: color = fat_pixel_sample(textures[4], tiledUV) * v_color; break;
		case 5: color = fat_pixel_sample(textures[5], tiledUV) * v_color; break;
		case 6: color = fat_pixel_sample(textures[6], tiledUV) * v_color; break;
		case 7: color = fat_pixel_sample(textures[7], tiledUV) * v_color; break;
		case 8: color = fat_pixel_sample(textures[8], tiledUV) * v_color; break;
		case 9: color = fat_pixel_sample(textures[9], tiledUV) * v_color; break;
		case 10: color = fat_pixel_sample(textures[10], tiledUV) * v_color; break;
		case 11: color = fat_pixel_sample(textures[11], tiledUV) * v_color; break;
		case 12: color = fat_pixel_sample(textures[12], tiledUV) * v_color; break;
		case 13: color = fat_pixel_sample(textures[13], tiledUV) * v_color; break;
		case 14: color = fat_pixel_sample(textures[14], tiledUV) * v_color; break;
		case 15: color = fat_pixel_sample(textures[15], tiledUV) * v_color; break;
	};

	//color.rgb *= color.a;
	// ^ this makes the black border come back lol

}