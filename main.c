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

    switch (sign)
    {
    case '=': return nb1;
    case '/': return nb1 / nb2;
    case 'x': return nb1 * nb2;
    case '-': return nb1 - nb2;
    case '+': return nb1 + nb2;
    default : return 0;
    }
}

static float get_result() {

    char last_sign;
    float result = 0;
    float current = 0;

    for (int i = 0; i < equation_str_size - 1; i++) {

       
        if (isdigit(equation_str[i])) {
            current = (current + (pow(10, get_exponent(equation_str, i)) * (equation_str[i] - '0')));
        }
        else {

            char sign = equation_str[i];

            if (result == 0) {
                result = current;
            }
            else {
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
        return;
    }

}

typedef struct {
    char sign;
    int left;
    int top;
    int width;
    int height;

} Calc_Btn_Layout;

static void create_btn_layout_and_events (GtkWidget *window, GtkWidget *grid) {
    
    GtkWidget *button;

    Calc_Btn_Layout btn_layout[] = {
        {'\%', 0,1,1,1}, {'/', 1,1,1,1}, {'x', 2,1,1,1}, {'-', 3,1,1,1},
        {'7' , 0,2,1,1}, {'8', 1,2,1,1}, {'9', 2,2,1,1}, {'+', 3,2,1,2},
        {'4' , 0,3,1,1}, {'5', 1,3,1,1}, {'6', 2,3,1,1}, {'1', 0,4,1,1},
        {'2' , 1,4,1,1}, {'3', 2,4,1,1}, {'=', 3,4,1,2}, {'0', 0,5,2,1},
        {'C' , 2,5,1,1}
    };

    for (int i = 0; i < 17; i++) {
        
        Calc_Btn_Layout current_layout = btn_layout[i];

        g_print("%c", current_layout.sign);

        gchar s_sign[2];
        s_sign[0] = current_layout.sign;
        s_sign[1] = '\0';

        button = gtk_button_new_with_label(s_sign);

        g_signal_connect (button, "clicked", G_CALLBACK (exec_command), current_layout.sign);

        gtk_grid_attach(GTK_GRID (grid), button, current_layout.left, current_layout.top, current_layout.width, current_layout.height);
    }
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

    gtk_grid_attach(GTK_GRID (grid), entry, 0, 0, 5, 1);

    create_btn_layout_and_events(window, grid);

    gtk_widget_show_all (window);

}

int main (int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.tdg.calc", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref(app);

    return status;
}