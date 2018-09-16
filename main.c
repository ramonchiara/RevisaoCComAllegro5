#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_PATH *dir = NULL;
ALLEGRO_FONT *fonte = NULL;
ALLEGRO_BITMAP *imagens[3] = { NULL, NULL, NULL };
ALLEGRO_SAMPLE *som = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;

int inicializa_allegro(void);
void finaliza_allegro(void);

int main()
{
    ALLEGRO_EVENT evento;

    int sair = 0;
    int x, y, clique;
    int esq_clicado = 0, dir_clicado = 0;
    int img_indice = 0;

    if (!inicializa_allegro()) {
        return -1;
    }

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

    finaliza_allegro();
    return 0;
}

int inicializa_font(void)
{
    al_init_font_addon();
    return al_init_ttf_addon();
}

ALLEGRO_FONT *carrega_fonte(char *filename)
{
    al_set_path_filename(dir, filename);
    return al_load_ttf_font(al_path_cstr(dir, '/'), 32, 0);
}

ALLEGRO_BITMAP *carrega_imagem(char *filename)
{
    al_set_path_filename(dir, filename);
    return al_load_bitmap(al_path_cstr(dir, '/'));
}

ALLEGRO_SAMPLE *carrega_som(char *filename)
{
    al_set_path_filename(dir, filename);
    return al_load_sample(al_path_cstr(dir, '/'));
}

int inicializa_allegro(void)
{
    int sucesso = 0;

    if (!al_init()) {
        fprintf(stderr, "al_init\n");
    } else if (!al_init_primitives_addon()) {
        fprintf(stderr, "al_init_primitives_addon\n");
    } else if (!inicializa_font()) {
        fprintf(stderr, "inicializa_font\n");
    } else if (!al_init_image_addon()) {
        fprintf(stderr, "al_init_image_addon\n");
    } else if (!al_install_audio()) {
        fprintf(stderr, "al_install_audio\n");
    } else if(!al_init_acodec_addon()) {
        fprintf(stderr, "al_init_acodec_addon\n");
    } else if (!al_reserve_samples(1)) {
        fprintf(stderr, "al_reserve_samples\n");
    } else if (!al_install_mouse()) {
        fprintf(stderr, "al_install_mouse\n");
    } else if (!(dir = al_get_standard_path(ALLEGRO_RESOURCES_PATH))) {
        fprintf(stderr, "al_get_standard_path\n");
    } else if (!(fonte = carrega_fonte("halo3.ttf"))) {
        fprintf(stderr, "carrega_fonte\n");
    } else if (!(imagens[0] = carrega_imagem("img1.jpg"))) {
        fprintf(stderr, "carrega_imagem\n");
    } else if (!(imagens[1] = carrega_imagem("img2.jpg"))) {
        fprintf(stderr, "carrega_imagem\n");
    } else if (!(imagens[2] = carrega_imagem("img3.jpg"))) {
        fprintf(stderr, "carrega_imagem\n");
    } else if (!(som = carrega_som("slide.wav"))) {
        fprintf(stderr, "carrega_som\n");
    } else if (!(janela = al_create_display(800, 600))) {
        fprintf(stderr, "al_create_display\n");
    } else if (!(fila_eventos = al_create_event_queue())) {
        fprintf(stderr, "al_create_event_queue\n");
    } else {
        sucesso = 1;

        al_set_window_title(janela, "Revisão de C com Allegro");
        al_register_event_source(fila_eventos, al_get_display_event_source(janela));
        al_register_event_source(fila_eventos, al_get_mouse_event_source());
    }

    if (!sucesso) {
        finaliza_allegro();
    }

    return sucesso;
}

void finaliza_allegro(void)
{
    if (fila_eventos) {
        al_unregister_event_source(fila_eventos, al_get_mouse_event_source());
        al_unregister_event_source(fila_eventos, al_get_display_event_source(janela));
        al_destroy_event_queue(fila_eventos);
    }
    if (janela) al_destroy_display(janela);
    if (som) al_destroy_sample(som);
    if (imagens[2]) al_destroy_bitmap(imagens[2]);
    if (imagens[1]) al_destroy_bitmap(imagens[1]);
    if (imagens[0]) al_destroy_bitmap(imagens[0]);
    if (fonte) al_destroy_font(fonte);
    if (dir) al_destroy_path(dir);
    al_uninstall_mouse();
    al_uninstall_audio();
    al_shutdown_image_addon();
    al_shutdown_ttf_addon();
    al_shutdown_font_addon();
    al_shutdown_primitives_addon();
}
