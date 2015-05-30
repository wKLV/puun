#include "text.h"
#include "gl_help.c"
#include <math.h>
#define STB_TRUETYPE_IMPLEMENTATION
#include "../../other/stb_truetype.h"


gf_textStyle initTextStyle(char* font_pos,
        f32 pixels, u32 color) {
    gf_textStyle s = {0};

    stbtt_fontinfo* font= malloc(sizeof(stbtt_fontinfo));
    char* buffer = malloc(1000000);
    fread(buffer, 1, 1000000, fopen(font_pos, "rb"));
    stbtt_InitFont(font, buffer, 0);

    s.id = ++gfTextStyles_length;
    s.pixels = pixels;
    s.color = color;

    struct gfFontData* data = &gfTextStyles[s.id];
    data->font = font;
    data->atlas = malloc(1024*1024);
    data->cdata = malloc(96*sizeof(stbtt_bakedchar));

    stbtt_BakeFontBitmap(buffer, 0, s.pixels,
            data->atlas, 1024, 1024, 32, 96,
            data->cdata);


    glGenTextures(1, &data->textureId);
    glBindTexture(GL_TEXTURE_2D, data->textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 1024,1024, 0,
            GL_ALPHA, GL_UNSIGNED_BYTE, data->atlas);
    // can free temp_bitmap at this point
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    char vs[] = "attribute vec2 position; attribute vec2 uv;\n\
                 varying vec2 texcoord; void main(){\n\
                     gl_Position=vec4(position/256., 0, 1);\n\
                    texcoord=uv;}";
    char fs[] = "precision mediump float; \n\
                 uniform sampler2D texture; \n\
                 varying vec2 texcoord; \n\
                 void main() {\n\
                     float c = texture2D(texture, texcoord).a;\n\
                     c = 1. -c;\n\
                     gl_FragColor = vec4(c, c, c, 1.-c);\n\
                 }\n";

    data->glProgram = setupProgram(vs, strlen(vs), fs, strlen(fs));

    glGenBuffers(1, &data->glBuffer);
    return s;
}

bool gfTextFit(gfText gfText) {
    gf_textStyle style = gfText.style;
    BBox bbox = gfText.bbox;
    char* text = gfText.text;
    stbtt_fontinfo* font = (stbtt_fontinfo*) gfTextStyles[style.id].font;

    f32 scale = stbtt_ScaleForPixelHeight(
            font, style.pixels);
    int i,j, ascent, descent, lineGap, baseline, ch=0;

    stbtt_GetFontVMetrics(font, &ascent, &descent, &lineGap);
    f32 xpos = 0, ypos = 0;

    while(text[ch]) {
        int advance,lsb,x0,y0,x1,y1;
        float x_shift = xpos - (float) floor(xpos);
        stbtt_GetCodepointHMetrics(font, text[ch],
                &advance, &lsb);
        xpos += (advance * scale);
        if (text[ch+1])
            xpos += scale*stbtt_GetCodepointKernAdvance(
                    font, text[ch],text[ch+1]);
        if(xpos>bbox.w) {
            ypos += ascent -descent + lineGap;
            xpos = 0;

            stbtt_GetCodepointHMetrics(font, text[ch],
                    &advance, &lsb);
            xpos += (advance * scale);
            if (text[ch+1])
                xpos += scale*stbtt_GetCodepointKernAdvance(
                        font, text[ch],text[ch+1]);
        }
        if(ypos>bbox.h) return false;
        ++ch;
    }
    return true;

}

void gfTextRender(gfText gfText) {
    gf_textStyle style = gfText.style;
    BBox bbox = gfText.bbox;
    char* text = gfText.text;
#if 0
    stbtt_fontinfo* font = (stbtt_fontinfo*) gfTextStyles[style.id].font;

    f32 scale = stbtt_ScaleForPixelHeight(
            font, style.pixels);
    int i,j, ascent, descent, lineGap, baseline, ch=0;

    stbtt_GetFontVMetrics(font, &ascent, &descent, &lineGap);
#endif

    struct gfFontData fontData = gfTextStyles[style.id];
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, fontData.textureId);

    glUseProgram(fontData.glProgram);


    int ch=0;
    f32 xpos = -256, ypos = 0;
    u32 length = strlen(text);
    float* vertex = malloc(length*4*6*sizeof(float));
    int i = 0;

    while(text[ch]) {
        stbtt_aligned_quad quad;
        stbtt_GetBakedQuad(fontData.cdata,
                1024, 1024,
                text[ch]-32, &xpos, &ypos, &quad, true);

        //Position 1
        vertex[i] = quad.x0; i++;
        vertex[i] = quad.y0; i++;
        vertex[i] = quad.s0; i++;
        vertex[i] = quad.t1; i++;
        //Position 2
        vertex[i] = quad.x1; i++;
        vertex[i] = quad.y0; i++;
        vertex[i] = quad.s1; i++;
        vertex[i] = quad.t1; i++;
        //Position 3
        vertex[i] = quad.x1; i++;
        vertex[i] = quad.y1; i++;
        vertex[i] = quad.s1; i++;
        vertex[i] = quad.t0; i++;
        //Position 3
        vertex[i] = quad.x1; i++;
        vertex[i] = quad.y1; i++;
        vertex[i] = quad.s1; i++;
        vertex[i] = quad.t0; i++;
        //Position 4
        vertex[i] = quad.x0; i++;
        vertex[i] = quad.y1; i++;
        vertex[i] = quad.s0; i++;
        vertex[i] = quad.t0; i++;
        //Position 1
        vertex[i] = quad.x0; i++;
        vertex[i] = quad.y0; i++;
        vertex[i] = quad.s0; i++;
        vertex[i] = quad.t1; i++;

#if 0
        int advance,lsb,x0,y0,x1,y1;
        float x_shift = xpos - (float) floor(xpos);

        stbtt_GetCodepointHMetrics(font, text[ch],
                &advance, &lsb);
        xpos += (advance * scale);
        if (text[ch+1])
            xpos += scale*stbtt_GetCodepointKernAdvance(
                    font, text[ch],text[ch+1]);
        if(xpos>bbox.w) {
            ypos += ascent -descent + lineGap;
            xpos = 0;

            stbtt_GetCodepointHMetrics(font, text[ch],
                    &advance, &lsb);
            xpos += (advance * scale);
            if (text[ch+1])
                xpos += scale*stbtt_GetCodepointKernAdvance(
                        font, text[ch],text[ch+1]);
        }
        if(ypos>bbox.h) return;*/
#endif
        ++ch;
    }

    GLint pos = glGetAttribLocation(fontData.glProgram, "position");
    GLint uv = glGetAttribLocation(fontData.glProgram, "uv");

    u8 stride = 4*sizeof(float);

    glBindBuffer(GL_ARRAY_BUFFER, fontData.glBuffer);
    glBufferData(GL_ARRAY_BUFFER, length*6*stride,
            vertex, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE,
            stride, 0);
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(uv, 2, GL_FLOAT, GL_FALSE,
            stride, (Data)(2*sizeof(float)));
    glEnableVertexAttribArray(uv);

    glDrawArrays(GL_TRIANGLES, 0, length*6);
}



// TODO Implement this
// TODO Problem: How should we try to create the BB?
// Save horizontal space? vertical? maximum? quite complicated
#if 0
BBox gfTextNeeded(char* text, gf_textStyle style) {
    f32 scale = stbtt_ScaleForPixelHeight(
            (stbtt_fontinfo*) style.font,
            style.pixels);


}
#endif
