#version 410

// Input
layout(location = 0) in vec2 texture_coord;
layout(location = 1) in vec2 texture_coord1;

// Uniform properties
uniform sampler2D textureImage;
uniform sampler2D textureImage1;

uniform ivec2 screenSize;
uniform int flipVertical;
uniform int outputMode = 2; // 0: original, 1: grayscale, 2: blur

// Output
layout(location = 0) out vec4 out_color;
vec4 s1, s2;
// Local variables
vec2 textureCoord = vec2(texture_coord.x, (flipVertical != 0) ? 1 - texture_coord.y : texture_coord.y); // Flip texture
vec2 textureCoord1 = vec2(texture_coord1.x, (flipVertical != 0) ? 1 - texture_coord1.y : texture_coord1.y); // Flip texture
float threshold = 1.2;
float theta;
vec2 texelSize  = 1.0f / screenSize;
vec2 texture_size = textureSize(textureImage, 0);

vec4 grayscale()
{
    vec4 color = texture(textureImage, textureCoord);
    float gray = 0.21 * color.r + 0.71 * color.g + 0.07 * color.b; 
    return vec4(gray, gray, gray,  0);
}

vec4 blur(int blurRadius, int k, int l)
{
    vec2 texelSize = 1.0f / screenSize;
    vec4 sum = vec4(0);
    for(int i = -blurRadius; i <= blurRadius; i++)
    {
        for(int j = -blurRadius; j <= blurRadius; j++)
        {
            sum += texture(textureImage, textureCoord + vec2(i, j) * texelSize + vec2(k, l) * texelSize);
        }
    }
        
    float samples = pow((2 * blurRadius + 1), 2);
    return sum / samples;
}

float avg_gray(vec4 pixel)
{
	return (pixel.r + pixel.g + pixel.b) / 3;
}

vec4 median()
{
	const int radius = 4;
	vec4 to_sort[82];
	vec2 texelSize = 1.0f / screenSize;

	//Collect all pixels
	for(int i = -radius; i <= radius; i++)
	{
		for(int j = -radius; j <= radius; j++)
		{
			to_sort[(2 * radius + 1) * (i + 2) + (j + 2)] = texture(textureImage, textureCoord + vec2(i, j) * texelSize);
		}
	}
	
	//Sort them
	for (int i = 0; i < (2 * radius + 1) * (2 * radius + 1); ++i)
	{
		for (int j = 0; j <= (2 * radius + 1) * (2 * radius + 1); ++j)
		{
			if (avg_gray(to_sort[i]) > avg_gray(to_sort[j]))
			{
				vec4 aux = to_sort[i];
				to_sort[i] = to_sort[j];
				to_sort[j] = aux;
			}
		}
	}

	//Return the middle value
	return to_sort[(2 * radius + 1) * (2 * radius + 1) / 2];
}

float blurGray(int i, int j)
{
    float p;
    vec4 P = blur(1, i, j);
    p  = 0.21 * P.r + 0.71 * P.g + 0.07 * P.b;
    return p;
}
void getNPS(float theta) {
    vec2 texelSize = 1.0f / screenSize;
  
	if (theta == 0) {
		s1 = blur(1, -1, 0);
		s2 = blur(1, 1, 0);
    
	}
	if (theta == 45) {
		s1 = blur(1, 1, 1);
		s2 = blur(1, -1, -1);
      
	}
	if (theta == 90) {
		s1 = blur(1, 0, 1);
		s2 = blur(1, 0, -1);
    
	}
	if (theta == 135) {
		s1 = blur(1, -1, 1);
		s2 = blur(1, 1, -1);
   
	}
 

}
vec4 direction = vec4(0.0);
vec4 magnitude = vec4(0.0);


 const mat3 Dx = mat3(
	-1.0, 0.0, 1.0,
	-2.0, 0.0, 2.0,
	-1.0, 0.0, 1.0
);

const mat3 Dy = mat3(
	 1.0,  2.0,  1.0,
	 0.0,  0.0,  0.0,
	-1.0, -2.0, -1.0
);
int getDirection(float Dx, float Dy) {
	if (Dy == 0.0)	return 0;
	if (Dx == 0.0)	return 90;

	theta = abs(atan(Dy/Dx));
	if (theta < 0)
		theta+= 180;
	if (theta > 0 && theta < 22.5)
		return 0;
	if (theta >= 22.5 && theta < 67.5)
		return 45;
	if (theta >= 67.5 && theta < 112.5)
		return 90;
	if (theta >= 112.5 && theta < 157.5)
		return 135;
	if (theta >= 157.5 && theta <= 180.0)
		return 0;
}

vec3 getDirections(vec3 Dx, vec3 Dy) {
	int r = getDirection(Dx.r, Dy.r);
	int g = getDirection(Dx.g, Dy.g);
	int b = getDirection(Dx.b, Dy.b);
	return vec3(r, g, b);
}


 vec4 sobel() {
	int x, y; 
	vec3 dx = vec3(0.0);
	vec3 dy = vec3(0.0);
    float sample_;
	vec3 color = vec3(0.0); // gradient magnitude
	for (y = 0; y < 3; y += 1) {
		for (x = 0; x < 3; x += 1) {
			sample_ = blurGray(x - 1, y - 1);			
			dx += Dx[y][x] * sample_;
			dy += Dy[y][x] * sample_;
		}
	}
	color = abs(dx) + abs(dy);
	direction = vec4(getDirections(dx, dy), 1.0);
	
	return vec4(color, 1.0);
}
vec4 non_max_surpression() {
	vec4 color = vec4(1.0);
	vec4 theta = direction;

	getNPS(theta.r);
	float S1 = 0.21 * s1.r + 0.71 * s1.g + 0.07 * s1.b;
	float S2 = 0.21 * s2.r + 0.71 * s2.g + 0.07 * s2.b;
	if (magnitude.r >= S1 && magnitude.r >= S1)
		color.r = magnitude.r;
	
	getNPS(theta.g);
	if (magnitude.g >= S1 && magnitude.g >= S2)
		color.g = magnitude.g;

	getNPS(theta.b);
	if (magnitude.b >= S1 && magnitude.b >= S2) 
		color.b = magnitude.b;
	
	return color;
}


void main()
{
    switch (outputMode)
    {
        case 1:
        {
            out_color = median();
            break;
        }

        case 2:
        {
            magnitude = sobel();
            out_color = non_max_surpression(); 
			
			
            break;
        }
        case 3:
        {
           magnitude =  sobel();
		   vec4 transparent = texture(textureImage1, textureCoord1);
           vec4 color = non_max_surpression(); 
		   if(color.r == 1 && color.g == 1 && color.b == 1) {
				color.a = 0;

				}
			out_color = mix(1-color, transparent, 0.5);
           break;
        }


        default:
            out_color = texture(textureImage, textureCoord);
			
            break;
    }
}