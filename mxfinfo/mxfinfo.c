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

/* Some macros. */

#define CIO_GENERIC_GETTER(cast, name, attr)                                \
  static                                                                    \
  VALUE cio_get_##name (VALUE self)                                         \
  {                                                                         \
    AvidMXFInfo *info;                                                      \
    Data_Get_Struct(self, AvidMXFInfo, info);                               \
    return cast (info-> attr );                                             \
  }

#define CIO_STRING_GETTER( name, attr)                                      \
  static                                                                    \
  VALUE cio_get_##name (VALUE self)                                         \
  {                                                                         \
    AvidMXFInfo *info;                                                      \
    Data_Get_Struct(self, AvidMXFInfo, info);                               \
    return ((info-> attr ) != NULL) ? rb_str_new2(info-> attr ) : Qnil;     \
  }

#define CIO_INT_GETTER(name, attr) CIO_GENERIC_GETTER(INT2FIX, name, attr)
#define CIO_UINT_GETTER(name, attr) CIO_GENERIC_GETTER(UINT2NUM, name, attr)
#define CIO_INT64_GETTER(name, attr) CIO_GENERIC_GETTER(LL2NUM, name, attr)

/* Ruby API. */

VALUE m_mxfinfo = Qnil;
VALUE c_infoobject = Qnil;

static
void cio_free(void *ptr) {
  ami_free_info(ptr);
}

static 
VALUE cio_new(VALUE class, VALUE path)
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
    VALUE tdata = Data_Wrap_Struct(class, 0, cio_free, info);
    return tdata;
  }
}

/* Simple getters. */

CIO_STRING_GETTER(clip_name, clipName)
CIO_STRING_GETTER(project_name, projectName)
CIO_INT64_GETTER(clip_duration, clipDuration)
CIO_INT_GETTER(num_video_tracks, numVideoTracks)
CIO_INT_GETTER(num_audio_tracks, numAudioTracks)
CIO_STRING_GETTER(tracks_string, tracksString)
CIO_UINT_GETTER(track_number, trackNumber)
CIO_INT_GETTER(is_video, isVideo)
CIO_INT64_GETTER(track_duration, trackDuration)
CIO_INT64_GETTER(segment_duration, segmentDuration)
CIO_INT64_GETTER(segment_offset, segmentOffset)
CIO_INT64_GETTER(start_timecode, startTimecode)
/* ... */
CIO_UINT_GETTER(channel_count, channelCount)
/* ... */

/* Complex getters. */

static
VALUE cio_get_clip_created(VALUE self)
{
  AvidMXFInfo *info;
  Data_Get_Struct(self, AvidMXFInfo, info);
  return rb_time_new(mxf_timestamp_to_time_t(&info->clipCreated), info->clipCreated.qmsec);
}

static
VALUE cio_get_project_edit_rate(VALUE self)
{
  AvidMXFInfo *info;
  Data_Get_Struct(self, AvidMXFInfo, info);
  return rb_rational_new(INT2FIX(info->projectEditRate.numerator), INT2FIX(info->projectEditRate.denominator));
}

static
VALUE cio_get_material_package_uid(VALUE self)
{
  AvidMXFInfo *info;
  Data_Get_Struct(self, AvidMXFInfo, info);

  /* Let's hope the struct is packed. */
  unsigned char *uid = (unsigned char*) &info->materialPackageUID;

  VALUE ary = rb_ary_new2(32);
  for(unsigned int i = 0; i < 32; i++) 
  {
    rb_ary_store(ary, i, CHR2FIX(uid[i]));
  }

  return ary;
}

static
VALUE cio_get_edit_rate(VALUE self)
{
  AvidMXFInfo *info;
  Data_Get_Struct(self, AvidMXFInfo, info);
  return rb_rational_new(INT2FIX(info->editRate.numerator), INT2FIX(info->editRate.denominator));
}

/* Module init. */

void Init_mxfinfo()
{
  m_mxfinfo = rb_define_module("MXFInfo");	

	c_infoobject = rb_define_class_under(m_mxfinfo, "InfoObject", rb_cObject);
  rb_define_singleton_method(c_infoobject, "new", cio_new, 1);
	
  rb_define_method(c_infoobject, "clip_name", cio_get_clip_name, 0);
  rb_define_method(c_infoobject, "project_name", cio_get_project_name, 0);
  rb_define_method(c_infoobject, "clip_created", cio_get_clip_created, 0);
  rb_define_method(c_infoobject, "project_edit_rate", cio_get_project_edit_rate, 0);
  rb_define_method(c_infoobject, "clip_duration", cio_get_clip_duration, 0);
  rb_define_method(c_infoobject, "material_package_uid", cio_get_material_package_uid, 0);

  /* AvidTaggedValue *userComments */
  /* int numUserComments */
  /* AvidTaggedValue *materialPackageAttributes */
  /* int numMaterialPackageAttributes */
  
  rb_define_method(c_infoobject, "num_video_tracks", cio_get_num_video_tracks, 0);
  rb_define_method(c_infoobject, "num_audio_tracks", cio_get_num_audio_tracks, 0);
  rb_define_method(c_infoobject, "tracks_string", cio_get_tracks_string, 0);
  rb_define_method(c_infoobject, "track_number", cio_get_track_number, 0);
  rb_define_method(c_infoobject, "is_video", cio_get_is_video, 0);
  rb_define_method(c_infoobject, "edit_rate", cio_get_edit_rate, 0);
  rb_define_method(c_infoobject, "track_duration", cio_get_track_duration, 0);
  rb_define_method(c_infoobject, "segment_duration", cio_get_segment_duration, 0);
  rb_define_method(c_infoobject, "segment_offset", cio_get_segment_offset, 0);
  rb_define_method(c_infoobject, "start_timecode", cio_get_start_timecode, 0);
  
  /*
  AvidEssenceType essenceType;
  mxfUMID fileSourcePackageUID;
  mxfUL essenceContainerLabel;
  mxfUL pictureCodingLabel;
  uint8_t frameLayout;
  mxfRational aspectRatio;
  uint32_t storedWidth;
  uint32_t storedHeight;
  uint32_t displayWidth;
  uint32_t displayHeight;
  mxfRational audioSamplingRate;
  */

  rb_define_method(c_infoobject, "channel_count", cio_get_channel_count, 0);

  /*
  uint32_t quantizationBits;
  char *physicalPackageName;
  mxfUMID physicalSourcePackageUID;
  AvidPhysicalPackageType physicalPackageType;
  char *physicalPackageLocator;
  */
}
