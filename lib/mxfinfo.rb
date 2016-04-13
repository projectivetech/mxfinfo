require 'mxfinfo.so'
require 'mxfinfo/version.rb'

module MXFInfo
  def self.scan(path)
    io = IO.new(2)
    stderr = io.dup
    io.reopen(IO::NULL)
    InfoObject.new path
  rescue => e
    io.reopen stderr
    stderr.close
    raise e
  end

  class InfoObject
    alias_method :videotracks, :num_video_tracks
    alias_method :v_tracks, :num_video_tracks 
    alias_method :video_tracks, :num_video_tracks
    alias_method :audiotracks, :num_audio_tracks
    alias_method :audio_tracks, :num_audio_tracks
    alias_method :a_tracks, :num_audio_tracks
    alias_method :clip_created_at, :clip_created
    alias_method :essence_label, :essence_container_label
    alias_method :t_number, :track_number
    alias_method :tracknumber, :track_number
    alias_method :channelcount, :channel_count
    alias_method :c_count, :channel_count
    alias_method :file_package_uid, :file_source_package_uid

    def render_file?
      physical_package_name == "Precompute Source Mob"
    end

    alias_method :essence_type_orig, :essence_type
    def essence_type
      result = self.send(:essence_type_orig)
      if result.start_with?("DNxHD")
        result.gsub!(/ \(\d+\)/, "")
      end
      result
    end
  end
end
