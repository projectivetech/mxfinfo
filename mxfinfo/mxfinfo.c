#include <time.h>
#include <ruby.h>
#include "avid_mxf_info.h"

/* Helpers. */

static
VALUE rb_time_from_mxf_timestamp(mxfTimestamp *ts)
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

  return rb_time_new(mktime(tmts), ts->qmsec);
}

static
VALUE rb_str_from_label(char *buf, const unsigned char *lbl, int len)
{
  char hex[len * 2 + 1];
  for(int i = 0; i < len; i++)
  {
    sprintf(&hex[i * 2], "%02x", uid[i]);
  }
  hex[len * 2 + 1] = '\0';
  return rb_str_new2(hex);
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

#define CIO_STRING_GETTER(name, attr)                                       \
  static                                                                    \
  VALUE cio_get_##name (VALUE self)                                         \
  {                                                                         \
    AvidMXFInfo *info;                                                      \
    Data_Get_Struct(self, AvidMXFInfo, info);                               \
    return ((info-> attr ) != NULL) ? rb_str_new2(info-> attr ) : Qnil;     \
  }

#define CIO_LABEL_GETTER(name, attr, len)                                   \
  static                                                                    \
  VALUE cio_get_##name (VALUE self)                                         \
  {                                                                         \
    AvidMXFInfo *info;                                                      \
    Data_Get_Struct(self, AvidMXFInfo, info);                               \
    return rb_str_from_label((unsigned char*) ( info-> attr ), len )        \
  }

#define CIO_RATIONAL_GETTER(name, attr)                                     \
  static                                                                    \
  VALUE cio_get_##name (VALUE self)                                         \
  {                                                                         \
    AvidMXFInfo *info;                                                      \
    Data_Get_Struct(self, AvidMXFInfo, info);                               \
    return rb_rational_new(INT2FIX((info-> attr ).numerator),               \
      INT2FIX((info-> attr ).denominator));                                 \
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
CIO_RATIONAL_GETTER(project_edit_rate, projectEditRate)
CIO_INT64_GETTER(clip_duration, clipDuration)
CIO_LABEL_GETTER(material_package_uid, materialPackageUID, 32)
CIO_INT_GETTER(num_video_tracks, numVideoTracks)
CIO_INT_GETTER(num_audio_tracks, numAudioTracks)
CIO_STRING_GETTER(tracks_string, tracksString)
CIO_UINT_GETTER(track_number, trackNumber)
CIO_INT_GETTER(is_video, isVideo)
CIO_RATIONAL_GETTER(edit_rate, editRate)
CIO_INT64_GETTER(track_duration, trackDuration)
CIO_INT64_GETTER(segment_duration, segmentDuration)
CIO_INT64_GETTER(segment_offset, segmentOffset)
CIO_INT64_GETTER(start_timecode, startTimecode)
CIO_LABEL_GETTER(essence_container_label, essenceContainerLabel, 16)
CIO_LABEL_GETTER(file_source_package_uid, fileSourcePackageUID, 32)
CIO_LABEL_GETTER(picture_coding_label, pictureCodingLabel, 16)
CIO_UINT_GETTER(frame_layout, frameLayout)
CIO_RATIONAL_GETTER(aspect_ratio, aspectRatio)
CIO_UINT_GETTER(stored_width, storedWidth)
CIO_UINT_GETTER(stored_height, storedHeight)
CIO_UINT_GETTER(display_width, displayWidth)
CIO_UINT_GETTER(display_height, displayHeight)
CIO_RATIONAL_GETTER(audio_sampling_rate, audioSamplingRate)
CIO_UINT_GETTER(channel_count, channelCount)
CIO_UINT_GETTER(quantization_bits, quantizationBits)
CIO_STRING_GETTER(physical_package_name, physicalPackageName)
CIO_LABEL_GETTER(physical_source_package_uid, physicalSourcePackageUID, 32)
CIO_STRING_GETTER(physical_package_locator, physicalPackageLocator)
/* ... */

/* Random attributes that need special treatment. */

static
VALUE cio_get_clip_created(VALUE self)
{
  AvidMXFInfo *info;
  Data_Get_Struct(self, AvidMXFInfo, info);
  return rb_time_from_mxf_timestamp(&info->clipCreated);
}

/*
 * Left that in for reference (array creation).
static
VALUE cio_get_material_package_uid(VALUE self)
{
  AvidMXFInfo *info;
  Data_Get_Struct(self, AvidMXFInfo, info);

  unsigned char *uid = (unsigned char*) &info->materialPackageUID;

  VALUE ary = rb_ary_new2(32);
  for(unsigned int i = 0; i < 32; i++) 
  {
    rb_ary_store(ary, i, CHR2FIX(uid[i]));
  }

  return ary;
}
*/

static
VALUE cio_get_essence_type(VALUE self)
{
  AvidMXFInfo *info;
  Data_Get_Struct(self, AvidMXFInfo, info);
  return rb_str_new2(get_essence_type_string(info->essenceType, info->projectEditRate)));
}

static
VALUE cio_get_physical_package_type(VALUE self)
{
  AvidMXFInfo *info;
  Data_Get_Struct(self, AvidMXFInfo, info);

  VALUE ret;
  switch (info->physicalPackageType)
  {
  case TAPE_PHYS_TYPE:
    ret = ID2SYM(rb_intern('tape'));
    break;
  case IMPORT_PHYS_TYPE:
    ret = ID2SYM(rb_intern('import'));
    break;
  case RECORDING_PHYS_TYPE:
    ret = ID2SYM(rb_intern('recording'));
    break;
  case UNKNOWN_PHYS_TYPE:
  default:
    ret = ID2SYM(rb_intern('unknown'));
    break;
  }

  return ret;
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
  rb_define_method(c_infoobject, "essence_type", cio_get_essence_type, 0);
  rb_define_method(c_infoobject, "essence_container_label", cio_get_essence_container_label, 0);
  rb_define_method(c_infoobject, "file_source_package_uid", cio_get_file_source_package_uid, 0);
  rb_define_method(c_infoobject, "picture_coding_label", cio_get_picture_coding_label, 0);
  rb_define_method(c_infoobject, "frame_layout", cio_get_frame_layout, 0);
  rb_define_method(c_infoobject, "aspect_ratio", cio_get_aspect_ratio, 0);
  rb_define_method(c_infoobject, "stored_width", cio_get_stored_width, 0);
  rb_define_method(c_infoobject, "stored_height", cio_get_stored_height, 0);
  rb_define_method(c_infoobject, "display_width", cio_get_display_width, 0);
  rb_define_method(c_infoobject, "display_height", cio_get_display_height, 0);
  rb_define_method(c_infoobject, "audio_sampling_rate", cio_get_audio_sampling_rate, 0);
  rb_define_method(c_infoobject, "channel_count", cio_get_channel_count, 0);
  rb_define_method(c_infoobject, "quantization_bits", cio_get_quantization_bits, 0);
  rb_define_method(c_infoobject, "physical_package_name", cio_get_physical_package_name, 0);
  rb_define_method(c_infoobject, "physical_source_package_uid", cio_get_physical_source_package_uid, 0);
  rb_define_method(c_infoobject, "physical_package_type", cio_get_physical_package_type, 0);
  rb_define_method(c_infoobject, "physical_package_locator", cio_get_physical_package_locator, 0);
}
