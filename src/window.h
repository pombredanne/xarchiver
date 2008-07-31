/*
 *  Copyright (C) 2008 Giuseppe Torelli - <colossus73@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Street #330, Boston, MA 02111-1307, USA.
 */

#ifndef __XARCHIVER_WINDOW_H
#define __XARCHIVER_WINDOW_H

#ifndef _GNU_SOURCE
# define _GNU_SOURCE 1
#endif

#include <string.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <glib/gstdio.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include "arj.h"
#include "deb.h"
#include "lha.h"
#include "zip.h"
#include "rar.h"
#include "tar.h"
#include "7zip.h"
#include "bzip2.h"
#include "rpm.h"
#include "gzip.h"
#include "archive.h"
#include "new_dialog.h"
#include "extract_dialog.h"
#include "add_dialog.h"
#include "pref_dialog.h"

unsigned long long int file_offset;
Extract_dialog_data *extract_window;
Add_dialog_data *add_window;
GtkWidget *dialog, *scrollwin, *view_window, *_properties_win,*comment_dialog;

void xa_new_archive (GtkMenuItem *, gpointer);
void xa_save_archive (GtkMenuItem *,gpointer);
void xa_open_archive (GtkMenuItem *, gpointer);
void xa_test_archive (GtkMenuItem *, gpointer);
void xa_list_archive (GtkMenuItem *, gpointer);
void xa_print_entry_in_file(XEntry *, FILE *,int);
void xa_close_archive (GtkMenuItem *, gpointer);
void xa_quit_application (GtkMenuItem *, gpointer);
void xa_delete_archive (GtkMenuItem *, gpointer);
void xa_determine_program_to_run(gchar *file);
gboolean xa_launch_external_program(gchar *program,gchar *arg);
void xa_show_help (GtkMenuItem *, gpointer);
void xa_enter_password (GtkMenuItem *, gpointer);
void xa_about (GtkMenuItem *, gpointer);
void on_options1_activate (GtkMenuItem *, gpointer);
void xa_extract_archive ( GtkMenuItem *, gpointer);
void xa_convert_sfx ( GtkMenuItem *, gpointer);
void xa_concat_filenames (GtkTreeModel *, GtkTreePath *, GtkTreeIter *, GSList **);
void xa_select_all ( GtkMenuItem *, gpointer);
void xa_deselect_all ( GtkMenuItem *, gpointer);
void xa_show_archive_comment ( GtkMenuItem *, gpointer);
void xa_archive_properties ( GtkMenuItem * , gpointer);
void xa_view_file_inside_archive ( GtkMenuItem * , gpointer);
void xa_cancel_archive ( GtkMenuItem * , gpointer);
void xa_add_files_archive ( GtkMenuItem *, gpointer);
void xa_show_prefs_dialog ( GtkMenuItem * , gpointer);
gchar *xa_set_size_string (unsigned long long int file_size);
void xa_set_statusbar_message_for_selected_rows (GtkTreeSelection *,gpointer );
void xa_set_statusbar_message_for_displayed_rows(XArchive *);
void on_drag_data_received (GtkWidget *,GdkDragContext *, int x,int y,GtkSelectionData *, unsigned int , unsigned int , gpointer );
void drag_begin (GtkWidget *,GdkDragContext *, gpointer );
void drag_end (GtkWidget *, GdkDragContext *, gpointer );
void drag_data_get (GtkWidget *, GdkDragContext *dc, GtkSelectionData *, guint , guint t, gpointer );

int xa_show_message_dialog ( GtkWindow *, int,int ,int , const gchar *,const gchar *);
int xa_detect_archive_type ( gchar * );
gboolean xa_detect_archive_comment ( int type, gchar *, XArchive * );
gboolean key_press_function ( GtkWidget* , GdkEventKey* ,gpointer );
gboolean treeview_select_search (GtkTreeModel *,gint ,const gchar *,GtkTreeIter *,gpointer );
gboolean xa_check_child_for_error_on_exit(XArchive *,gint );
void xa_archive_operation_finished(XArchive *);
void xa_reload_archive_content(XArchive *);
void xa_watch_child ( GPid , gint , gpointer );
void xa_remove_columns();
void xa_create_liststore (XArchive *,gchar *[]);
gchar *xa_get_statusbar_message(unsigned long int,gint,gint,gboolean);
gchar *xa_open_file_dialog ();
gchar *xa_open_sfx_file_selector ();
void xa_activate_link (GtkAboutDialog *,const gchar *,gpointer );
void xa_comment_window_insert_in_archive(GtkButton *,gpointer );
void xa_load_comment_window_from_file(GtkButton *,gpointer );
void xa_clear_comment_window(GtkButton *,gpointer );
void xa_destroy_comment_window(GtkButton *,gpointer);
void xa_location_entry_activated (GtkEntry *,gpointer );
int xa_mouse_button_event(GtkWidget *,GdkEventButton *,gpointer );
void xa_treeview_row_activated(GtkTreeView *,GtkTreePath *,GtkTreeViewColumn *,gpointer );
void xa_update_window_with_archive_entries(XArchive *,XEntry *);
#endif

