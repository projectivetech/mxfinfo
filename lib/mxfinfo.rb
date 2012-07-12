require "forwardable"
require "mxfinfo/version"
require "mxfinfo/attr_readers"

class MXFinfo
  extend Forwardable
  # We assume it's in the path of the user
  @@binary = 'avidmxfinfo'

  # BINARY CHANGE
  def self.binary; @@binary; end
  def self.binary=(binary); @@binary = binary; end

  def self.scan(path)
    info = InfoObject.new(path)
    info.valid? ? info : nil
  end

  def self.import(data)
    InfoObject.new(data, false)
  end

  class InfoObject
    extend AttrReaders

    # RAW AVIDMXFINFO OUTPUT
    def raw_data; @mxfinfo; end

    # Attributes
    def self.supported_attributes; @supported_attributes ||= []; end

    def initialize(input, process = true)
      if process
        @filepath = File.absolute_path(input)
        File.exists?(@filepath) ? @valid = true : @valid = false
        @mxfinfo = mxfinfo if @valid
      else
        @valid = true
        @mxfinfo = input
      end
      process_data if @valid
    end

    def valid? 
      @valid
    end

    mxfinfo_attr_reader :filename
    mxfinfo_attr_reader :project_name
    mxfinfo_attr_reader :clip_name
    mxfinfo_date_reader :clip_created_at, "Clip Created"
    mxfinfo_attr_reader :project_edit_rate
    mxfinfo_attr_reader :clip_edit_rate
    mxfinfo_duration_reader :clip_duration

    mxfinfo_attr_reader :videotracks, "Clip Video Tracks"
    def v_tracks; videotracks.to_i; end
    alias_method :video_tracks, :v_tracks

    mxfinfo_attr_reader :audiotracks, "Clip Audio Tracks"
    def a_tracks; audiotracks.to_i; end
    alias_method :audio_tracks, :a_tracks

    mxfinfo_attr_reader :clip_track_string
    mxfinfo_attr_reader :essence_type
    mxfinfo_attr_reader :essence_label

    mxfinfo_attr_reader :tracknumber, "Track Number"
    def t_number; tracknumber.to_i; end
    alias_method :track_number, :t_number

    mxfinfo_attr_reader :edit_rate
    mxfinfo_duration_reader :track_duration
    mxfinfo_duration_reader :track_segment_duration
    mxfinfo_duration_reader :track_segment_offset
    mxfinfo_duration_reader :start_timecode
    mxfinfo_attr_reader :audio_sampling_rate

    mxfinfo_attr_reader :channelcount, "Channel Count"
    def c_count; channelcount.to_i; end
    alias_method :channel_count, :c_count

    mxfinfo_attr_reader :quantizationbits, "Quantization Bits"
    def q_bits; quantizationbits.to_i; end
    alias_method :quantization_bits, :q_bits

    mxfinfo_attr_reader :unc_path
    mxfinfo_attr_reader :material_package_uid
    mxfinfo_attr_reader :file_package_uid
    mxfinfo_attr_reader :physical_package_uid
    mxfinfo_attr_reader :physical_package_type
    mxfinfo_attr_reader :physical_package_name
    mxfinfo_attr_reader :physical_package_locator

    private
      def mxfinfo
        # TODO: - add check if return data is valid
        `#{MXFinfo.binary} #{@filepath.shellescape} 2>&1`
      end

      def process_data
        @processed_data = Hash.new
        @mxfinfo.each_line do |line|
          if line.include?("=")
            key, value = line.split("=")[0], line.split("=")[1]
            @processed_data[key.strip.gsub(/\s+/, "_").downcase] = value.strip
          end
        end
      end
  end
end
