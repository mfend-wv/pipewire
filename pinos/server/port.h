/* Pinos
 * Copyright (C) 2015 Wim Taymans <wim.taymans@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef __PINOS_PORT_H__
#define __PINOS_PORT_H__

#include <glib-object.h>

G_BEGIN_DECLS

typedef struct _PinosPort PinosPort;
typedef struct _PinosPortClass PinosPortClass;
typedef struct _PinosPortPrivate PinosPortPrivate;

#include <spa/include/spa/buffer.h>

#include <pinos/client/introspect.h>
#include <pinos/server/daemon.h>


#define PINOS_TYPE_PORT             (pinos_port_get_type ())
#define PINOS_IS_PORT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), PINOS_TYPE_PORT))
#define PINOS_IS_PORT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), PINOS_TYPE_PORT))
#define PINOS_PORT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), PINOS_TYPE_PORT, PinosPortClass))
#define PINOS_PORT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), PINOS_TYPE_PORT, PinosPort))
#define PINOS_PORT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), PINOS_TYPE_PORT, PinosPortClass))
#define PINOS_PORT_CAST(obj)        ((PinosPort*)(obj))
#define PINOS_PORT_CLASS_CAST(klass)((PinosPortClass*)(klass))

/**
 * PinosPort:
 *
 * Pinos port object class.
 */
struct _PinosPort {
  GObject object;

  PinosDirection direction;
  uint32_t id;
  PinosNode *node;

  SpaBuffer **buffers;
  guint       n_buffers;

  PinosPortPrivate *priv;
};

/**
 * PinosPortClass:
 *
 * Pinos port object class.
 */
struct _PinosPortClass {
  GObjectClass parent_class;

  void        (*get_ringbuffer)     (PinosPort          *port,
                                     PinosProperties    *props,
                                     GTask              *task);
};

typedef gboolean (*PinosBufferCallback) (PinosPort *port, uint32_t buffer_id, GError **error, gpointer user_data);
typedef gboolean (*PinosEventCallback) (PinosPort *port, SpaEvent *event, GError **error, gpointer user_data);

/* normal GObject stuff */
GType               pinos_port_get_type           (void);

void                pinos_port_set_received_cb        (PinosPort *port,
                                                       PinosBufferCallback buffer_cb,
                                                       PinosEventCallback event_cb,
                                                       gpointer user_data,
                                                       GDestroyNotify notify);
gulong              pinos_port_add_send_cb            (PinosPort *port,
                                                       PinosBufferCallback buffer_cb,
                                                       PinosEventCallback event_cb,
                                                       gpointer user_data,
                                                       GDestroyNotify notify);
void                pinos_port_remove_send_cb         (PinosPort *port,
                                                       gulong     id);

void                pinos_port_remove                 (PinosPort *port);

gboolean            pinos_port_have_common_format     (PinosPort  *port,
                                                       guint       n_filter_formats,
                                                       SpaFormat **filter_formats,
                                                       GError    **error);

void                pinos_port_activate             (PinosPort *port);
void                pinos_port_deactivate           (PinosPort *port);

gboolean            pinos_port_send_buffer          (PinosPort   *port,
                                                     uint32_t     buffer_id,
                                                     GError     **error);
gboolean            pinos_port_send_event           (PinosPort   *port,
                                                     SpaEvent    *event,
                                                     GError     **error);
gboolean            pinos_port_receive_buffer       (PinosPort   *port,
                                                     uint32_t     buffer_id,
                                                     GError     **error);
gboolean            pinos_port_receive_event        (PinosPort   *port,
                                                     SpaEvent    *event,
                                                     GError     **error);

G_END_DECLS

#endif /* __PINOS_PORT_H__ */
