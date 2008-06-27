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

#include <glib.h>
#include <gtk/gtk.h>
#include "config.h"
#include "pref_dialog.h"
#include "main.h"
#include "support.h"

extern gboolean unrar;
Prefs_dialog_data *xa_create_prefs_dialog()
{
	GtkWidget *vbox1, *vbox4, *hbox1, *scrolledwindow1, *prefs_iconview, *label5;
	GtkWidget *label1, *label2, *label3, *label4, *frame1, *frame2, *frame3, *alignment1, *alignment2, *alignment3;
	GtkWidget *label6, *label7, *label8, *label9, *table1, *table2;
	GtkTreeIter iter;
	GList *archive_type;
	GdkPixbuf *icon_pixbuf;
	Prefs_dialog_data *prefs_data;

	prefs_data = g_new0 (Prefs_dialog_data,1);
	prefs_data->dialog1 = gtk_dialog_new_with_buttons (_("Preferences"),
									GTK_WINDOW (xa_main_window), GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
									GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,GTK_STOCK_OK,GTK_RESPONSE_OK, NULL);
	tooltips = gtk_tooltips_new ();
	gtk_dialog_set_default_response (GTK_DIALOG (prefs_data->dialog1), GTK_RESPONSE_OK);

	vbox1 = GTK_DIALOG (prefs_data->dialog1)->vbox;
	hbox1 = gtk_hbox_new (FALSE, 6);
	gtk_box_pack_start (GTK_BOX (vbox1), hbox1, TRUE, TRUE, 6);

	scrolledwindow1 = gtk_scrolled_window_new (NULL, NULL);
	gtk_box_pack_start (GTK_BOX (hbox1), scrolledwindow1, TRUE, TRUE, 6);
	g_object_set (G_OBJECT (scrolledwindow1),"hscrollbar-policy", GTK_POLICY_NEVER,"shadow-type", GTK_SHADOW_IN,"vscrollbar-policy", GTK_POLICY_NEVER, NULL);

	prefs_data->prefs_liststore = gtk_list_store_new (3,GDK_TYPE_PIXBUF,G_TYPE_STRING,G_TYPE_UINT);
	gtk_list_store_append (prefs_data->prefs_liststore,&iter);
	gchar *pixbuf_file = g_strconcat(DATADIR,"/pixmaps/xarchiver-behaviour.svg",NULL);
	icon_pixbuf = gdk_pixbuf_new_from_file (pixbuf_file,NULL);
	g_free (pixbuf_file);
	gtk_list_store_set (prefs_data->prefs_liststore, &iter, 0, icon_pixbuf, 1, _("Archive"),2,0,-1);
	if(icon_pixbuf != NULL)
		g_object_unref (icon_pixbuf);

	gtk_list_store_append (prefs_data->prefs_liststore, &iter);
	icon_pixbuf = gtk_widget_render_icon (prefs_data->dialog1, "gtk-leave-fullscreen", GTK_ICON_SIZE_DND, NULL);
	gtk_list_store_set (prefs_data->prefs_liststore, &iter, 0, icon_pixbuf, 1, _("Window"),2,1,-1);
	g_object_unref (icon_pixbuf);

	gtk_list_store_append (prefs_data->prefs_liststore, &iter);
	icon_pixbuf = gtk_widget_render_icon (prefs_data->dialog1, "gtk-execute", GTK_ICON_SIZE_DND, NULL);
	gtk_list_store_set (prefs_data->prefs_liststore, &iter, 0, icon_pixbuf, 1, _("Advanced"),2,2,-1);
	g_object_unref (icon_pixbuf);

	prefs_iconview = gtk_icon_view_new_with_model(GTK_TREE_MODEL(prefs_data->prefs_liststore));
	g_object_unref (prefs_data->prefs_liststore);	

	gtk_widget_set_size_request(prefs_iconview, 140, 190);
	gtk_icon_view_set_orientation (GTK_ICON_VIEW (prefs_iconview), GTK_ORIENTATION_VERTICAL);
	gtk_icon_view_set_item_width (GTK_ICON_VIEW (prefs_iconview),130);
	gtk_icon_view_set_columns (GTK_ICON_VIEW (prefs_iconview),1);
	gtk_icon_view_set_pixbuf_column (GTK_ICON_VIEW (prefs_iconview), 0);
	gtk_icon_view_set_text_column(GTK_ICON_VIEW (prefs_iconview), 1);
	gtk_container_add (GTK_CONTAINER (scrolledwindow1), prefs_iconview);
	
	prefs_data->prefs_notebook = gtk_notebook_new ();
	g_object_set (G_OBJECT (prefs_data->prefs_notebook),"show-border", FALSE,"show-tabs", FALSE,"enable-popup",FALSE,NULL);
	gtk_box_pack_start (GTK_BOX (hbox1), prefs_data->prefs_notebook, TRUE, TRUE, 0);
	GTK_WIDGET_UNSET_FLAGS (prefs_data->prefs_notebook, GTK_CAN_FOCUS);
	g_signal_connect (G_OBJECT (prefs_iconview), "selection-changed",G_CALLBACK (xa_prefs_iconview_changed), prefs_data);

	/* Archive page*/
	frame1 = gtk_frame_new (NULL);
	gtk_container_add (GTK_CONTAINER (prefs_data->prefs_notebook), frame1);
	gtk_frame_set_shadow_type (GTK_FRAME (frame1), GTK_SHADOW_NONE);

	alignment1 = gtk_alignment_new (0.5, 0.5, 1, 1);
	gtk_container_add (GTK_CONTAINER (frame1), alignment1);
	gtk_alignment_set_padding (GTK_ALIGNMENT (alignment1), 0, 0, 12, 0);

	vbox4 = gtk_vbox_new (FALSE, 2);
	gtk_container_add (GTK_CONTAINER (alignment1), vbox4);

	hbox1 = gtk_hbox_new (FALSE, 5);
	gtk_box_pack_start (GTK_BOX (vbox4), hbox1, FALSE, TRUE, 0);

	label4 = gtk_label_new (_("Preferred archive format"));
	gtk_box_pack_start (GTK_BOX (hbox1), label4, FALSE, FALSE, 0);

	prefs_data->combo_prefered_format = gtk_combo_box_new_text();
	gtk_box_pack_start (GTK_BOX (hbox1), prefs_data->combo_prefered_format, FALSE, TRUE, 0);
	archive_type = g_list_first (ArchiveType);
	while ( archive_type != NULL )
	{
		if (strcmp(archive_type->data, "tgz") == 0 ||
			strcmp(archive_type->data, "rpm") == 0 ||
			(strcmp(archive_type->data, "rar") == 0 && unrar) )
			goto next;
		else
			gtk_combo_box_append_text (GTK_COMBO_BOX (prefs_data->combo_prefered_format),archive_type->data );
		next:
			archive_type = g_list_next (archive_type);
	}

	prefs_data->allow_dir_extract_with_dnd = gtk_check_button_new_with_mnemonic (_("Allow extracting dirs by drag and drop"));
	gtk_box_pack_start (GTK_BOX (vbox4), prefs_data->allow_dir_extract_with_dnd, FALSE, FALSE, 0);
	gtk_button_set_focus_on_click (GTK_BUTTON (prefs_data->allow_dir_extract_with_dnd), FALSE);

	prefs_data->confirm_deletion = gtk_check_button_new_with_mnemonic (_("Confirm deletion of files"));
	gtk_box_pack_start (GTK_BOX (vbox4), prefs_data->confirm_deletion, FALSE, FALSE, 0);
	gtk_button_set_focus_on_click (GTK_BUTTON (prefs_data->confirm_deletion), FALSE);
	
	prefs_data->store_output = gtk_check_button_new_with_mnemonic (_("Store archiver output"));
	gtk_box_pack_start (GTK_BOX (vbox4), prefs_data->store_output, FALSE, FALSE, 0);
	gtk_button_set_focus_on_click (GTK_BUTTON (prefs_data->store_output), FALSE);
	gtk_tooltips_set_tip(tooltips, prefs_data->store_output, _("It takes a lot of memory with large archives!"), NULL);

	label1 = gtk_label_new ("");
	gtk_notebook_set_tab_label (GTK_NOTEBOOK (prefs_data->prefs_notebook), gtk_notebook_get_nth_page (GTK_NOTEBOOK (prefs_data->prefs_notebook), 0), label1);

	/* Window page*/
	frame2 = gtk_frame_new (NULL);
	gtk_container_add (GTK_CONTAINER (prefs_data->prefs_notebook), frame2);
	gtk_frame_set_shadow_type (GTK_FRAME (frame2), GTK_SHADOW_NONE);

	alignment2 = gtk_alignment_new (0.5, 0.5, 1, 1);
	gtk_container_add (GTK_CONTAINER (frame2), alignment2);
	gtk_alignment_set_padding (GTK_ALIGNMENT (alignment2), 0, 0, 12, 0);

	table1 = gtk_table_new (6, 2,FALSE);
	gtk_container_add (GTK_CONTAINER (alignment2), table1);
	gtk_table_set_row_spacings (GTK_TABLE (table1), 2);
	gtk_table_set_col_spacings (GTK_TABLE (table1), 4);

	label5 = gtk_label_new (_("View archive content as"));
	gtk_table_attach (GTK_TABLE (table1), label5, 0, 1, 0, 1,
                     (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_SHRINK), 0, 0);
	gtk_misc_set_alignment (GTK_MISC (label5), 0, 0.5);
	prefs_data->combo_archive_view = gtk_combo_box_new_text();
	gtk_combo_box_append_text (GTK_COMBO_BOX (prefs_data->combo_archive_view), _("list") );
	gtk_combo_box_append_text (GTK_COMBO_BOX (prefs_data->combo_archive_view), _("icon") );
	gtk_table_attach (GTK_TABLE (table1), prefs_data->combo_archive_view, 1, 2, 0, 1,
                     (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_SHRINK), 0, 0);
	
	label9 = gtk_label_new (_("Size of the mimetype icons"));
	gtk_table_attach (GTK_TABLE (table1), label9, 0, 1, 1, 2,
                     (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_SHRINK), 0, 0);
	gtk_misc_set_alignment (GTK_MISC (label9), 0, 0.5);
	prefs_data->combo_icon_size = gtk_combo_box_new_text();
	gtk_combo_box_append_text (GTK_COMBO_BOX (prefs_data->combo_icon_size), _("large") );
	gtk_combo_box_append_text (GTK_COMBO_BOX (prefs_data->combo_icon_size), _("small") );
	gtk_table_attach (GTK_TABLE (table1), prefs_data->combo_icon_size, 1, 2, 1, 2,
                     (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_SHRINK), 0, 0);
	
	prefs_data->check_show_comment = gtk_check_button_new_with_mnemonic (_("Show archive comment after loading it"));
	gtk_table_attach (GTK_TABLE (table1), prefs_data->check_show_comment, 0, 2, 2, 3,
                     (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_SHRINK), 0, 0);
	gtk_button_set_focus_on_click (GTK_BUTTON (prefs_data->check_show_comment), FALSE);

	prefs_data->check_sort_filename_column = gtk_check_button_new_with_mnemonic (_("Sort content by filename"));
	gtk_table_attach (GTK_TABLE (table1), prefs_data->check_sort_filename_column, 0, 2, 4, 5,
                     (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_SHRINK), 0, 0);
	gtk_button_set_focus_on_click (GTK_BUTTON (prefs_data->check_sort_filename_column), FALSE);
	gtk_tooltips_set_tip(tooltips, prefs_data->check_sort_filename_column, _("The filename column is sort after loading the archive"), NULL);

	prefs_data->show_location_bar = gtk_check_button_new_with_mnemonic (_("Show location bar"));
	gtk_table_attach (GTK_TABLE (table1), prefs_data->show_location_bar, 0, 2, 5, 6,
                     (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_SHRINK), 0, 0);
	gtk_button_set_focus_on_click (GTK_BUTTON (prefs_data->show_location_bar), FALSE);

	label2 = gtk_label_new ("");
	gtk_notebook_set_tab_label (GTK_NOTEBOOK (prefs_data->prefs_notebook), gtk_notebook_get_nth_page (GTK_NOTEBOOK (prefs_data->prefs_notebook), 1), label2);

	/* Advanced page*/
	frame3 = gtk_frame_new (NULL);
	gtk_container_add (GTK_CONTAINER (prefs_data->prefs_notebook), frame3);
	gtk_frame_set_shadow_type (GTK_FRAME (frame3), GTK_SHADOW_NONE);

	alignment3 = gtk_alignment_new (0.5, 0.5, 1, 1);
	gtk_container_add (GTK_CONTAINER (frame3), alignment3);
	gtk_alignment_set_padding (GTK_ALIGNMENT (alignment3), 0, 0, 12, 0);

	table2 = gtk_table_new (5, 2,FALSE);
	gtk_container_add (GTK_CONTAINER (alignment3), table2);
	gtk_table_set_row_spacings (GTK_TABLE (table2), 2);
	gtk_table_set_col_spacings (GTK_TABLE (table2), 4);

	label6 = gtk_label_new (_("Web browser to use:"));
	gtk_table_attach (GTK_TABLE (table2), label6, 0, 1, 0, 1,
                     (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_SHRINK), 0, 0);
	gtk_misc_set_alignment (GTK_MISC (label6), 0, 0.5);
	prefs_data->combo_prefered_web_browser = gtk_combo_box_new_text();
	gtk_combo_box_append_text (GTK_COMBO_BOX (prefs_data->combo_prefered_web_browser), "" );
	gtk_combo_box_append_text (GTK_COMBO_BOX (prefs_data->combo_prefered_web_browser), _("choose...") );
	g_signal_connect (prefs_data->combo_prefered_web_browser,"changed",G_CALLBACK (xa_prefs_combo_changed),NULL);
	gtk_table_attach (GTK_TABLE (table2), prefs_data->combo_prefered_web_browser, 1, 2, 0, 1,
                     (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_SHRINK), 0, 0);

	label7 = gtk_label_new (_("Open text files with:"));
	gtk_table_attach (GTK_TABLE (table2), label7, 0, 1, 1, 2,
                     (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_SHRINK), 0, 0);
	gtk_misc_set_alignment (GTK_MISC (label7), 0, 0.5);
	prefs_data->combo_prefered_editor = gtk_combo_box_new_text();
	gtk_combo_box_append_text (GTK_COMBO_BOX (prefs_data->combo_prefered_editor), "" );
	gtk_combo_box_append_text (GTK_COMBO_BOX (prefs_data->combo_prefered_editor), _("choose...") );
	g_signal_connect (prefs_data->combo_prefered_editor,"changed",G_CALLBACK (xa_prefs_combo_changed),NULL);
	gtk_table_attach (GTK_TABLE (table2), prefs_data->combo_prefered_editor, 1, 2, 1, 2,
                     (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_SHRINK), 0, 0);
                    
	label8 = gtk_label_new (_("Open image files with:"));
	gtk_table_attach (GTK_TABLE (table2), label8, 0, 1, 2, 3,
                     (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_SHRINK), 0, 0);
	gtk_misc_set_alignment (GTK_MISC (label8), 0, 0.5);
	prefs_data->combo_prefered_viewer = gtk_combo_box_new_text();
	gtk_combo_box_append_text (GTK_COMBO_BOX (prefs_data->combo_prefered_viewer), "" );
	gtk_combo_box_append_text (GTK_COMBO_BOX (prefs_data->combo_prefered_viewer), _("choose...") );
	g_signal_connect (prefs_data->combo_prefered_viewer,"changed",G_CALLBACK (xa_prefs_combo_changed),NULL);
	gtk_table_attach (GTK_TABLE (table2), prefs_data->combo_prefered_viewer, 1, 2, 2, 3,
                     (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_SHRINK), 0, 0);

	label9 = gtk_label_new (_("Preferred temp directory:"));
	gtk_table_attach (GTK_TABLE (table2), label9, 0, 1, 3, 4,
                     (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_SHRINK), 0, 0);
	gtk_misc_set_alignment (GTK_MISC (label9), 0, 0.5);
	prefs_data->combo_prefered_temp_dir = gtk_combo_box_new_text();
	gtk_combo_box_append_text (GTK_COMBO_BOX (prefs_data->combo_prefered_temp_dir), _("/tmp") );
	gtk_combo_box_append_text (GTK_COMBO_BOX (prefs_data->combo_prefered_temp_dir), _("choose...") );
	g_signal_connect (prefs_data->combo_prefered_temp_dir,"changed",G_CALLBACK (xa_prefs_combo_changed),(gpointer) 1);
	gtk_table_attach (GTK_TABLE (table2), prefs_data->combo_prefered_temp_dir, 1, 2, 3, 4,
                     (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_SHRINK), 0, 0);

	prefs_data->check_save_geometry = gtk_check_button_new_with_mnemonic (_("Save window geometry"));
	gtk_table_attach (GTK_TABLE (table2), prefs_data->check_save_geometry, 0, 2, 4, 5,
                     (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_SHRINK), 0, 0);
	gtk_button_set_focus_on_click (GTK_BUTTON (prefs_data->check_save_geometry), FALSE);

	label3 = gtk_label_new ("");
	gtk_notebook_set_tab_label (GTK_NOTEBOOK (prefs_data->prefs_notebook), gtk_notebook_get_nth_page (GTK_NOTEBOOK (prefs_data->prefs_notebook), 2), label3);
	return prefs_data;
}

