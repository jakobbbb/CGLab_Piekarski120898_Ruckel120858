#version 150

#define PX 1.0/350

in vec2 TexCoord;
out vec4 out_Color;

// Screen Texture
uniform sampler2D Screen;

// post-processing effects
uniform bool HorizontalMirroring;
uniform bool VerticalMirroring;
uniform bool Grayscale;
uniform bool Blur;

float lumi = 0.0f;
vec3 result = vec3(0.0, 0.0, 0.0);


void main() {

    // invert texture coordinates around y-axis
    if (HorizontalMirroring) {
        TexCoord.y = 1.0 - TexCoord.y;
    }

    // invert texture coordinates around x-axis
    if (VerticalMirroring) {
        TexCoord.x = 1.0 - TexCoord.x;
    }

    out_Color = texture(Screen, TexCoord); 

    // Luminance Preserving Grayscale (from slides)
    if (Grayscale) {
        lumi = (0.2126 * out_Color.r + 
                0.7152 * out_Color.g + 
                0.0722 * out_Color.b);
        out_Color = vec4(lumi, lumi, lumi, 1.0);
    }

    if (Blur) {
        // 3x3 gaussian kernel matrix (from slides)
        gauss_kernel[9] = float[](
            1.0/16, 1.0/8, 1.0/16,
            1.0/8,  1.0/4, 1.0/8,
            1.0/16, 1.0/8, 1.0/16
        );

        // overwrite color with weighted sum 
        // of colors of the neighboring pixels
        pixels[9] = vec2[](
            vec2(-PX, PX),  vec2(0.0, PX),  vec2(PX, PX),
            vec2(-PX, 0.0), vec2(0.0, 0.0), vec2(PX, 0.0),
            vec2(-PX, -PX), vec2(0.0, -PX), vec2(PX, -PX)
        );
       
        // calculate color for all positions and sum up in result
        for (int i = 0; i < 9; i++) {
            result += vec3(texture(Screen, TexCoord.st + pixels[i])) * gauss_kernel[i];
        }

        out_Color = vec4(result, 1.0);     
    }
}