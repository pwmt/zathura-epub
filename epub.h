/* See LICENSE file for license and copyright information */

#ifndef epub_H
#define epub_H

#include <stdbool.h>

#if HAVE_CAIRO
#include <cairo.h>
#endif

#include <zathura/plugin-api.h>

/**
 * Opens a new document
 *
 * @param document The document
 * @return ZATHURA_ERROR_OK if no error occured  
 */
zathura_error_t epub_document_open(zathura_document_t* document);

/**
 * Frees the document
 *
 * @param document The document
 * @param data Custom data
 * @return ZATHURA_ERROR_OK if no error occured  
 */
zathura_error_t epub_document_free(zathura_document_t* document, void* data);

/**
 * Initializes a page
 *
 * @param page The page
 * @return ZATHURA_ERROR_OK if no error occured  
 */
zathura_error_t epub_page_init(zathura_page_t* page);

/**
 * Clear page
 *
 * @param page The page
 * @param data Custom data
 * @return ZATHURA_ERROR_OK if no error occured  
 */
zathura_error_t epub_page_clear(zathura_page_t* page, void* data);

#if HAVE_CAIRO
/**
 * Renders the page to a cairo object
 *
 * @param page The page
 * @param data Custm data
 * @param cairo Cairo object
 * @param printing Render for printing
 * @return ZATHURA_ERROR_OK if no error occured  
 */
zathura_error_t epub_page_render_cairo(zathura_page_t* page, void* data,
    cairo_t* cairo, bool printing);
#endif

#endif // epub_H
