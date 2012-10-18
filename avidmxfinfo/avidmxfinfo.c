#include <time.h>
#include <ruby.h>
#include "avid_mxf_info.h"

/* Helpers. */

static
time_t mxf_timestamp_to_time_t(mxfTimestamp *ts)
{
	/*
	 * NOTE: This is probably not thread-safe, since struct tm*
	 * returned by localtime is shared static.
	 */
	time_t t;
	time(&t);
	struct tm *tmts;
	tmts = localtime(&t);

	tmts->tm_year = (int) ts->year - 1900;
	tmts->tm_mon = (int) ts->month;
	tmts->tm_mday = (int) ts->day;
	tmts->tm_hour = (int) ts->hour;
	tmts->tm_min = (int) ts->min;
	tmts->tm_sec = (int) ts->sec;

	return mktime(tmts);
}

/* Ruby API. */

VALUE c_avidmxfinfo = Qnil;

static
void camxfi_free(void *ptr) {
	ami_free_info(ptr);
}

static 
VALUE camxfi_new(VALUE class, VALUE path)
{
	Check_Type(path, T_STRING);
	const char *fn = StringValuePtr(path);

	AvidMXFInfo *info = ALLOC(AvidMXFInfo);

	int result = ami_read_info(fn, info, 1);

	if(result != 0) 
	{
		ami_free_info(info);

		switch(result) 
		{
            case -2:
            	rb_raise(rb_eIOError, "Failed to open file.");
                break;
            case -3:
            	rb_raise(rb_eIOError, "Failed to read header partition.");
                break;
            case -4:
            	rb_raise(rb_eException, "File is not OP-Atom.");
                break;
            case -1:
            default:
            	rb_raise(rb_eException, "Failed to read info.");
                break;
		}

		return Qnil;
	} 
	else
	{
		VALUE tdata = Data_Wrap_Struct(class, 0, camxfi_free, info);
		return tdata;
	}
}

static
VALUE camxfi_get_clip_name(VALUE self)
{
  AvidMXFInfo *ptr;
  Data_Get_Struct(self, AvidMXFInfo, ptr);
	return (ptr->clipName != NULL) ? rb_str_new2(ptr->clipName) : Qnil;
}

static
VALUE camxfi_get_project_name(VALUE self)
{
  AvidMXFInfo *ptr;
  Data_Get_Struct(self, AvidMXFInfo, ptr);
  return (ptr->projectName != NULL) ? rb_str_new2(ptr->projectName) : Qnil;
}

static
VALUE camxfi_get_clip_created(VALUE self)
{
  AvidMXFInfo *ptr;
  Data_Get_Struct(self, AvidMXFInfo, ptr);
  return rb_time_new(mxf_timestamp_to_time_t(&ptr->clipCreated), ptr->clipCreated.qmsec);
}

static
VALUE camxfi_get_project_edit_rate(VALUE self)
{
  AvidMXFInfo *ptr;
  Data_Get_Struct(self, AvidMXFInfo, ptr);
  return rb_rational_new(INT2FIX(ptr->projectEditRate.numerator), INT2FIX(ptr->projectEditRate.denominator));
}


static
VALUE camxfi_get_channel_count(VALUE self)
{
  AvidMXFInfo *ptr;
  Data_Get_Struct(self, AvidMXFInfo, ptr);
  return rb_int_new(ptr->channelCount);
}

void Init_avidmxfinfo()
{
	c_avidmxfinfo = rb_define_class("AvidMXFInfo", rb_cObject);
	rb_define_singleton_method(c_avidmxfinfo, "new", camxfi_new, 1);

    rb_define_method(c_avidmxfinfo, "clip_name", camxfi_get_clip_name, 0);
    rb_define_method(c_avidmxfinfo, "project_name", camxfi_get_project_name, 0);
    rb_define_method(c_avidmxfinfo, "clip_created", camxfi_get_clip_created, 0);
    rb_define_method(c_avidmxfinfo, "project_edit_rate", camxfi_get_project_edit_rate, 0);
    /* ... */
	rb_define_method(c_avidmxfinfo, "channel_count", camxfi_get_channel_count, 0);
}
