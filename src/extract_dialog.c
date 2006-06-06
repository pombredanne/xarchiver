/*
 *  Copyright (C) 2006 Giuseppe Torelli - <colossus73@gmail.com>
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

#include <gtk/gtk.h>
#include "extract_dialog.h"
#include "interface.h"
#include "callbacks.h"
#include "support.h"

Extract_dialog_data *create_extract_dialog (gint selected , unsigned short int archive_type)
{
	Extract_dialog_data *dialog_data;

	dialog_data = g_new0 (Extract_dialog_data, 1);
	dialog_data->radio_group = NULL;
	dialog_data->dialog1 = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (dialog_data->dialog1), _("Extract Dialog"));
	gtk_window_set_type_hint (GTK_WINDOW (dialog_data->dialog1), GDK_WINDOW_TYPE_HINT_DIALOG);
	gtk_window_set_transient_for ( GTK_WINDOW (dialog_data->dialog1) , GTK_WINDOW (MainWindow) );

	dialog_data->option_tooltip = gtk_tooltips_new ();
	dialog_data->dialog_vbox1 = GTK_DIALOG (dialog_data->dialog1)->vbox;
	gtk_widget_show (dialog_data->dialog_vbox1);

	dialog_data->vbox1 = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (dialog_data->vbox1);
	gtk_box_pack_start (GTK_BOX (dialog_data->dialog_vbox1), dialog_data->vbox1, TRUE, TRUE, 0);

	dialog_data->hbox3 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (dialog_data->hbox3);
	gtk_box_pack_start (GTK_BOX (dialog_data->vbox1), dialog_data->hbox3, FALSE, TRUE, 17);

	dialog_data->extract_to_label = gtk_label_new (_("Extract to:"));
	gtk_widget_show (dialog_data->extract_to_label);
	gtk_box_pack_start (GTK_BOX (dialog_data->hbox3), dialog_data->extract_to_label, FALSE, TRUE, 0);

	dialog_data->destination_path_entry = gtk_entry_new ();
	gtk_widget_show (dialog_data->destination_path_entry);
	gtk_box_pack_start (GTK_BOX (dialog_data->hbox3), dialog_data->destination_path_entry, TRUE, TRUE, 0);
	gtk_widget_set_size_request (dialog_data->destination_path_entry, 385, -1);
	gtk_entry_set_activates_default (GTK_ENTRY (dialog_data->destination_path_entry), TRUE);

	dialog_data->button1 = gtk_button_new ();
	gtk_widget_set_size_request (dialog_data->button1, 33, 27);
	gtk_widget_show (dialog_data->button1);

	dialog_data->image1 = gtk_image_new_from_stock ("gtk-open", GTK_ICON_SIZE_BUTTON);
	gtk_widget_show (dialog_data->image1);
	gtk_box_pack_end(GTK_BOX (dialog_data->hbox3), dialog_data->button1, FALSE, TRUE, 0);
	gtk_widget_set_size_request (dialog_data->image1, 30, 30);

	gtk_container_add(GTK_CONTAINER(dialog_data->button1), dialog_data->image1);
	gtk_tooltips_set_tip (dialog_data->option_tooltip,dialog_data->button1 , _("Choose a folder where to extract files"), NULL );
	g_signal_connect ( (gpointer) dialog_data->button1, "clicked", G_CALLBACK (Show_File_Dialog) ,  "extract" );

	dialog_data->hbox4 = gtk_hbox_new (TRUE, 7);
	gtk_widget_show (dialog_data->hbox4);
	gtk_box_pack_start (GTK_BOX (dialog_data->vbox1), dialog_data->hbox4, FALSE, FALSE, 0);

	dialog_data->frame1 = gtk_frame_new (NULL);
	gtk_widget_show (dialog_data->frame1);
	gtk_box_pack_start (GTK_BOX (dialog_data->hbox4), dialog_data->frame1, TRUE, TRUE, 0);
	gtk_widget_set_size_request (dialog_data->frame1, 1, -1);
	gtk_frame_set_shadow_type (GTK_FRAME (dialog_data->frame1), GTK_SHADOW_OUT);

	dialog_data->alignment1 = gtk_alignment_new (0.5, 0.5, 1, 1);
	gtk_widget_show (dialog_data->alignment1);
	gtk_container_add (GTK_CONTAINER (dialog_data->frame1), dialog_data->alignment1);
	gtk_alignment_set_padding (GTK_ALIGNMENT (dialog_data->alignment1), 0, 0, 12, 0);

	dialog_data->vbox3 = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (dialog_data->vbox3);
	gtk_container_add (GTK_CONTAINER (dialog_data->alignment1), dialog_data->vbox3);

	dialog_data->all_files_radio = gtk_radio_button_new_with_mnemonic (NULL, _("All"));
	gtk_widget_show (dialog_data->all_files_radio);
	gtk_box_pack_start (GTK_BOX (dialog_data->vbox3), dialog_data->all_files_radio, FALSE, FALSE, 0);
	gtk_radio_button_set_group (GTK_RADIO_BUTTON (dialog_data->all_files_radio), dialog_data->radio_group);
	dialog_data->radio_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (dialog_data->all_files_radio));

	dialog_data->selected_files_radio = gtk_radio_button_new_with_mnemonic (NULL, _("Only selected"));
	gtk_widget_show (dialog_data->selected_files_radio);
	gtk_box_pack_start (GTK_BOX (dialog_data->vbox3), dialog_data->selected_files_radio, FALSE, FALSE, 0);
	gtk_radio_button_set_group (GTK_RADIO_BUTTON (dialog_data->selected_files_radio), dialog_data->radio_group);
	dialog_data->radio_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (dialog_data->selected_files_radio));

	if (selected)
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (dialog_data->selected_files_radio), TRUE);
	else
		gtk_widget_set_sensitive (dialog_data->selected_files_radio , FALSE);
	
	dialog_data->files_frame_label = gtk_label_new (_("<b>Files to extract </b>"));
	gtk_widget_show (dialog_data->files_frame_label);
	gtk_frame_set_label_widget (GTK_FRAME (dialog_data->frame1), dialog_data->files_frame_label);
	gtk_label_set_use_markup (GTK_LABEL (dialog_data->files_frame_label), TRUE);

	dialog_data->frame2 = gtk_frame_new (NULL);
	gtk_widget_show (dialog_data->frame2);
	gtk_box_pack_start (GTK_BOX (dialog_data->hbox4), dialog_data->frame2, TRUE, TRUE, 0);
	gtk_frame_set_shadow_type (GTK_FRAME (dialog_data->frame2), GTK_SHADOW_OUT);

	dialog_data->alignment2 = gtk_alignment_new (0.5, 0.5, 1, 1);
	gtk_widget_show (dialog_data->alignment2);
	gtk_container_add (GTK_CONTAINER (dialog_data->frame2), dialog_data->alignment2);
	gtk_alignment_set_padding (GTK_ALIGNMENT (dialog_data->alignment2), 0, 0, 12, 0);

	dialog_data->vbox4 = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (dialog_data->vbox4);
	gtk_container_add (GTK_CONTAINER (dialog_data->alignment2), dialog_data->vbox4);

	dialog_data->overwrite_check = gtk_check_button_new_with_mnemonic (_("Overwrite existing files"));
	gtk_widget_show (dialog_data->overwrite_check);
	gtk_box_pack_start (GTK_BOX (dialog_data->vbox4), dialog_data->overwrite_check, FALSE, FALSE, 0);

	/* RAR, ZIP, ARJ, 7ZIP */
	if (archive_type == 3 || archive_type == 4 || archive_type == 5 || archive_type == 10) 
	{
		dialog_data->extract_full = gtk_check_button_new_with_mnemonic (_("Extract files with full path"));
		gtk_widget_show (dialog_data->extract_full);
		gtk_tooltips_set_tip (dialog_data->option_tooltip,dialog_data->extract_full , _("The archive's directory structure is recreated in the extraction directory"), NULL );
		gtk_box_pack_start (GTK_BOX (dialog_data->vbox4), dialog_data->extract_full, FALSE, FALSE, 0);
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (dialog_data->extract_full), TRUE);
	}
	
	/* TAR, TAR_GZ, TAR_BZ2 */
	else if (archive_type == 6 || archive_type == 7 || archive_type == 8)
	{
		dialog_data->preserve_permissions = gtk_check_button_new_with_mnemonic (_("Preserve permissions"));
		gtk_widget_show (dialog_data->preserve_permissions);
		gtk_box_pack_start (GTK_BOX (dialog_data->vbox4), dialog_data->preserve_permissions, FALSE, FALSE, 0);

		dialog_data->preserve_ownership = gtk_check_button_new_with_mnemonic (_("Preserve ownership"));
		gtk_widget_show (dialog_data->preserve_ownership);
		gtk_box_pack_start (GTK_BOX (dialog_data->vbox4), dialog_data->preserve_ownership, FALSE, FALSE, 0);
	}

	if (archive_type == 3 || archive_type == 4 || archive_type == 5)
	{
		dialog_data->fresh = gtk_check_button_new_with_mnemonic (_("Freshen existing files"));
		gtk_tooltips_set_tip (dialog_data->option_tooltip,dialog_data->fresh , _("Extract only those files that already exist on disk and that are newer than the disk copies."), NULL );
		gtk_widget_show (dialog_data->fresh);
		gtk_box_pack_start (GTK_BOX (dialog_data->vbox4), dialog_data->fresh, FALSE, FALSE, 0);
		g_signal_connect (G_OBJECT (dialog_data->fresh),"toggled",G_CALLBACK (fresh_update_toggled_cb) , dialog_data);
	}
	if (archive_type == 3 || archive_type == 4 || archive_type == 5 || archive_type == 10)
	{
		dialog_data->update = gtk_check_button_new_with_mnemonic (_("Update existing files"));
		gtk_tooltips_set_tip (dialog_data->option_tooltip,dialog_data->update , _("This option performs the same function as the freshen one, extracting files that are newer than those with the same name on disk, and in addition it extracts those files that do not already exist on disk."), NULL );
		gtk_widget_show (dialog_data->update);
		gtk_box_pack_start (GTK_BOX (dialog_data->vbox4), dialog_data->update, FALSE, FALSE, 0);
		g_signal_connect (G_OBJECT (dialog_data->update),"toggled",G_CALLBACK (update_fresh_toggled_cb) , dialog_data);
	}

	dialog_data->hbox5 = gtk_hbox_new (FALSE, 2);
	gtk_widget_show (dialog_data->hbox5);
	gtk_box_pack_start (GTK_BOX (dialog_data->vbox4), dialog_data->hbox5, FALSE, FALSE, 0);

	dialog_data->label_password = gtk_label_new (_("Password:"));
	gtk_widget_show (dialog_data->label_password);
	gtk_box_pack_start (GTK_BOX (dialog_data->hbox5), dialog_data->label_password, FALSE, FALSE, 0);

	dialog_data->password_entry = gtk_entry_new ();
	gtk_widget_show (dialog_data->password_entry);
	gtk_box_pack_start (GTK_BOX (dialog_data->hbox5), dialog_data->password_entry, FALSE, FALSE, 0);
	gtk_entry_set_visibility (GTK_ENTRY (dialog_data->password_entry), FALSE);

	dialog_data->options_frame_label = gtk_label_new (_("<b>Options </b>"));
	gtk_widget_show (dialog_data->options_frame_label);
	gtk_frame_set_label_widget (GTK_FRAME (dialog_data->frame2), dialog_data->options_frame_label);
	gtk_label_set_use_markup (GTK_LABEL (dialog_data->options_frame_label), TRUE);

	dialog_data->dialog_action_area1 = GTK_DIALOG (dialog_data->dialog1)->action_area;
	gtk_widget_show (dialog_data->dialog_action_area1);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_data->dialog_action_area1), GTK_BUTTONBOX_END);

	dialog_data->cancel_button = gtk_button_new_from_stock(GTK_STOCK_CANCEL);
	gtk_widget_show (dialog_data->cancel_button);
	gtk_dialog_add_action_widget (GTK_DIALOG (dialog_data->dialog1), dialog_data->cancel_button, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (dialog_data->cancel_button, GTK_CAN_DEFAULT);

	dialog_data->extract_button = gtk_button_new();
	dialog_data->extract_image = xa_main_window_find_image("extract_button.png", GTK_ICON_SIZE_SMALL_TOOLBAR);
	dialog_data->extract_hbox = gtk_hbox_new(FALSE, 4);
	dialog_data->extract_label = gtk_label_new_with_mnemonic(_("_Extract"));
	gtk_box_pack_start(GTK_BOX(dialog_data->extract_hbox), dialog_data->extract_image, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(dialog_data->extract_hbox), dialog_data->extract_label, FALSE, FALSE, 0);
	gtk_widget_show_all(dialog_data->extract_hbox);
	gtk_container_add(GTK_CONTAINER(dialog_data->extract_button), dialog_data->extract_hbox);
	gtk_widget_show (dialog_data->extract_button);
	gtk_dialog_add_action_widget (GTK_DIALOG (dialog_data->dialog1), dialog_data->extract_button, GTK_RESPONSE_OK);
	gtk_dialog_set_default_response (GTK_DIALOG (dialog_data->dialog1), GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (dialog_data->extract_button, GTK_CAN_DEFAULT);
	return dialog_data;
}

static void fresh_update_toggled_cb (GtkToggleButton *button, Extract_dialog_data *data)
{
	gboolean active = gtk_toggle_button_get_active (button);
	if (active)
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (data->update), FALSE);
}

