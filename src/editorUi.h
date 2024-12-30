#ifndef EDITORUI_H
#define EDITORUI_H

#include <GLFW/glfw3.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT

#include <Nuklear/nuklear.h>
#include <Nuklear/nuklear_glfw_gl3.h>

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

/* Nuklear-GLFW structure */
static struct nk_glfw glfw = {0};

void editorui_init(GLFWwindow *window)
{
    /* Initialize Nuklear with GLFW and OpenGL */
    nk_glfw3_init(&glfw, window, NK_GLFW3_INSTALL_CALLBACKS);

    // Fonts
    struct nk_font_atlas *atlas;
    nk_glfw3_font_stash_begin(&glfw, &atlas);
    nk_glfw3_font_stash_end(&glfw);
}

void editorui_terminate() 
{
    nk_glfw3_shutdown(&glfw);
}

void editorui_update()
{
    nk_glfw3_new_frame(&glfw);
}

void editorui_renderViewport(unsigned int *texture, int *w, int *h)
{
    struct nk_context *ctx = &glfw.ctx;
    struct nk_rect rect = nk_rect((*w * 0.5), (*h * 0.5), *w, *h);

    if (nk_begin(ctx, "Scene", rect, NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE))
    {
        // Get the content region of the Nuklear window
        struct nk_rect bounds = nk_window_get_content_region(ctx);
        *w = (int)bounds.w;
        *h = (int)bounds.h;

        // Retrieve the drawing buffer
        struct nk_command_buffer *cmd = nk_window_get_canvas(ctx);

        // Calculate the image rectangle
        struct nk_rect img_rect = nk_rect(bounds.x, bounds.y, bounds.w, bounds.h);

        // Create Nuklear image from texture
        struct nk_image img = nk_image_id((int)*texture);

        // Draw the image
        nk_draw_image(cmd, img_rect, &img, nk_white);
    }
    nk_end(ctx);
}

void editorui_render(GLFWwindow *window)
{
    /*if (nk_begin(&glfw.ctx, "Nuklear Window", nk_rect(150, 150, 500, 500),
    NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE)) 
    {
        nk_layout_row_static(&glfw.ctx, 30, 80, 1);
        if (nk_button_label(&glfw.ctx, "Button")) {
            printf("Button Pressed!\n");
        }
        nk_end(&glfw.ctx);
    }*/


    // glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    nk_glfw3_render(&glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
}

#endif