void xa_prefs_iconview_changed (GtkIconView *iconview, gpointer data)
{
	Prefs_dialog_data *prefs = data;
	GList *list;
	GtkTreePath *path;
	GtkTreeIter iter;
	guint column = 0;

	list = gtk_icon_view_get_selected_items (iconview);
	if (list == NULL)
		return;

	list = g_list_first (list);
	path = (GtkTreePath*)list->data;

	gtk_tree_model_get_iter ( GTK_TREE_MODEL(prefs->prefs_liststore), &iter, path );
	gtk_tree_model_get ( GTK_TREE_MODEL(prefs->prefs_liststore), &iter, 2, &column, -1);

	gtk_tree_path_free( (GtkTreePath*)list->data );
	g_list_free (list);

	if (column == 0)
		gtk_notebook_set_current_page (GTK_NOTEBOOK(prefs->prefs_notebook),0);
	else if (column == 1)
		gtk_notebook_set_current_page (GTK_NOTEBOOK(prefs->prefs_notebook),1);
	else if (column == 2)
		gtk_notebook_set_current_page (GTK_NOTEBOOK(prefs->prefs_notebook),2);
}

void xa_prefs_dialog_set_default_options(Prefs_dialog_data *prefs_data)
{
	gtk_combo_box_set_active (GTK_COMBO_BOX(prefs_data->combo_prefered_format),0);
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (prefs_data->confirm_deletion),TRUE);
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (prefs_data->store_output),FALSE);
	

	gtk_combo_box_set_active (GTK_COMBO_BOX(prefs_data->combo_archive_view),0);
	gtk_combo_box_set_active (GTK_COMBO_BOX(prefs_data->combo_icon_size),0);
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (prefs_data->show_location_bar),TRUE);

	gtk_combo_box_set_active (GTK_COMBO_BOX(prefs_data->combo_prefered_web_browser),0);
	gtk_combo_box_set_active (GTK_COMBO_BOX(prefs_data->combo_prefered_editor),0);
	gtk_combo_box_set_active (GTK_COMBO_BOX(prefs_data->combo_prefered_viewer),0);
	gtk_combo_box_set_active (GTK_COMBO_BOX(prefs_data->combo_prefered_temp_dir),0);
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (prefs_data->check_save_geometry),FALSE);
}

