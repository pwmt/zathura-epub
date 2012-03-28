/* See LICENSE file for license and copyright information */

#include "epub.h"

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
  const char* path = zathura_document_get_path(document);

  zathura_document_set_data(document, NULL);
  zathura_document_set_number_of_pages(document, 0);

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
  unsigned int page_index      = zathura_page_get_index(page);
  zathura_document_t* document = zathura_page_get_document(page);

  zathura_page_set_width(page,  0);
  zathura_page_set_height(page, 0);

  return ZATHURA_ERROR_OK;
}

zathura_error_t
epub_page_clear(zathura_page_t* page, void* data)
{
  return ZATHURA_ERROR_OK;
}

#if HAVE_CAIRO
zathura_error_t
epub_page_render_cairo(zathura_page_t* page, void* data, cairo_t*
    cairo, bool printing)
{
  return ZATHURA_ERROR_OK;
}
#endif
