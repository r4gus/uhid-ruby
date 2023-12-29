#include "ruby.h"
#include "linux/uhid.h"
#include <fcntl.h>

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
    ev.type = UHID_CREATE2;
    strncpy((char*)ev.u.create2.name, StringValueCStr(name), 127);
    ev.u.create2.rd_size = RSTRING_LEN(StringValue(data));
    memcpy(ev.u.create2.rd_data, RSTRING_PTR(StringValue(data)), ev.u.create2.rd_size);
    ev.u.create2.bus = BUS_USB;
    ev.u.create2.vendor = (uint32_t)NUM2INT(vendor);
    ev.u.create2.product = (uint32_t)NUM2INT(product);
    ev.u.create2.version = 0;
	ev.u.create2.country = 0;

    ID file_var = rb_intern("@file");
    VALUE file = rb_ivar_get(self, file_var);
    Check_Type(file, T_FILE);
    VALUE file_fileno = rb_funcall(file, rb_intern("fileno"), 0);
    int fd = NUM2INT(file_fileno);
       
    // This makes the open file non-blocking
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);

    cuhid_write(fd, &ev);

    return Qnil;
}

/*
 * Close the open uhid device.
 */
static VALUE
cuhid_destroy(VALUE self)
{
    struct uhid_event ev; 
    
    memset(&ev, 0, sizeof(ev));
    ev.type = UHID_DESTROY;

    ID file_var = rb_intern("@file");
    VALUE file = rb_ivar_get(self, file_var);
    Check_Type(file, T_FILE);
    VALUE file_fileno = rb_funcall(file, rb_intern("fileno"), 0);
    int fd = NUM2INT(file_fileno);

    cuhid_write(fd, &ev);

    return Qnil;
}

/*
 * Write the given data to the host.
 *
 * Make sure you send the data in a format, expected
 * by the host, e.g. in 64 byte chunks.
 *
 * @param data [String] the data to be sent to the host.
 * @returns [Integer] the number of bytes sent.
 */
static VALUE
cuhid_write_data(VALUE self, VALUE data)
{
    const char* d = RSTRING_PTR(StringValue(data));
    size_t l = RSTRING_LEN(StringValue(data));
    size_t size = l;
    if (size > UHID_DATA_MAX) {
        size = UHID_DATA_MAX;
    }

    struct uhid_event ev; 
    
    memset(&ev, 0, sizeof(ev));
    ev.type = UHID_INPUT2;
    memcpy(ev.u.input2.data, d, size);
    ev.u.input2.size = size;

    ID file_var = rb_intern("@file");
    VALUE file = rb_ivar_get(self, file_var);
    Check_Type(file, T_FILE);
    VALUE file_fileno = rb_funcall(file, rb_intern("fileno"), 0);
    int fd = NUM2INT(file_fileno);

    cuhid_write(fd, &ev);
    
    // We the number of processed bytes
    return INT2NUM(size);
}

/*
 * Read data from the host.
 *
 * All returned hashes contain the type key that specifies the type 
 * of the received packet. Valid types are: START, STOP, OPEN, CLOSE,
 * OUTPUT. Only OUTPUT Hashes contain a data field that holds the
 * data received by the host.
 *
 * @returns [nil, Hash] a Hash that contains the received data or nil if no data is available.
 */
static VALUE
cuhid_read_data(VALUE self)
{
    ID file_var = rb_intern("@file");
    VALUE file = rb_ivar_get(self, file_var);
    Check_Type(file, T_FILE);
    VALUE file_fileno = rb_funcall(file, rb_intern("fileno"), 0);
    int fd = NUM2INT(file_fileno);

    struct uhid_event ev; 
    
    memset(&ev, 0, sizeof(ev));
    ssize_t ret = read(fd, &ev, sizeof(ev));

    if (ret == -1) { // EAGAIN
        return Qnil;
    }
    
    VALUE h = rb_hash_new();
    switch (ev.type) {
    case UHID_START:
        rb_hash_aset(h, rb_str_new_cstr("type"), rb_str_new_cstr("START"));
        return h;
    case UHID_STOP:
        rb_hash_aset(h, rb_str_new_cstr("type"), rb_str_new_cstr("STOP"));
        return h;
    case UHID_OPEN:
        rb_hash_aset(h, rb_str_new_cstr("type"), rb_str_new_cstr("OPEN"));
        return h;
    case UHID_CLOSE:
        rb_hash_aset(h, rb_str_new_cstr("type"), rb_str_new_cstr("CLOSE"));
        return h;
    case UHID_OUTPUT:
        rb_hash_aset(h, rb_str_new_cstr("type"), rb_str_new_cstr("OUTPUT"));
        rb_hash_aset(h, rb_str_new_cstr("data"), rb_str_new((const char *) ev.u.output.data, ev.u.output.size));
        return h;
    default:
        return Qnil;
    }
}

void Init_uhid() {
    VALUE cUhid  = rb_const_get(rb_cObject, rb_intern("Uhid")); 
    rb_define_private_method(cUhid, "create", cuhid_create, 4);
    rb_define_private_method(cUhid, "destroy", cuhid_destroy, 0);
    rb_define_method(cUhid, "write", cuhid_write_data, 1);
    rb_define_method(cUhid, "read", cuhid_read_data, 0);
}
