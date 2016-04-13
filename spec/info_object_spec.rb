require 'spec_helper'

describe 'MXFInfo::InfoObject' do
  context 'when created from an valid MXF file' do
    subject do
      MXFInfo.scan File.expand_path('../fixtures/IMG_0395.MOV.A14DC7130D.mxf', __FILE__)
    end

    its(:project_name) { should eq('Ballerinas_zippy') }
    its(:clip_name) { should eq('IMG_0395.MOV') }
    its(:clip_created_at) { should eq(Time.parse('2011-05-08 22:02:53.000')) }
    its(:project_edit_rate) { should eq((25/1)) }
    its(:clip_duration) { should eq(287) }
    its(:clip_edit_rate) { should eq((25/1)) }
    its(:video_tracks) { should eq(1) }
    its(:audio_tracks) { should eq(1) }
    its(:clip_track_string) { should eq('V1 A1') }
    its(:essence_type) { should eq('PCM') }
    its(:essence_label) { should eq('060e2b34040101010d01030102060200') }
    its(:track_number) { should eq(1) }
    its(:edit_rate) { should eq((48000/1)) }
    its(:track_duration) { should eq(551040) }
    its(:segment_duration) { should eq(551040) }
    its(:segment_offset) { should eq(0) }
    its(:start_timecode) { should eq(0) }
    its(:audio_sampling_rate) { should eq((48000/1)) }
    its(:quantization_bits) { should eq(16) }
    its(:channel_count) { should eq(1) }
    its(:material_package_uid) { should eq('060a2b340101010101010f00130000004dc7130d05831a0a060e2b347f7f2a80') }
    its(:file_package_uid) { should eq('060a2b340101010101010f00130000004dc7130d05841a0a060e2b347f7f2a80') }
    its(:physical_source_package_uid) { should eq('060a2b340101010101010f00130000004dc7130d05851a0a060e2b347f7f2a80') }
    its(:physical_package_type) { should eq(:import) }
    its(:physical_package_name) { should eq('IMG_0395.MOV') }
    its(:physical_package_locator) { should eq('file:///Macintosh%20HD/Users/susannehassepass/Desktop/London%201video/IMG_0395.MOV') }
  end
end
