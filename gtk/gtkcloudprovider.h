/* gtkcloudprovider.h
 *
 * Copyright (C) 2015 Carlos Soriano <csoriano@gnome.org>
 *
 * This file is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GTK_CLOUD_PROVIDER_H
#define GTK_CLOUD_PROVIDER_H

#if !defined (__GTK_H_INSIDE__) && !defined (GTK_COMPILATION)
#error "Only <gtk/gtk.h> can be included directly."
#endif

#include <gio/gio.h>
#include <gdk/gdk.h>

G_BEGIN_DECLS

typedef enum {
  GTK_CLOUD_PROVIDER_STATUS_INVALID,
  GTK_CLOUD_PROVIDER_STATUS_IDLE,
  GTK_CLOUD_PROVIDER_STATUS_SYNCING,
  GTK_CLOUD_PROVIDER_STATUS_ERROR
} GtkCloudProviderStatus;

#define GTK_TYPE_CLOUD_PROVIDER             (gtk_cloud_provider_get_type())
#define GTK_CLOUD_PROVIDER(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_CLOUD_PROVIDER, GtkCloudProvider))
#define GTK_CLOUD_PROVIDER_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_CLOUD_PROVIDER, GtkCloudProviderClass))
#define GTK_IS_CLOUD_PROVIDER(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_CLOUD_PROVIDER))
#define GTK_IS_CLOUD_PROVIDER_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_CLOUD_PROVIDER))
#define GTK_CLOUD_PROVIDER_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_CLOUD_PROVIDER, GtkCloudProviderClass))

typedef struct _GtkCloudProvider GtkCloudProvider;
typedef struct _GtkCloudProviderClass GtkCloudProviderClass;


struct _GtkCloudProviderClass
{
  GObjectClass parent_class;
};

struct _GtkCloudProvider
{
  GObject parent_instance;
};

GDK_AVAILABLE_IN_3_18
GType          gtk_cloud_provider_get_type          (void) G_GNUC_CONST;
GDK_AVAILABLE_IN_3_18
GtkCloudProvider *gtk_cloud_provider_new (const gchar *bus_name,
                                          const gchar *object_path);
GDK_AVAILABLE_IN_3_18
gchar* gtk_cloud_provider_get_name (GtkCloudProvider *self);
GDK_AVAILABLE_IN_3_18
GtkCloudProviderStatus gtk_cloud_provider_get_status (GtkCloudProvider *self);
GDK_AVAILABLE_IN_3_18
GIcon *gtk_cloud_provider_get_icon (GtkCloudProvider *self);
GDK_AVAILABLE_IN_3_18
GMenuModel *gtk_cloud_provider_get_menu_model (GtkCloudProvider *self);

G_END_DECLS

#endif /* GTK_CLOUD_PROVIDER_H */
