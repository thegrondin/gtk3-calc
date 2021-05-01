#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <ctype.h>
#include <math.h>

char * equation_str;
int equation_str_size = 1;

GtkWidget *entry;

static int get_exponent(char * array, int position) {

    int result = 0;

    while (isdigit(array[position])) {

        position++;
        result++;

    }

    return result-1;

}

static void accumulate_result(char data) {
    
    char action = (char)data;

    if (equation_str_size != 1 && !isdigit(action) && !isdigit(equation_str[equation_str_size - 2])) {
        equation_str[equation_str_size - 2] = action;

        return;
    }

    equation_str_size++;

    if (equation_str_size == 0) {
        equation_str = malloc(sizeof(char) * equation_str_size);
        equation_str[1] = '\0';
    }
    else {
        equation_str = realloc(equation_str, sizeof(char) * equation_str_size);
    }


    equation_str[equation_str_size - 2] = action;
    equation_str[equation_str_size - 1] = '\0';

}

static float calculate(float nb1, float nb2, char sign) {

    g_print("nb1 : %f, nb2 : %f, sign : %c\n", nb1, nb2, sign);

    switch (sign)
    {
    case '=':
        return nb1;
    case '/':
        return nb1 / nb2;
    case 'x':
        return nb1 * nb2;
    case '-':
        return nb1 - nb2;
    case '+':
        return nb1 + nb2;
    default:
        g_print("DEFAULTED\n");
        return 0;
    }

}

static float get_result() {
    char* number_acc;
    int number_acc_size = 1;

    char last_sign;
    

    float result = 0;
    float current = 0;


    for (int i = 0; i < equation_str_size - 1; i++) {

       
        if (isdigit(equation_str[i])) {

            g_print("CURRENT BEFORE: %f\n", current);
            current = (current + (pow(10, get_exponent(equation_str, i)) * (equation_str[i] - '0')));
            g_print("CURRENT : %f\n", current);

        }
        else {

            char sign = equation_str[i];

            if (result == 0) {
                result = current;
            }
            else {
                g_print("LAST_SIGN : %c\n", last_sign);
                result = calculate(result, current, last_sign);
            }

            current = 0;
            last_sign = sign;
        }
    }


    return result;
}

static void exec_command(GtkWidget *widget, gpointer data) {

    char command = (char)data;

    accumulate_result(command);

    gtk_entry_set_text(entry, equation_str);

    if (command == '=') {
        
        float result = get_result();
        g_print("RESULT: %f\n", result);
        
        

        return;
    }

    g_print("%s\n", equation_str);
}

static void create_btn_layout_and_events (GtkWidget *window, GtkWidget *grid) {
    
    GtkWidget *button;

    button = gtk_button_new_with_label("\%");
    g_signal_connect (button, "clicked", G_CALLBACK (exec_command), '\%');

    
    gtk_grid_attach(GTK_GRID (grid), button, 0, 1, 1, 1);

    button = gtk_button_new_with_label("/");
    g_signal_connect (button, "clicked", G_CALLBACK (exec_command), '/');
    
    gtk_grid_attach(GTK_GRID (grid), button, 1, 1, 1, 1);

    button = gtk_button_new_with_label("x");
    g_signal_connect (button, "clicked", G_CALLBACK (exec_command), 'x');

    gtk_grid_attach(GTK_GRID (grid), button, 2, 1, 1, 1);

    button = gtk_button_new_with_label("-");
    g_signal_connect (button, "clicked", G_CALLBACK (exec_command), '-');

    gtk_grid_attach(GTK_GRID (grid), button, 3, 1, 1, 1);

    button = gtk_button_new_with_label("C");
    g_signal_connect (button, "clicked", G_CALLBACK (exec_command), 'C');

    gtk_grid_attach(GTK_GRID (grid), button, 4, 1, 1, 1);



    button = gtk_button_new_with_label("7");
    g_signal_connect (button, "clicked", G_CALLBACK (exec_command), '7');

    gtk_grid_attach(GTK_GRID (grid), button, 0, 2, 1, 1);

    button = gtk_button_new_with_label("8");
    g_signal_connect (button, "clicked", G_CALLBACK (exec_command), '8');

    gtk_grid_attach(GTK_GRID (grid), button, 1, 2, 1, 1);

    button = gtk_button_new_with_label("9");
    g_signal_connect (button, "clicked", G_CALLBACK (exec_command), '9');

    gtk_grid_attach(GTK_GRID (grid), button, 2, 2, 1, 1);

    button = gtk_button_new_with_label("+");
    g_signal_connect (button, "clicked", G_CALLBACK (exec_command), '+');

    gtk_grid_attach(GTK_GRID (grid), button, 3, 2, 1, 2);



    button = gtk_button_new_with_label("4");
    g_signal_connect (button, "clicked", G_CALLBACK (exec_command), '4');

    gtk_grid_attach(GTK_GRID (grid), button, 0, 3, 1, 1);

    button = gtk_button_new_with_label("5");
    g_signal_connect (button, "clicked", G_CALLBACK (exec_command), '5');

    gtk_grid_attach(GTK_GRID (grid), button, 1, 3, 1, 1);

    button = gtk_button_new_with_label("6");
    g_signal_connect (button, "clicked", G_CALLBACK (exec_command), '6');

    gtk_grid_attach(GTK_GRID (grid), button, 2, 3, 1, 1);



    button = gtk_button_new_with_label("1");
    g_signal_connect (button, "clicked", G_CALLBACK (exec_command), '1');

    gtk_grid_attach(GTK_GRID (grid), button, 0, 4, 1, 1);

    button = gtk_button_new_with_label("2");
    g_signal_connect (button, "clicked", G_CALLBACK (exec_command), '2');

    gtk_grid_attach(GTK_GRID (grid), button, 1, 4, 1, 1);

    button = gtk_button_new_with_label("3");
    g_signal_connect (button, "clicked", G_CALLBACK (exec_command), '3');

    gtk_grid_attach(GTK_GRID (grid), button, 2, 4, 1, 1);

    button = gtk_button_new_with_label("=");
    g_signal_connect (button, "clicked", G_CALLBACK (exec_command), '=');

    gtk_grid_attach(GTK_GRID (grid), button, 3, 4, 1, 2);

    
    button = gtk_button_new_with_label("0");
    g_signal_connect (button, "clicked", G_CALLBACK (exec_command), '0');

    gtk_grid_attach(GTK_GRID (grid), button, 0, 5, 2, 1);

    button = gtk_button_new_with_label(",");
    g_signal_connect (button, "clicked", G_CALLBACK (exec_command), ',');

    gtk_grid_attach(GTK_GRID (grid), button, 2, 5, 1, 1);

}

static void activate (GtkApplication *app, gpointer user_data) {

    GtkWidget *window;
    GtkWidget *grid;



    

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW (window), "Calc");
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);

    grid = gtk_grid_new();

    gtk_container_add(GTK_CONTAINER (window), grid);

    entry = gtk_entry_new();

    // ['12', '+', '25', '*', '30', '/', '15']


    gtk_grid_attach(GTK_GRID (grid), entry, 0, 0, 5, 1);

    create_btn_layout_and_events(window, grid);

    gtk_widget_show_all (window);


}

int main (int argc, char **argv) {
    GtkApplication *app;

    int status;

    //equation.size = 0;

    app = gtk_application_new("org.tdg.calc", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref(app);

    return status;

}