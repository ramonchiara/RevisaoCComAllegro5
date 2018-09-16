#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "allegro_util.h"

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
