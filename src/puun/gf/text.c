#include "text.h"
#include "gl_help.c"
#include <math.h>
#define STB_TRUETYPE_IMPLEMENTATION
#include "../../other/stb_truetype.h"

struct gfFontData gfTextStyles[255] = {};
u8 gfTextStyles_length = 0;

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../other/stb_image_write.h"

gf_textStyle initTextStyle(u8* font_pos,
        f32 pixels, u32 color) {
    gf_textStyle s = {};

    stbtt_fontinfo* font= malloc(sizeof(stbtt_fontinfo));
    u8* buffer = malloc(1000000);
    fread((char*)buffer, 1, 1000000, fopen((char*)font_pos, "rb"));
    stbtt_InitFont(font, buffer, stbtt_GetFontOffsetForIndex(buffer, 0U));

    s.id = ++gfTextStyles_length;
    s.pixels = pixels;
    s.color = color;

    struct gfFontData* data = &gfTextStyles[s.id];
    data->font = font;
   // data->atlas = {};
    data->cdata = malloc(96*sizeof(stbtt_bakedchar));

    float scale = stbtt_ScaleForPixelHeight(font, pixels);
#if 1
    u8 c;
    u32 lastPos;
    for(c = 0x30, lastPos=0; c<0x7a; c++) {
        int ix0, ix1, iy0, iy1;
        u8* location = data->atlas[lastPos];
        assert(lastPos<gf_TEXT_LENGTH);
        stbtt_GetCodepointBitmapBox(font, c, scale, scale, &ix0, &iy0, &ix1, &iy1);
        int width = - ix0 + ix1;
        int height = - iy0 + iy1;
        stbtt_MakeCodepointBitmap(font, location, width, height, gf_TEXT_STRIDE, scale, scale, c);


        struct gfCharacterData ch = {};
        ch.beggining = lastPos*1.f / gf_TEXT_LENGTH;
        ch.end = (lastPos += height)*1.f /gf_TEXT_LENGTH;
        ch.width = width*1.f;
        ch.height =  - height*1.f;
        ch.offsetx = ix0;
        ch.offsety = -iy0;
        data->characters[c] = ch;
        //1 pixel margin
        lastPos += 1;

    }
    printf("lastPos:%d; lastLoc:%x\n", lastPos, lastPos*gf_TEXT_STRIDE);
#else
    stbtt_BakeFontBitmap(buffer, 0, s.pixels,
            data->atlas, 1024, 1024, 32, 96,
            data->cdata);
#endif

    stbi_write_png("prueba", gf_TEXT_STRIDE, gf_TEXT_LENGTH, 1, data->atlas, gf_TEXT_STRIDE);
    data->textureId = setupTexture_Any(data->atlas, gf_TEXT_STRIDE, gf_TEXT_LENGTH, GL_R8, GL_RED, GL_UNSIGNED_BYTE);
//glGenTextures(1, &data->textureId);
//glBindTexture(GL_TEXTURE_2D, data->textureId);
//glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, gf_TEXT_STRIDE,gf_TEXT_LENGTH, 0,
//        GL_ALPHA, GL_UNSIGNED_BYTE, data->atlas);
//// can free temp_bitmap at this point
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    u8 vs[] = "attribute vec2 position; attribute vec2 uv;\n\
                 varying vec2 texcoord; void main(){\n\
                     gl_Position=vec4(position/256, 0, 1);\n\
                    texcoord=uv;}";
    u8 fs[] = "precision mediump float; \n\
                 uniform sampler2D texture; \n\
                 varying vec2 texcoord; \n\
                 void main() {\n\
                     float c = texture2D(texture, texcoord).r;\n\
                       c=1.0-c;\n\
                       gl_FragColor = vec4(c, c, c, 1.0);\n\
                //     gl_FragColor =  texture2D(texture, texcoord).rgba;\n\
               //      gl_FragColor = vec4(texcoord, 0., 1.0);\n\
                    // gl_FragColor = vec4(0.5, 0.75, 0.25, 1.0);\n\
                 }\n";

    data->glProgram = setupProgram(vs, strlen((char*)vs), fs, strlen((char*)fs));

    glGenBuffers(1, &data->glBuffer);
    return s;
}

