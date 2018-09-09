#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

int main()
{
    ALLEGRO_DISPLAY *janela = NULL;
    ALLEGRO_PATH *dir = NULL;
    ALLEGRO_FONT *fonte = NULL;
    ALLEGRO_BITMAP *imagens[3] = { NULL, NULL, NULL };
    ALLEGRO_SAMPLE *som = NULL;
    ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
    ALLEGRO_EVENT evento;

    int sair = 0;
    int x, y, clique;
    int esq_clicado = 0, dir_clicado = 0;
    int img_indice = 0;

    if (!al_init()) {
        fprintf(stderr, "al_init\n");
        return -1;
    }

    if (!al_init_primitives_addon()) {
        fprintf(stderr, "al_init_primitives_addon\n");
        return -1;
    }

    al_init_font_addon();
    if (!al_init_ttf_addon()) {
        fprintf(stderr, "al_init_ttf_addon\n");
        al_shutdown_primitives_addon();
        return -1;
    }

    if (!al_init_image_addon()) {
        fprintf(stderr, "al_init_image_addon\n");
        al_shutdown_ttf_addon();
        al_shutdown_font_addon();
        al_shutdown_primitives_addon();
        return -1;
    }

    if (!al_install_audio()) {
        fprintf(stderr, "al_install_audio\n");
        al_shutdown_image_addon();
        al_shutdown_ttf_addon();
        al_shutdown_font_addon();
        al_shutdown_primitives_addon();
        return -1;
    }

    if(!al_init_acodec_addon()) {
        fprintf(stderr, "al_init_acodec_addon\n");
        al_uninstall_audio();
        al_shutdown_image_addon();
        al_shutdown_ttf_addon();
        al_shutdown_font_addon();
        al_shutdown_primitives_addon();
        return -1;
    }

    if (!al_reserve_samples(1)) {
        fprintf(stderr, "al_reserve_samples\n");
        al_uninstall_audio();
        al_shutdown_image_addon();
        al_shutdown_ttf_addon();
        al_shutdown_font_addon();
        al_shutdown_primitives_addon();
        return -1;
    }

    if (!al_install_mouse()) {
        fprintf(stderr, "al_install_mouse\n");
        al_uninstall_audio();
        al_shutdown_image_addon();
        al_shutdown_ttf_addon();
        al_shutdown_font_addon();
        al_shutdown_primitives_addon();
        return -1;
    }

    dir = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    if (!dir) {
        fprintf(stderr, "al_get_standard_path\n");
        al_uninstall_mouse();
        al_uninstall_audio();
        al_shutdown_image_addon();
        al_shutdown_ttf_addon();
        al_shutdown_font_addon();
        al_shutdown_primitives_addon();
        return -1;
    };

    al_set_path_filename(dir, "halo3.ttf");
    fonte = al_load_ttf_font(al_path_cstr(dir, '/'), 32, 0);
    if (!fonte) {
        fprintf(stderr, "al_load_ttf_font\n");
        al_destroy_path(dir);
        al_uninstall_mouse();
        al_uninstall_audio();
        al_shutdown_image_addon();
        al_shutdown_ttf_addon();
        al_shutdown_font_addon();
        al_shutdown_primitives_addon();
        return -1;
    }

    al_set_path_filename(dir, "img1.jpg");
    imagens[0] = al_load_bitmap(al_path_cstr(dir, '/'));
    if (!imagens[0]) {
        fprintf(stderr, "al_load_bitmap\n");
        al_destroy_font(fonte);
        al_destroy_path(dir);
        al_uninstall_mouse();
        al_uninstall_audio();
        al_shutdown_image_addon();
        al_shutdown_ttf_addon();
        al_shutdown_font_addon();
        al_shutdown_primitives_addon();
        return -1;
    }

    al_set_path_filename(dir, "img2.jpg");
    imagens[1] = al_load_bitmap(al_path_cstr(dir, '/'));
    if (!imagens[1]) {
        fprintf(stderr, "al_load_bitmap\n");
        al_destroy_bitmap(imagens[0]);
        al_destroy_font(fonte);
        al_destroy_path(dir);
        al_uninstall_mouse();
        al_uninstall_audio();
        al_shutdown_image_addon();
        al_shutdown_ttf_addon();
        al_shutdown_font_addon();
        al_shutdown_primitives_addon();
        return -1;
    }

    al_set_path_filename(dir, "img3.jpg");
    imagens[2] = al_load_bitmap(al_path_cstr(dir, '/'));
    if (!imagens[2]) {
        fprintf(stderr, "al_load_bitmap\n");
        al_destroy_bitmap(imagens[1]);
        al_destroy_bitmap(imagens[0]);
        al_destroy_font(fonte);
        al_destroy_path(dir);
        al_uninstall_mouse();
        al_uninstall_audio();
        al_shutdown_image_addon();
        al_shutdown_ttf_addon();
        al_shutdown_font_addon();
        al_shutdown_primitives_addon();
        return -1;
    }

    al_set_path_filename(dir, "slide.wav");
    som = al_load_sample(al_path_cstr(dir, '/'));
    if (!som) {
        fprintf(stderr, "al_load_sample\n");
        al_destroy_bitmap(imagens[2]);
        al_destroy_bitmap(imagens[1]);
        al_destroy_bitmap(imagens[0]);
        al_destroy_font(fonte);
        al_destroy_path(dir);
        al_uninstall_mouse();
        al_uninstall_audio();
        al_shutdown_image_addon();
        al_shutdown_ttf_addon();
        al_shutdown_font_addon();
        al_shutdown_primitives_addon();
        return -1;
    }

    janela = al_create_display(800, 600);
    if (!janela) {
        fprintf(stderr, "al_create_display\n");
        al_destroy_sample(som);
        al_destroy_bitmap(imagens[2]);
        al_destroy_bitmap(imagens[1]);
        al_destroy_bitmap(imagens[0]);
        al_destroy_font(fonte);
        al_destroy_path(dir);
        al_uninstall_mouse();
        al_uninstall_audio();
        al_shutdown_image_addon();
        al_shutdown_ttf_addon();
        al_shutdown_font_addon();
        al_shutdown_primitives_addon();
        return -1;
    }
    al_set_window_title(janela, "Revisão de C com Allegro");

    fila_eventos = al_create_event_queue();
    if (!fila_eventos) {
        fprintf(stderr, "al_create_event_queue\n");
        al_destroy_display(janela);
        al_destroy_sample(som);
        al_destroy_bitmap(imagens[2]);
        al_destroy_bitmap(imagens[1]);
        al_destroy_bitmap(imagens[0]);
        al_destroy_font(fonte);
        al_destroy_path(dir);
        al_uninstall_mouse();
        al_uninstall_audio();
        al_shutdown_image_addon();
        al_shutdown_ttf_addon();
        al_shutdown_font_addon();
        al_shutdown_primitives_addon();
        return -1;
    }
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_mouse_event_source());

    while(!sair) {
        clique = 0;
        while(!al_is_event_queue_empty(fila_eventos)) {
            al_wait_for_event(fila_eventos, &evento);

            if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                sair = 1;
            } else if (evento.type == ALLEGRO_EVENT_MOUSE_AXES) {
                x = evento.mouse.x;
                y = evento.mouse.y;
            } else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
                clique = evento.mouse.button;
            }
        }

        al_clear_to_color(al_map_rgb(255, 255, 255));

        if (x >= 100 && y >= 100 && x <= 300 && y <= 175) {
            if (clique) {
                al_play_sample(som, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                esq_clicado = 5;
                img_indice--;
                if (img_indice < 0) {
                    img_indice = 2;
                }
            } else {
                al_draw_filled_rectangle(100, 100, 300, 175, al_map_rgb(0xff, 0x99, 0x99));
                al_draw_rectangle(101, 101, 300, 175, al_map_rgb(0, 0, 0), 1);
            }
        } else {
            al_draw_filled_rectangle(100, 100, 300, 175, al_map_rgb(0xff, 0xcc, 0xcc));
            al_draw_rectangle(101, 101, 300, 175, al_map_rgb(0, 0, 0), 1);
        }
        if (esq_clicado) {
            al_draw_filled_rectangle(100, 100, 300, 175, al_map_rgb(0xff, 0x00, 0x00));
            al_draw_rectangle(101, 101, 300, 175, al_map_rgb(0, 0, 0), 1);
            esq_clicado--;
        }
        al_draw_text(fonte, al_map_rgb(0, 0, 0), 120, 120, 0, "Anterior");

        if (x >= 500 && y >= 100 && x <= 700 && y <= 175) {
            if (clique) {
                al_play_sample(som, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                dir_clicado = 5;
                img_indice++;
                if (img_indice >= 3) {
                    img_indice = 0;
                }
            } else {
                al_draw_filled_rectangle(500, 100, 700, 175, al_map_rgb(0x99, 0xff, 0x99));
                al_draw_rectangle(501, 101, 700, 175, al_map_rgb(0, 0, 0), 1);
            }
        } else {
            al_draw_filled_rectangle(500, 100, 700, 175, al_map_rgb(0xcc, 0xff, 0xcc));
            al_draw_rectangle(501, 101, 700, 175, al_map_rgb(0, 0, 0), 1);
        }
        if (dir_clicado) {
            al_draw_filled_rectangle(500, 100, 700, 175, al_map_rgb(0x00, 0xff, 0x00));
            al_draw_rectangle(501, 101, 700, 175, al_map_rgb(0, 0, 0), 1);
            dir_clicado--;
        }
        al_draw_text(fonte, al_map_rgb(0, 0, 0), 530, 120, 0, "Proxima");

        al_draw_bitmap(imagens[img_indice], 240, 225, 0);

        al_flip_display();
    }

    al_unregister_event_source(fila_eventos, al_get_mouse_event_source());
    al_unregister_event_source(fila_eventos, al_get_display_event_source(janela));
    al_destroy_event_queue(fila_eventos);
    al_destroy_display(janela);
    al_destroy_sample(som);
    al_destroy_bitmap(imagens[2]);
    al_destroy_bitmap(imagens[1]);
    al_destroy_bitmap(imagens[0]);
    al_destroy_font(fonte);
    al_destroy_path(dir);
    al_uninstall_mouse();
    al_uninstall_audio();
    al_shutdown_image_addon();
    al_shutdown_ttf_addon();
    al_shutdown_font_addon();
    al_shutdown_primitives_addon();
    return 0;
}
