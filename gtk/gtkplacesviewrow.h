/* gtkplacesviewrow.h
 *
 * Copyright (C) 2015 Georges Basile Stavracas Neto <georges.stavracas@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GTK_PLACES_VIEW_ROW_H
#define GTK_PLACES_VIEW_ROW_H

#if !defined (__GTK_H_INSIDE__) && !defined (GTK_COMPILATION)
#error "Only <gtk/gtk.h> can be included directly."
#endif

#include <gtk/gtkwidget.h>

G_BEGIN_DECLS

#define GTK_TYPE_PLACES_VIEW_ROW (gtk_places_view_row_get_type())

G_DECLARE_FINAL_TYPE (GtkPlacesViewRow, gtk_places_view_row, GTK, PLACES_VIEW_ROW, GtkListBoxRow)

GtkWidget*         gtk_places_view_row_new                       (GVolume            *volume,
                                                                  GMount             *mount);

GtkWidget*         gtk_places_view_row_get_event_box             (GtkPlacesViewRow   *row);

GMount*            gtk_places_view_row_get_mount                 (GtkPlacesViewRow   *row);

GVolume*           gtk_places_view_row_get_volume                (GtkPlacesViewRow   *row);

G_END_DECLS

#endif /* GTK_PLACES_VIEW_ROW_H */