bool gfTextFit(gfText gfText) {
    gf_textStyle style = gfText.style;
    BBox bbox = gfText.bbox;
    u8* text = gfText.text;
    stbtt_fontinfo* font = (stbtt_fontinfo*) gfTextStyles[style.id].font;

    f32 scale = stbtt_ScaleForPixelHeight(
            font, style.pixels);
    int ascent, descent, lineGap, ch=0; // UNUSED baseline,

    stbtt_GetFontVMetrics(font, &ascent, &descent, &lineGap);
    f32 xpos = 0, ypos = 0;
    while(text[ch]) {
        int advance,lsb;//x0,y0,x1,y1;
        // UNUSED float x_shift = xpos - (float) floor(xpos);
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
    u8* text = gfText.text;
    stbtt_fontinfo* font = (stbtt_fontinfo*) gfTextStyles[style.id].font;

    f32 scale = stbtt_ScaleForPixelHeight(
            font, style.pixels);
    int i, ascent, descent, lineGap; //TODO VERTICAL MOVEMENT

    stbtt_GetFontVMetrics(font, &ascent, &descent, &lineGap);
    f32 xpos = bbox.x, ypos = bbox.y;

    u32 length = strlen((char*)text) +0;
    struct gfFontData fontData = gfTextStyles[style.id];
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, fontData.textureId);

    glUseProgram(fontData.glProgram);
    float* vertex = malloc(length*4*6*sizeof(float));

    i = 0;
#if 0
        vertex[i] = -256.f; i++;
        vertex[i] = -256.f; i++;
        vertex[i] = 0; i++;
        vertex[i] = 0; i++;
        //Position 2
        vertex[i] = 256.f; i++;
        vertex[i] = -256.f; i++;
        vertex[i] = 1; i++;
        vertex[i] = 0.f; i++;
        //Position 3
        vertex[i] = 256.f; i++;
        vertex[i] = 256.f; i++;
        vertex[i] = 1; i++;
        vertex[i] = 1.f; i++;
        //Position 3
        vertex[i] = 256.f; i++;
        vertex[i] = 256.f; i++;
        vertex[i] = 1.f; i++;
        vertex[i] = 1.f; i++;
        //Position 4
        vertex[i] = -256.f; i++;
        vertex[i] = 256.f; i++;
        vertex[i] = 0; i++;
        vertex[i] = 1.f; i++;
        //Position 1
        vertex[i] = -256.f; i++;
        vertex[i] = -256.f; i++;
        vertex[i] = 0; i++;
        vertex[i] = 0; i++;
#endif
    while(*text) {
#if 0
        float any = 0;
        stbtt_aligned_quad quad;
        stbtt_GetBakedQuad(fontData.cdata,
                1024, 1024,
                text[ch]-32, &any, &any, &quad, true);
#endif
        struct gfCharacterData cd = fontData.characters[*text];

//      cd.offsetx =0;
//      cd.offsety=0;
//      cd.width = 5;
//      cd.height = 5;
        //Posi++tion 1
        vertex[i++] = xpos + cd.offsetx;
        vertex[i++] = ypos + cd.offsety;
        vertex[i++] = 0;
        vertex[i++] = cd.beggining;
        //Position 2
        vertex[i++] = xpos + cd.offsetx + cd.width;
        vertex[i++] = ypos + cd.offsety;
        vertex[i++] = 1.f*cd.width/gf_TEXT_STRIDE;
        vertex[i++] = cd.beggining;
        //Position 3
        vertex[i++] = xpos + cd.offsetx+ cd.width;
        vertex[i++] = ypos + cd.offsety+ cd.height;
        vertex[i++] = 1.f*cd.width/gf_TEXT_STRIDE;
        vertex[i++] = cd.end;
        //Position 3
        vertex[i++] = xpos + cd.offsetx+ cd.width;
        vertex[i++] = ypos + cd.offsety+ cd.height;
        vertex[i++] = 1.f*cd.width/gf_TEXT_STRIDE;
        vertex[i++] = cd.end;
        //Position 4
        vertex[i++] = xpos + cd.offsetx;
        vertex[i++] = ypos + cd.offsety + cd.height;
        vertex[i++] = 0;
        vertex[i++] = cd.end;
        //Position 1
        vertex[i++] = xpos + cd.offsetx;
        vertex[i++] = ypos + cd.offsety;
        vertex[i++] = 0;
        vertex[i++] = cd.beggining;

        int advance,lsb;//,x0,y0,x1,y1;
        // UNUSED float x_shift = xpos - (float) floor(xpos);
        stbtt_GetCodepointHMetrics(font, *text,
                &advance, &lsb);
        xpos += (advance * scale);
        if (text[1])
            xpos += scale*stbtt_GetCodepointKernAdvance(
                    font, text[0],text[1]);
#if 0
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
#endif
        ++text;
    }
    GLint pos = glGetAttribLocation(fontData.glProgram, "position");
    GLint uv = glGetAttribLocation(fontData.glProgram, "uv");

    u8 stride = 4*sizeof(float);

    glBindBuffer(GL_ARRAY_BUFFER, fontData.glBuffer);
    glBufferData(GL_ARRAY_BUFFER, length*6*stride,
            vertex, GL_DYNAMIC_DRAW);
    free(vertex);
    glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE,
            stride, 0);
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(uv, 2, GL_FLOAT, GL_FALSE,
            stride, (Data)(2*sizeof(float)));
    glEnableVertexAttribArray(uv);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fontData.textureId);
    glUniform1i(glGetUniformLocation(fontData.textureId, "texture"), 0);

    glDrawArrays(GL_TRIANGLES, 0, length*6);
}
#if 0
void gfTextRenderChungo(gfText gfText) {
    gf_textStyle style = gfText.style;
    BBox bbox = gfText.bbox;
    u8* text = gfText.text;
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
    f32 xpos = gfText.bbox.x, ypos = gfText.bbox.y;
    u32 length = strlen((char*)text);
    float* vertex = malloc(length*4*6*sizeof(float));
    int i = 0;

    while(text[ch]) {
        stbtt_aligned_quad quad;
        stbtt_GetBakedQuad(fontData.cdata,
                1024, 1024,
                text[ch]-32, &xpos, &ypos, &quad, 1);

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
        if(ypos>bbox.h) return;
#endif
        ++ch;
    }

    GLint pos = glGetAttribLocation(fontData.glProgram, "position");
    GLint uv = glGetAttribLocation(fontData.glProgram, "uv");

    u8 stride = 4*sizeof(float);

    glBindBuffer(GL_ARRAY_BUFFER, fontData.glBuffer);
    glBufferData(GL_ARRAY_BUFFER, length*6*stride,
            vertex, GL_DYNAMIC_DRAW);
    free(vertex);
    glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE,
            stride, 0);
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(uv, 2, GL_FLOAT, GL_FALSE,
            stride, (Data)(2*sizeof(float)));
    glEnableVertexAttribArray(uv);

    glDrawArrays(GL_TRIANGLES, 0, length*6);
}
#endif


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
