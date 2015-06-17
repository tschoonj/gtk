#include "config.h"
#include <glib.h>
#include <gtk/gtk.h>

enum {
  IDLE,
  SYNCING,
  ERROR
};

static void
on_manager_changed (GtkCloudProviderManager *manager)
{
  GList *providers;
  GList *l;
  gint provider_status;
  gchar *status_string;

  providers = gtk_cloud_provider_manager_get_providers (manager);
  g_print ("Providers data\n");
  g_print ("--------------\n");
  for (l = providers; l != NULL; l = l->next)
    {
      provider_status = gtk_cloud_provider_get_status (GTK_CLOUD_PROVIDER (l->data));
      switch (provider_status)
        {
        case IDLE:
          status_string = "idle";
          break;

        case SYNCING:
          status_string = "syncing";
          break;

        case ERROR:
          status_string = "error";
          break;

        default:
          g_assert_not_reached ();
        }

      g_print ("Name - %s Status - %s\n",
               gtk_cloud_provider_get_name (GTK_CLOUD_PROVIDER (l->data)),
               status_string);
    }
  g_print ("\n");
}

gint
main (gint   argc,
      gchar *argv[])
{
  GtkCloudProviderManager *manager;
  GMainLoop *loop;

  manager = gtk_cloud_provider_manager_dup_singleton ();
  g_signal_connect (manager, "changed", G_CALLBACK (on_manager_changed), NULL);
  //gtk_cloud_provider_manager_update (manager);

  loop = g_main_loop_new (NULL, FALSE);
  g_print ("before loop\n");
  g_main_loop_run (loop);
  g_print ("after\n");

  return 0;
}
