#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <ctype.h>
#include <math.h>

typedef struct {
    char * equation_str;
    int size;
} Equation;

typedef struct  {
    Equation * eq;
    char sign;
    GtkWidget * entry;
} Command_Args;

static int get_exponent(char * array, int position) {

    int result = -1;

    while (isdigit(array[position])) {

        position++;
        result++;
    }

    return result;
}

static void accumulate_result(Command_Args * args) {

    if (args->eq->size == 1 && !isdigit(args->sign)) {
        return;
    }


    if (args->eq->size != 1 && !isdigit(args->sign) && !isdigit(args->eq->equation_str[args->eq->size - 2])) {
        
        args->eq->equation_str[args->eq->size - 2] = args->sign;
        return;
    }

    args->eq->size++;

    if (args->eq->size == 0) {
        args->eq->equation_str = malloc(sizeof(char) * args->eq->size);
        args->eq->equation_str[1] = '\0';
    }
    else {
        args->eq->equation_str = realloc(args->eq->equation_str, sizeof(char) * args->eq->size);
    }

    args->eq->equation_str[args->eq->size - 2] = args->sign;
    args->eq->equation_str[args->eq->size - 1] = '\0';

}

static void reset_equation(Equation * eq) {

        eq->equation_str = realloc(eq->equation_str, sizeof(char));
        eq->size = 1;
        eq->equation_str[0] = '\0';
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

static float get_result(Equation * eq) {

    char last_sign;
    float result = 0;
    float current = 0;

    for (int i = 0; i < eq->size - 1; i++) {

       
        if (isdigit(eq->equation_str[i])) {
            current = (current + (pow(10, get_exponent(eq->equation_str, i)) * (eq->equation_str[i] - '0')));
        }
        else {

            char sign = eq->equation_str[i];

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

    Command_Args * command = (Command_Args*)data;

    if (command->sign == 'C') {
        reset_equation(command->eq);
        gtk_entry_set_text(command->entry, command->eq->equation_str);
        return;
    }

    accumulate_result(command);

    gtk_entry_set_text(command->entry, command->eq->equation_str);

    if (command->sign == '=') {
        
        float result = get_result(command->eq); 

        char c_result[100]; 
        sprintf(c_result, "%f", result);

        gtk_entry_set_text(command->entry, c_result);
        
        reset_equation(command->eq);

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

static void create_btn_layout_and_events (GtkWidget *window, GtkWidget *grid, Equation * eq) {
    
    GtkWidget *button;
    GtkWidget *entry;

    entry = gtk_entry_new();

    gtk_grid_attach(GTK_GRID (grid), entry, 0, 0, 5, 1);

    Calc_Btn_Layout btn_layout[] = {
        {'\%', 0,1,1,1}, {'/', 1,1,1,1}, {'x', 2,1,1,1}, {'-', 3,1,1,1},
        {'7' , 0,2,1,1}, {'8', 1,2,1,1}, {'9', 2,2,1,1}, {'+', 3,2,1,2},
        {'4' , 0,3,1,1}, {'5', 1,3,1,1}, {'6', 2,3,1,1}, {'1', 0,4,1,1},
        {'2' , 1,4,1,1}, {'3', 2,4,1,1}, {'=', 3,4,1,2}, {'0', 0,5,2,1},
        {'C' , 2,5,1,1}
    };

    for (int i = 0; i < 17; i++) {
        
        
        Calc_Btn_Layout current_layout = btn_layout[i];

        gchar s_sign[2];
        s_sign[0] = current_layout.sign;
        s_sign[1] = '\0';

        button = gtk_button_new_with_label(s_sign);
        
        // TODO: Fix memory leak
        Command_Args * args = (Command_Args*)malloc(sizeof(Command_Args));
        args->eq = eq;
        args->sign = current_layout.sign;
        args->entry = entry;
        g_signal_connect (button, "clicked", G_CALLBACK (exec_command), args);

        gtk_grid_attach(GTK_GRID (grid), button, current_layout.left, current_layout.top, current_layout.width, current_layout.height);
    }
}

static void activate (GtkApplication *app, gpointer user_data) {


    Equation * eq = (Equation*)user_data;

    GtkWidget *window;
    GtkWidget *grid;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW (window), "Calc");
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);

    grid = gtk_grid_new();

    gtk_container_add(GTK_CONTAINER (window), grid);

    create_btn_layout_and_events(window, grid, eq);

    gtk_widget_show_all (window);

}

int main (int argc, char **argv) {
    GtkApplication *app;

    Equation * eq = (Equation*)malloc(sizeof(Equation));
    eq->equation_str = (char*)malloc(sizeof(char) * 2);
    eq->equation_str[1] = '\0';
    eq->size = 1;
    
    int status;

    app = gtk_application_new("org.tdg.calc", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK (activate), eq);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref(app);

    free(eq->equation_str);
    free(eq);

    return status;
}