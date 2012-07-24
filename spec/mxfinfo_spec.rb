require 'spec_helper'

describe MXFinfo do
  before(:each) do
    MXFinfo .class_variable_set :@@binary, 'avidmxfinfo'
  end

  describe "avidmxfinfo binary change" do
    it "should be default on 'avidmxfinfo'" do
      MXFinfo.binary.should == "avidmxfinfo"
    end

    it "should be changeable by MXFinfo.binary='new_path' " do
      MXFinfo.binary.should == "avidmxfinfo"
      MXFinfo.binary = "/bin/avidmxfinfo"
      MXFinfo.binary.should == "/bin/avidmxfinfo"
    end
  end

  it "should be instantiated with valid path" do
    info = MXFinfo.scan(name_to_fixture "IMG_0395.MOV.A14DC7130D.mxf")
    info.nil?.should == false
    info.raw_data.empty?.should == false
  end

  #it "should work with whitespace filenames" do
  #  MXFinfo.scan(name_to_fixture "Shellescape Test.mxf").should_not == nil
  #end

  it "should be possible to access raw data" do
    info = MXFinfo.scan(name_to_fixture "IMG_0395.MOV.A14DC7130D.mxf")
    info.nil?.should == false
    infoFile = File.open(name_to_fixture "IMG_0395.MOV.A14DC7130D.mxf.info")
    info.raw_data.should eql infoFile.read
  end

  it "should be possible to create infoObject from avidmxfinfo dump" do
    infoFile = File.open(name_to_fixture "IMG_0395.MOV.A14DC7130D.mxf.info")
    info = MXFinfo.import(infoFile.read)
    info.valid?.should == true
  end

  it "should have supported attributes" do
    supported_attributes = [:project_name,
      :filename,
      :clip_name,
      :clip_created_at,
      :project_edit_rate,
      :clip_edit_rate,
      :clip_duration,
      :videotracks,
      :audiotracks,
      :clip_track_string,
      :essence_type,
      :essence_label,
      :tracknumber,
      :edit_rate,
      :track_duration,
      :track_segment_duration,
      :track_segment_offset,
      :start_timecode,
      :audio_sampling_rate,
      :channelcount,
      :quantizationbits,
      :unc_path,
      :material_package_uid,
      :file_package_uid,
      :physical_package_uid,
      :physical_package_type,
      :physical_package_name,
      :physical_package_locator]
    MXFinfo::InfoObject.supported_attributes.each do |a|
      supported_attributes.include?(a).should == true
    end
  end
end