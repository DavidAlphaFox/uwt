/* Libuv bindings for OCaml
 * http://github.com/fdopen/uwt
 * Copyright (C) 2015-2016 Andreas Hauptmann
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, with linking exceptions;
 * either version 2.1 of the License, or (at your option) any later
 * version. See COPYING file for details.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 */

#include "uwt_stubs_tty.h"

CAMLprim value
uwt_tty_init(value o_loop,value o_fd, value o_readable)
{
  uv_loop_t * l = Uv_loop_val(o_loop);
  const int fd = FD_VAL(o_fd);
  const int readable = Long_val(o_readable) == 1;
  value ret = uwt__handle_res_create(UV_TTY, true);
  value dc = Field(ret,0);
  struct handle * h =  Handle_val(dc);
  h->initialized = 1;
  const int erg = uv_tty_init(l, (uv_tty_t*)h->handle, fd, readable);
  if ( erg < 0 ){
    uwt__free_handle(h);
    Field(dc,1) = 0;
    Field(ret,0) = Val_uwt_error(erg);
    Tag_val(ret) = Error_tag;
  }
  return ret;
}

CAMLprim value
uwt_tty_set_mode_na(value o_tty,value o_mode)
{
  HANDLE_INIT_NOUNINIT_NA(s, o_tty);
  int mode ;
  switch ( Long_val(o_mode) ){
  case 2: mode = UV_TTY_MODE_IO; break;
  case 1: mode = UV_TTY_MODE_RAW; break;
  default: assert(false); /* fall */
  case 0: mode = UV_TTY_MODE_NORMAL; break;
  }
  int ret = uv_tty_set_mode((uv_tty_t*)s->handle,mode);
  return (VAL_UWT_UNIT_RESULT(ret));
}

CAMLprim value
uwt_tty_reset_mode_na(value unit)
{
  (void) unit;
  int x = uv_tty_reset_mode();
  return (VAL_UWT_UNIT_RESULT(x));
}

CAMLprim value
uwt_tty_get_winsize(value o_tty)
{
  HANDLE_NO_UNINIT_CLOSED_WRAP(o_tty);
  CAMLparam1(o_tty);
  CAMLlocal1(tup);
  struct handle * s = Handle_val(o_tty);
  value ret;
  int width;
  int height;
  const int erg = uv_tty_get_winsize((uv_tty_t*)s->handle,&width,&height);
  if ( erg < 0 ){
    ret = caml_alloc_small(1,Error_tag);
    Field(ret,0) = Val_uwt_error(erg);
  }
  else {
    tup = caml_alloc_small(2,0);
    Field(tup,0) = Val_long(width);
    Field(tup,1) = Val_long(height);
    ret = caml_alloc_small(1,Ok_tag);
    Field(ret,0) = tup;
  }
  CAMLreturn(ret);
}
