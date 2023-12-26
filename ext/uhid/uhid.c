#include "ruby.h"

static VALUE hello_world(VALUE self) {
    return rb_str_new2("Hello, World!");
}

void Init_uhid() {
    VALUE cUhid  = rb_const_get(rb_cObject, rb_intern("Uhid")); 
    rb_define_method(cUhid, "hello_world", hello_world, 0);
}