void xa_prefs_save_options(Prefs_dialog_data *prefs_data, const char *filename)
{
	gchar *conf;
	gchar *value;
	FILE *fp;
	gint bytes_written, len;
	GKeyFile *xa_key_file = g_key_file_new();
	
	g_key_file_set_integer (xa_key_file,PACKAGE,"preferred_format",gtk_combo_box_get_active (GTK_COMBO_BOX(prefs_data->combo_prefered_format)));
	g_key_file_set_boolean (xa_key_file,PACKAGE,"allow_ext_dir_by_dnd",gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (prefs_data->allow_dir_extract_with_dnd)));
	g_key_file_set_boolean (xa_key_file,PACKAGE,"confirm_deletion",gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (prefs_data->confirm_deletion)));
	g_key_file_set_boolean (xa_key_file,PACKAGE,"store_output",gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (prefs_data->store_output)));

	g_key_file_set_integer (xa_key_file,PACKAGE,"archive_view",gtk_combo_box_get_active (GTK_COMBO_BOX(prefs_data->combo_archive_view)));
	g_key_file_set_integer (xa_key_file,PACKAGE,"icon_size",gtk_combo_box_get_active (GTK_COMBO_BOX(prefs_data->combo_icon_size)));
	g_key_file_set_boolean (xa_key_file,PACKAGE,"show_archive_comment",gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (prefs_data->check_show_comment)));
	g_key_file_set_boolean (xa_key_file,PACKAGE,"sort_filename_content",gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (prefs_data->check_sort_filename_column)));
	g_key_file_set_boolean (xa_key_file,PACKAGE,"show_location_bar",gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (prefs_data->show_location_bar)));
	
	value = gtk_combo_box_get_active_text (GTK_COMBO_BOX(prefs_data->combo_prefered_web_browser));
	if (value != NULL)
	{
		g_key_file_set_string (xa_key_file,PACKAGE,"preferred_web_browser",value);
		g_free (value);
	}
	value = gtk_combo_box_get_active_text (GTK_COMBO_BOX(prefs_data->combo_prefered_editor));
	if (value != NULL)
	{
		g_key_file_set_string (xa_key_file,PACKAGE,"preferred_editor",value);
		g_free(value);
	}
	value = gtk_combo_box_get_active_text (GTK_COMBO_BOX(prefs_data->combo_prefered_viewer));
	if (value != NULL)
	{
		g_key_file_set_string (xa_key_file,PACKAGE,"preferred_viewer",value);
		g_free(value);
	}
	value = gtk_combo_box_get_active_text (GTK_COMBO_BOX(prefs_data->combo_prefered_temp_dir));
	if (value != NULL)
	{
		g_key_file_set_string (xa_key_file,PACKAGE,"preferred_temp_dir",value);
		g_free(value);
	}
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(prefs_data->check_save_geometry)) )
	{
		gtk_window_get_position (GTK_WINDOW(xa_main_window),&prefs_data->geometry[0],&prefs_data->geometry[1]);
		gtk_window_get_size (GTK_WINDOW(xa_main_window),&prefs_data->geometry[2],&prefs_data->geometry[3]);
		g_key_file_set_integer_list(xa_key_file, PACKAGE, "geometry", prefs_data->geometry, 4);
	}
	conf = g_key_file_to_data (xa_key_file, NULL, NULL);
	len = strlen(conf);

	fp = fopen(filename, "w");
	if (fp != NULL)
	{
		bytes_written = fwrite(conf, sizeof (gchar), len, fp);
		fclose(fp);
	}
	g_free (conf);
	g_key_file_free(xa_key_file);
}

