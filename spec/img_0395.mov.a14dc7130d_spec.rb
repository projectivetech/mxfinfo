require 'spec_helper'

describe "IMG_0395.MOV.A14DC7130D.mxf" do
  before (:all) do 
    infoFile = File.open(name_to_fixture "IMG_0395.MOV.A14DC7130D.mxf.info")
    @info = MXFinfo.import(infoFile.read)
  end

  it "should have a filename" do
    @info.filename.should == "/home/steved/Documents/git/mxfinfo/spec/fixtures/IMG_0395.MOV.A14DC7130D.mxf"
  end

  it "should have a project name" do
    @info.project_name.should == "Ballerinas_zippy"
  end

  it "should have a clip name" do
    @info.clip_name.should == "IMG_0395.MOV"
  end

  it "should have a clip created at" do
    @info.clip_created_at.should == Time.parse("2011-05-08 22:02:53.000")
  end

  it "should have a edit rate" do
    @info.project_edit_rate.should == "25/1"
  end

  it "should have a clip duration" do
    @info.clip_duration.should == 11120
  end

  it "should have a clip video tracks count" do
    @info.video_tracks.should == 1
  end

  it "should have a clip audio tracks count" do
    @info.audio_tracks.should == 1
  end

  it "should have a clip tracks string" do
    @info.clip_track_string.should == "V1 A1"
  end

  it "should have a essence type" do
    @info.essence_type.should == "PCM"
  end

  it "should have a essence label" do
    @info.essence_label.should == "060e2b34040101010d01030102060200"
  end

  it "should have a track number" do
    @info.track_number.should == 1
  end

  it "should have a edit rate" do
    @info.edit_rate.should == "48000/1"
  end

  it "should have a track duration" do
    @info.track_duration.should == 11120
  end

  it "should have a track segment duration" do
    @info.track_segment_duration.should == 11120
  end

  it "should have a track segment offset" do
    @info.track_segment_offset.should == 0
  end

  it "should have a start timecode" do
    @info.start_timecode.should == "00:00:00:00"
  end

  it "should have a audio sampling rate" do
    @info.audio_sampling_rate.should == "48000/1"
  end

  it "should have a channel count" do
    @info.channel_count.should == 1
  end

  it "should have a quantization bits" do
    @info.quantization_bits.should == 16
  end

  it "should have a unc path" do
    @info.unc_path.should == "Macintosh HD:Users:susannehassepass:Desktop:London 1video:IMG_0395.MOV"
  end

  it "should have a material package uid" do
    @info.material_package_uid.should == "060a2b340101010101010f00130000004dc7130d05831a0a060e2b347f7f2a80"
  end

  it "should have a file package uid" do
    @info.file_package_uid.should == "060a2b340101010101010f00130000004dc7130d05841a0a060e2b347f7f2a80"
  end

  it "should have a physical package uid" do
    @info.physical_package_uid.should == "060a2b340101010101010f00130000004dc7130d05851a0a060e2b347f7f2a80"
  end

  it "should have a physical package type" do
    @info.physical_package_type.should == "Import"
  end

  it "should have a physical package name" do
    @info.physical_package_name.should == "IMG_0395.MOV"
  end

  it "should have a physical package locator" do
    @info.physical_package_locator.should == "file:///Macintosh%20HD/Users/susannehassepass/Desktop/London%201video/IMG_0395.MOV"
  end
end