//Compile:  cc `pkg-config --cflags gtk4,webkitgtk-6.0` main.c -o webkitgtkcallback `pkg-config --libs gtk4,webkitgtk-6.0`.

#include <gtk/gtk.h>
#include "webkit/webkit.h"

static void
script_message(GtkWidget* widget, gpointer data)
{
    g_print("clickedd\n");
}

static void
activate (GtkApplication *app,
          gpointer        user_data)
{
    GtkWidget *window;
    GtkWidget *web_view;
    WebKitUserContentManager *content_manager;

    window = gtk_application_window_new (app);
    web_view = webkit_web_view_new();
    content_manager = webkit_web_view_get_user_content_manager(WEBKIT_WEB_VIEW (web_view));
    
    g_signal_connect(content_manager, "script-message-received::handlerid", G_CALLBACK(script_message), NULL);
    webkit_user_content_manager_register_script_message_handler(content_manager, "handlerid", NULL);
    webkit_web_view_load_html(WEBKIT_WEB_VIEW(web_view), "<!DOCTYPE html><html><body><input onClick=\"alert(window.webkit.messageHandlers.handlerid.postMessage('test'));\" id=\"inputId\" type=\"button\" value=\"Click\" /></body></html>", NULL);

    gtk_window_set_child(GTK_WINDOW(window), web_view);
    gtk_window_present (GTK_WINDOW(window));
}

int
main (int    argc,
      char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}