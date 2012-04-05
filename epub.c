/* See LICENSE file for license and copyright information */

#include <gtk/gtk.h>
#include <webkit/webkit.h>
#include <stdlib.h>

#include "epub.h"

struct epub_page_s {
  GtkWidget* web_view;
  GtkWidget* window;
  GdkPixbuf* pixbuf;
};

static gboolean cb_load_error(WebKitWebView* web_view, WebKitWebFrame* web_frame, epub_page_t* epub_page);
static gboolean cb_load_finished(WebKitWebView* web_view, WebKitWebFrame* web_frame, epub_page_t* epub_page);

void
register_functions(zathura_plugin_functions_t* functions)
{
  functions->document_open     = epub_document_open;
  functions->document_free     = epub_document_free;
  functions->page_init         = epub_page_init;
  functions->page_clear        = epub_page_clear;
  functions->page_render_cairo = epub_page_render_cairo;
}

ZATHURA_PLUGIN_REGISTER(
  "epub",
  0, 1, 0,
  register_functions,
  ZATHURA_PLUGIN_MIMETYPES({
    "application/epub+zip"
  })
)

zathura_error_t
epub_document_open(zathura_document_t* document)
{
  zathura_document_set_data(document, NULL);
  zathura_document_set_number_of_pages(document, 1);

  return ZATHURA_ERROR_OK;
}

zathura_error_t
epub_document_free(zathura_document_t* document, void* data)
{
  return ZATHURA_ERROR_OK;
}

zathura_error_t
epub_page_init(zathura_page_t* page)
{
  /* init page */
  epub_page_t* epub_page = calloc(1, sizeof(epub_page_t));
  if (epub_page == NULL) {
    return ZATHURA_ERROR_UNKNOWN;
  }

  /* init window and web view */
  epub_page->window = gtk_offscreen_window_new();
  epub_page->web_view = webkit_web_view_new();

  g_signal_connect(epub_page->web_view, "load-error",    G_CALLBACK(cb_load_error),    epub_page);
  g_signal_connect(epub_page->web_view, "load-finished", G_CALLBACK(cb_load_finished), epub_page);

  gtk_container_add(GTK_CONTAINER(epub_page->window), epub_page->web_view);
  gtk_widget_show_all(epub_page->window);

  /* load page.. */
  webkit_web_view_load_uri(WEBKIT_WEB_VIEW(epub_page->web_view), "http://orf.at");
  gtk_main();

  if (epub_page->pixbuf == NULL) {
    epub_page_clear(page, epub_page);
    return ZATHURA_ERROR_UNKNOWN;
  }

  /* set zathura page */
  zathura_page_set_data(page, epub_page);

  zathura_page_set_width(page,  gdk_pixbuf_get_width(epub_page->pixbuf));
  zathura_page_set_height(page, gdk_pixbuf_get_height(epub_page->pixbuf));

  return ZATHURA_ERROR_OK;
}

zathura_error_t
epub_page_clear(zathura_page_t* page, epub_page_t* epub_page)
{
  if (epub_page != NULL) {
    free(epub_page);
  }

  return ZATHURA_ERROR_OK;
}

#if HAVE_CAIRO
zathura_error_t
epub_page_render_cairo(zathura_page_t* page, epub_page_t* epub_page,
    cairo_t* cairo, bool printing)
{
  if (epub_page == NULL || epub_page->pixbuf == NULL || cairo == NULL) {
    return ZATHURA_ERROR_UNKNOWN;
  }

  gdk_cairo_set_source_pixbuf(cairo, epub_page->pixbuf, 0, 0);
  cairo_paint(cairo);

  return ZATHURA_ERROR_OK;
}
#endif

static gboolean
cb_load_error(WebKitWebView* web_view, WebKitWebFrame* web_frame, epub_page_t* epub_page)
{
  gtk_main_quit();

  return TRUE;
}

static gboolean
cb_load_finished(WebKitWebView* web_view, WebKitWebFrame* web_frame, epub_page_t* epub_page)
{
  if (epub_page != NULL && epub_page->window != NULL) {
    epub_page->pixbuf = gtk_offscreen_window_get_pixbuf(GTK_OFFSCREEN_WINDOW(epub_page->window));
  }

  gtk_main_quit();

  return TRUE;
}
