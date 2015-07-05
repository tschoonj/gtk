#include "config.h"
#include <glib.h>
#include <gtk/gtk.h>

static void
print_gmenu_model (GMenuModel  *model)
{
  gint i, n_items;
  GMenuModel *submodel = NULL;
  gchar *label;

  n_items = g_menu_model_get_n_items (model);
  g_print ("n items %d\n", n_items);

  for (i = 0; i < n_items; i++)
    {
      label = NULL;
      if (g_menu_model_get_item_attribute (model, i, G_MENU_ATTRIBUTE_LABEL, "s", &label))
        {
          g_print ("Menu item - %s\n", label);
          if (label != NULL)
            g_free (label);
        }

      submodel = g_menu_model_get_item_link (model, i, G_MENU_LINK_SECTION);
      if (!submodel)
       submodel = g_menu_model_get_item_link (model, i, G_MENU_LINK_SUBMENU);

      if (!submodel)
        {
          g_print ("no submodel\n");
          continue;
        }

      print_gmenu_model (submodel);
      g_clear_object (&submodel);
  }
}

static void
on_manager_changed (GtkCloudProviderManager *manager)
{
  GList *providers;
  GList *l;
  gint provider_status;
  gchar *status_string;
  GIcon *icon;
  gchar *icon_representation;
  GMenuModel *menu_model;

  providers = gtk_cloud_provider_manager_get_providers (manager);
  g_print ("Providers data\n");
  g_print ("--------------\n");
  for (l = providers; l != NULL; l = l->next)
    {
      provider_status = gtk_cloud_provider_get_status (GTK_CLOUD_PROVIDER (l->data));
      switch (provider_status)
        {
        case GTK_CLOUD_PROVIDER_STATUS_INVALID:
          status_string = "invalid";
          break;

        case GTK_CLOUD_PROVIDER_STATUS_IDLE:
          status_string = "idle";
          break;

        case GTK_CLOUD_PROVIDER_STATUS_SYNCING:
          status_string = "syncing";
          break;

        case GTK_CLOUD_PROVIDER_STATUS_ERROR:
          status_string = "error";
          break;

        default:
          g_assert_not_reached ();
        }

      icon = gtk_cloud_provider_get_icon (l->data);
      icon_representation = g_icon_to_string (icon);

      g_print ("Name - %s Status - %s Icon - %s\n",
               gtk_cloud_provider_get_name (GTK_CLOUD_PROVIDER (l->data)),
               status_string,
               icon_representation);

      g_free (icon_representation);

      menu_model = gtk_cloud_provider_get_menu_model (l->data);
      print_gmenu_model (menu_model);
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
  g_main_loop_run (loop);

  return 0;
}