static void update_fresh_toggled_cb (GtkToggleButton *button, Extract_dialog_data *data)
{
	if (data->fresh == NULL)
		return;
	gboolean active = gtk_toggle_button_get_active (button);
	if (active)
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (data->fresh), FALSE);
}

gchar *parse_extract_dialog_options ( XArchive *archive , Extract_dialog_data *dialog_data, GtkTreeSelection *selection)
{
	gchar *command = NULL;
	gchar *name = NULL;
	gchar *extract_path = NULL;
	gchar *destination_path = NULL;
	gboolean done = FALSE;
	gboolean all_files = FALSE;
	unsigned long long int file_size, file_offset;

    while ( ! done )
	{
		switch (gtk_dialog_run ( GTK_DIALOG (dialog_data->dialog1 ) ) )
		{
			case GTK_RESPONSE_CANCEL:
			case GTK_RESPONSE_DELETE_EVENT:
			done = TRUE;
            gtk_widget_destroy ( dialog_data->dialog1 );
			break;

			case GTK_RESPONSE_OK:
            destination_path = g_strdup (gtk_entry_get_text ( GTK_ENTRY (dialog_data->destination_path_entry) ));
			extract_path = EscapeBadChars ( destination_path );
			if ( strlen ( extract_path ) > 0 )
			{
				done = TRUE;
                gtk_widget_set_sensitive (Stop_button,TRUE);
				all_files = gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON ( dialog_data->all_files_radio ));
                gtk_widget_destroy ( extract_window->dialog1 );
				if ( all_files )
				{
                    gchar *text = g_strconcat (_("Extracting files to "), destination_path , NULL );
                    Update_StatusBar ( text );
                    g_free (text);
					g_free (destination_path);
					switch ( archive->type )
					{
						case XARCHIVETYPE_RAR:
                        if (archive->passwd !=NULL)
							command = g_strconcat ( "rar x -p",archive->passwd," -o+ -idp " , archive->escaped_path , " " , extract_path , NULL );
                        else
							command = g_strconcat ( "rar x -o+ -idp " , archive->escaped_path , " " , extract_path , NULL );
						break;

						case XARCHIVETYPE_TAR:
						command = g_strconcat ( "tar xfv " , archive->escaped_path , " -C " , extract_path , NULL );
						break;

						case XARCHIVETYPE_TAR_BZ2:
						command = g_strconcat ( "tar xfjv " , archive->escaped_path , " -C " , extract_path , NULL );
						break;

						case XARCHIVETYPE_TAR_GZ:
						command = g_strconcat ( "tar xfzv " , archive->escaped_path , " -C " , extract_path , NULL );
						break;

                        case XARCHIVETYPE_ZIP:
                        if ( archive->passwd != NULL )
							command = g_strconcat ( "unzip -o -P " , archive->passwd , " " , archive->escaped_path , " -d " , extract_path , NULL );
                        else
							command = g_strconcat ( "unzip -o " , archive->escaped_path , " -d " , extract_path , NULL );
						break;

						case XARCHIVETYPE_RPM:
                        chdir ( extract_path );
                        command = g_strconcat ( "cpio --make-directories -F " , archive->tmp , " -i" , NULL );
                        break;

                        case XARCHIVETYPE_7ZIP:
                        if (archive->passwd != NULL)
							command = g_strconcat ( "7za x -aoa -bd -p",archive->passwd," ", archive->escaped_path , " -o" , extract_path , NULL );
                        else
							command = g_strconcat ( "7za x -aoa -bd " , archive->escaped_path , " -o" , extract_path , NULL );
                        break;

						case XARCHIVETYPE_ARJ:
						if (archive->passwd !=NULL)
							command = g_strconcat ( "arj x -g",archive->passwd," -i -y " , archive->escaped_path , " " , extract_path , NULL );
                        else
							command = g_strconcat ( "arj x -i -y " , archive->escaped_path , " " , extract_path , NULL );
						break;

						case XARCHIVETYPE_ISO:
						command = NULL;

						gtk_tree_model_get_iter_first (model,&iter);
						gtk_tree_model_get (model, &iter,
                        0, &name,
                        2, &file_size,
                        4, &file_offset,
                        -1);
						xa_extract_iso_file (archive, extract_path, name , file_size, file_offset );
						g_free (name);

						while (gtk_tree_model_iter_next (model,&iter) == TRUE )
						{
							gtk_tree_model_get (model, &iter,
							0, &name,
							2, &file_size,
							4, &file_offset,
							-1);
							xa_extract_iso_file (archive, extract_path, name , file_size, file_offset );
							g_free (name);
						}
						SetButtonState (1,1,0,0,1);
						OffTooltipPadlock();
						Update_StatusBar ( _("Operation completed.") );
						break;
						
						default:
						command = NULL;
					}
					if ( command != NULL )
						return command;
				}
				/* Here we handle only the selected files */
				else
				{
					/* ISO extraction is different from the other type of archives */
					if (archive->type == XARCHIVETYPE_ISO)
					{
						GList *row_list = NULL;
						selection = gtk_tree_view_get_selection ( GTK_TREE_VIEW (treeview1) );
						row_list = gtk_tree_selection_get_selected_rows (selection, &model);
						while (row_list)
						{
							gtk_tree_model_get_iter(model, &iter, row_list->data);
							gtk_tree_model_get (model, &iter,
							0, &name,
							2, &file_size,
							4, &file_offset,
							-1);
							gtk_tree_path_free (row_list->data);
							/* TODO: to support the extraction with the path/filename instead of the filename only */
							gchar *filename = StripPathFromFilename (name);
							xa_extract_iso_file (archive, extract_path, filename , file_size, file_offset );
							g_free (name);
							row_list = row_list->next;
						}
						g_list_free (row_list);
						SetButtonState (1,1,0,0,1);
						OffTooltipPadlock();
						Update_StatusBar ( _("Operation completed.") );
					}
					/* The rest of other archive types */
					else
					{
						names = g_string_new ( " " );
						gtk_tree_selection_selected_foreach (selection, (GtkTreeSelectionForeachFunc) ConcatenateFileNames, names );
						command = ChooseCommandtoExecute ( 1 , names, extract_path );
						if ( command != NULL )
						{
							g_string_free (names , FALSE );
							return command;
						}
					}
				}
			}
			else
				response = ShowGtkMessageDialog (GTK_WINDOW (MainWindow),GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK, _("Please select where to extract the files !") );
			break;
		}
	}
	return command;
}