void xa_prefs_load_options(Prefs_dialog_data *prefs_data)
{
	gint *coords = NULL;
	gchar *value;
	gsize coords_len = 0;
	gchar *config_dir = NULL;
	gchar *xarchiver_config_dir = NULL;
	GKeyFile *xa_key_file = g_key_file_new();
	GError *error = NULL;

	config_dir = g_strconcat (g_get_home_dir(),"/.config",NULL);
	if (g_file_test(config_dir, G_FILE_TEST_EXISTS) == FALSE)
		g_mkdir_with_parents(config_dir,0600);
					
	xarchiver_config_dir = g_strconcat (config_dir,"/xarchiver",NULL);
	g_free (config_dir);
	if (g_file_test(xarchiver_config_dir, G_FILE_TEST_EXISTS) == FALSE)
		g_mkdir_with_parents(xarchiver_config_dir,0700);

	config_file = g_strconcat (xarchiver_config_dir,"/xarchiverrc",NULL);
	g_free (xarchiver_config_dir);
		
	if ( ! g_key_file_load_from_file(xa_key_file,config_file,G_KEY_FILE_KEEP_COMMENTS,NULL) )
	{
		/* Write the config file with the default options */
		xa_prefs_dialog_set_default_options(prefs_data);
		xa_prefs_save_options(prefs_data,config_file);
	}
	else
	{
		/* set the options from the config file */
		gtk_combo_box_set_active (GTK_COMBO_BOX(prefs_data->combo_prefered_format),g_key_file_get_integer(xa_key_file,PACKAGE,"preferred_format",NULL));
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (prefs_data->allow_dir_extract_with_dnd),g_key_file_get_boolean(xa_key_file,PACKAGE,"allow_ext_dir_by_dnd",NULL));
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (prefs_data->confirm_deletion),g_key_file_get_boolean(xa_key_file,PACKAGE,"confirm_deletion",NULL));
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (prefs_data->store_output),g_key_file_get_boolean(xa_key_file,PACKAGE,"store_output",NULL));

		gtk_combo_box_set_active (GTK_COMBO_BOX(prefs_data->combo_archive_view),g_key_file_get_integer(xa_key_file,PACKAGE,"archive_view",NULL));
		gtk_combo_box_set_active (GTK_COMBO_BOX(prefs_data->combo_icon_size),g_key_file_get_integer(xa_key_file,PACKAGE,"icon_size",NULL));

		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(prefs_data->check_show_comment),g_key_file_get_boolean(xa_key_file,PACKAGE,"show_archive_comment",NULL));
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(prefs_data->check_sort_filename_column),g_key_file_get_boolean(xa_key_file,PACKAGE,"sort_filename_content",NULL));
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(prefs_data->show_location_bar),g_key_file_get_boolean(xa_key_file,PACKAGE,"show_location_bar",NULL));

		value = g_key_file_get_string(xa_key_file,PACKAGE,"preferred_web_browser",NULL);
		if (value != NULL)
		{
			gtk_combo_box_remove_text(GTK_COMBO_BOX (prefs_data->combo_prefered_web_browser),0);
			gtk_combo_box_prepend_text(GTK_COMBO_BOX(prefs_data->combo_prefered_web_browser),value);
			gtk_combo_box_set_active (GTK_COMBO_BOX(prefs_data->combo_prefered_web_browser),0);
			g_free(value);
		}
		value = g_key_file_get_string(xa_key_file,PACKAGE,"preferred_editor",NULL);
		if (value != NULL)
		{
			gtk_combo_box_remove_text(GTK_COMBO_BOX (prefs_data->combo_prefered_editor),0);
			gtk_combo_box_insert_text (GTK_COMBO_BOX(prefs_data->combo_prefered_editor),0,value);
			gtk_combo_box_set_active (GTK_COMBO_BOX(prefs_data->combo_prefered_editor),0);
			g_free(value);
		}
		value = g_key_file_get_string(xa_key_file,PACKAGE,"preferred_viewer",NULL);
		if (value != NULL)
		{
			gtk_combo_box_remove_text(GTK_COMBO_BOX (prefs_data->combo_prefered_viewer),0);
			gtk_combo_box_insert_text (GTK_COMBO_BOX(prefs_data->combo_prefered_viewer),0,value);
			gtk_combo_box_set_active (GTK_COMBO_BOX(prefs_data->combo_prefered_viewer),0);
			g_free(value);
		}
		value = g_key_file_get_string(xa_key_file,PACKAGE,"preferred_temp_dir",NULL);
		if (value != NULL)
		{
			gtk_combo_box_remove_text(GTK_COMBO_BOX (prefs_data->combo_prefered_temp_dir),0);
			gtk_combo_box_insert_text (GTK_COMBO_BOX(prefs_data->combo_prefered_temp_dir),0,value);
			gtk_combo_box_set_active (GTK_COMBO_BOX(prefs_data->combo_prefered_temp_dir),0);
			g_free(value);
		}
		gtk_combo_box_set_active (GTK_COMBO_BOX(prefs_data->combo_prefered_temp_dir),0);
		coords = g_key_file_get_integer_list(xa_key_file, PACKAGE, "geometry", &coords_len, &error);
		if (error)
		{
			prefs_data->geometry[0] = -1;
			g_error_free(error);
			error = NULL;
		}
		else
		{	
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (prefs_data->check_save_geometry),TRUE);	
			prefs_data->geometry[0] = coords[0];
			prefs_data->geometry[1] = coords[1];
			prefs_data->geometry[2] = coords[2];
			prefs_data->geometry[3] = coords[3];
		}
	}
	g_key_file_free (xa_key_file);
	/* config_file is freed in window.c xa_quit_application */
}

