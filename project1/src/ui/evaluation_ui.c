#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

#include "evaluation_ui.h"
#include "../logic/common.h"
#include "../logic/matrix.h"
#include "../logic/dfa.h"
#include "../logic/list.h"
#include "../logic/controller.h"
#include "../test/test.h"

static GtkBuilder *builder;

static GtkWidget *prev_window;
static GtkWidget *evaluation_window;
static GtkWidget *evaluate_button;
static GtkWidget *final_state_label;
static GtkWidget *quit_button;
static GtkWidget *reset_button;
static GtkWidget *back_button;
static GtkWidget *states_route_label;
static GtkWidget *string_entry;

void init_widgets() {
    evaluation_window = GTK_WIDGET(gtk_builder_get_object(builder, "evaluation_window"));
    evaluate_button = GTK_WIDGET(gtk_builder_get_object(builder, "evaluate_button"));
    final_state_label = GTK_WIDGET(gtk_builder_get_object(builder, "final_state_label"));
    quit_button = GTK_WIDGET(gtk_builder_get_object(builder, "finish_button"));
    reset_button = GTK_WIDGET(gtk_builder_get_object(builder, "reset_button"));
    back_button = GTK_WIDGET(gtk_builder_get_object(builder, "back_button"));
    states_route_label = GTK_WIDGET(gtk_builder_get_object(builder, "states_route_label"));
    string_entry = GTK_WIDGET(gtk_builder_get_object(builder, "string_entry"));

}

int init_gui_eval(GtkWidget *previous_window) {
	prev_window = previous_window;
    builder = gtk_builder_new_from_file("src/ui/evaluation.ui");
    gtk_builder_connect_signals(builder, NULL);
    init_widgets();
    gtk_widget_show(evaluation_window);

    gtk_main();

    return OK;
}

void display_results(){
    char *input = (char *) malloc(gtk_entry_get_text_length (GTK_ENTRY(string_entry)));;
    strcpy(input, gtk_entry_get_text(GTK_ENTRY(string_entry)));
    int len_input = strlen(input);

    int *sequence = (int*)createList(len_input + 1, sizeof(int));
    fillList(sequence, len_input + 1);

    int result = execute_machine(input, sequence);
    
    g_print("Result: %d\n\n", result);

    if(result == 1) {
        gtk_label_set_text(GTK_LABEL(final_state_label), "Accepted");
    } else {
        gtk_label_set_text(GTK_LABEL(final_state_label), "Rejected");
    }

    char states_route[(strlen(input) + 1)*20];
    int index = 0;

    char **state_labels = get_state_labels();

    for (int i=0; i < strlen(input) + 1; i++) {
        g_print("\tElement[%d]: %s\n", i, state_labels[sequence[i]]);
        index += sprintf(&states_route[index], "%s\n", state_labels[sequence[i]]);
    }
        
    gtk_label_set_text(GTK_LABEL(states_route_label), states_route);
}

void on_evaluation_window_delete_event(GtkWidget *widget, gpointer data)
{
	free_config();
	gtk_widget_destroy(GTK_WIDGET(prev_window));
	gtk_widget_destroy(GTK_WIDGET(evaluation_window));
	exit(EXIT_SUCCESS);
}

void on_evaluate_button_clicked(GtkButton * b) {
    display_results();
}

void on_reset_button_clicked(GtkButton * b) {
    gtk_label_set_text(GTK_LABEL(final_state_label), "");
    gtk_label_set_text(GTK_LABEL(states_route_label), "");
    gtk_entry_set_text(GTK_ENTRY(string_entry), "");
}

void on_back_button_clicked(GtkButton *b)
{
	gtk_widget_show(prev_window);
	gtk_widget_destroy(GTK_WIDGET(evaluation_window));
}

void on_quit_button_clicked(GtkButton * b) {
    g_signal_emit_by_name(evaluation_window, "delete-event");
}

void on_string_entry_insert_text(
                        GtkEntry *entry,
                        const gchar *text,
                        gint length,
                        gint *position,
                        gpointer data) 
{
    GtkEditable *editable = GTK_EDITABLE (entry);
    int i, count = 0;
    gchar *result = g_new (gchar, length);

    char *alphabet = get_symbols();

    for (i=0; i < length; i++)
    {
        //Verify if the char is in the alphabet
        if (!strchr(alphabet, text[i]))
            continue;
        result[count++] = text[i];
    }

    if (count > 0)
    {
        g_signal_handlers_block_by_func(G_OBJECT (editable), G_CALLBACK (on_string_entry_insert_text), data);
        gtk_editable_insert_text(editable, result, count, position);
        
        g_signal_handlers_unblock_by_func(G_OBJECT (editable), G_CALLBACK (on_string_entry_insert_text), data);
    }

    g_signal_stop_emission_by_name(G_OBJECT (editable), "insert_text");
    g_free (result);
}

