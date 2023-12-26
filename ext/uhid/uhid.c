#include "ruby.h"
#include "linux/uhid.h"

static VALUE hello_world(VALUE self) {
    return rb_str_new2("Hello, World!");
}

static void
cuhid_write(int fd, const struct uhid_event *ev)
{
	ssize_t ret;

	ret = write(fd, ev, sizeof(*ev));
	if (ret < 0) {
        rb_raise(rb_eRuntimeError, "Cannot write to uhid");
	} else if (ret != sizeof(*ev)) {
        rb_raise(rb_eRuntimeError, "Wrong size written to uhid: %zd != %zu",
                ret, sizeof(ev));
	} 
}

static VALUE 
cuhid_create(VALUE self, VALUE name, VALUE vendor, VALUE product, VALUE data) 
{
    Check_Type(name, T_STRING);
    Check_Type(data, T_STRING);

    struct uhid_event ev; 
    
    memset(&ev, 0, sizeof(ev));
    ev.type = UHID_CREATE;
    strcpy((char*)ev.u.create.name, StringValueCStr(name));
    ev.u.create.rd_data = (unsigned char*)RSTRING_PTR(StringValue(data));
    ev.u.create.rd_size = RSTRING_LEN(StringValue(data));
    ev.u.create.bus = BUS_USB;
    ev.u.create.vendor = (uint16_t)NUM2INT(vendor);
    ev.u.create.product = (uint16_t)NUM2INT(product);
    ev.u.create.version = 0;
	ev.u.create.country = 0;

    ID file_var = rb_intern("@file");
    VALUE file = rb_ivar_get(self, file_var);
    Check_Type(file, T_FILE);
    VALUE file_fileno = rb_funcall(file, rb_intern("fileno"), 0);
    int fd = NUM2INT(file_fileno);

    cuhid_write(fd, &ev);

    return Qnil;
}

void Init_uhid() {
    VALUE cUhid  = rb_const_get(rb_cObject, rb_intern("Uhid")); 
    rb_define_method(cUhid, "hello_world", hello_world, 0);
    rb_define_method(cUhid, "create", cuhid_create, 4);
}