void xa_prefs_combo_changed (GtkComboBox *widget,gpointer user_data)
{
	gchar *filename = NULL;
	unsigned short int flag = GPOINTER_TO_UINT(user_data);

	if (gtk_combo_box_get_active(GTK_COMBO_BOX (widget)) == 1)
	{
		filename = xa_prefs_choose_program(flag);
		if (filename != NULL)
		{
			gtk_combo_box_remove_text(GTK_COMBO_BOX (widget),0);
			gtk_combo_box_insert_text(GTK_COMBO_BOX (widget),0,filename);
			g_free(filename);
		}
		gtk_combo_box_set_active (GTK_COMBO_BOX (widget),0);
	}
}

gchar *xa_prefs_choose_program(gboolean flag)
{
	gchar *filename = NULL;
	GtkWidget *dialog;

	dialog = gtk_file_chooser_dialog_new (flag ? _("Choose the temp directory to use") : _("Choose the application to use"),
				      GTK_WINDOW(xa_main_window),
				      flag ? GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER : GTK_FILE_CHOOSER_ACTION_OPEN,
				      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
				      GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
				      NULL);

	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
		filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

	gtk_widget_destroy (dialog);
	
	return filename;
}

void xa_apply_prefs_option(Prefs_dialog_data *prefs_data)
{
	gint i,idx;

	if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(prefs_data->show_location_bar)))
		gtk_widget_show_all (toolbar2);
	else
		gtk_widget_hide (toolbar2);

	for (i = 0; i < gtk_notebook_get_n_pages(notebook) ; i++)
	{
		idx = xa_find_archive_index (i);
		if (archive[idx] != NULL)
			g_object_set(G_OBJECT(archive[idx]->renderer), "stock-size", (3 - gtk_combo_box_get_active(GTK_COMBO_BOX(prefs_data->combo_icon_size))), NULL);
		gtk_widget_queue_draw(GTK_WIDGET(archive[idx]->treeview));
	}
	
}
