require 'spec_helper'

describe "IMG_0395.MOV.A14DC7130D.mxf" do

  it "should recognize all render files" do
    ["Center Smooth",
    "Color Correction",
    "Crystal",
    "Flip-Flop",
    "FluidBlur",
    "Left Box",
    "Luma Key",
    "Mosaic Effect",
    "Stabilize",
    "Timecode Burn-In"].each do |f|
      infoFile = File.open(name_to_fixture "#{f.downcase.gsub(' ','_')}.info")
      @info = MXFinfo.import(infoFile.read)
      @info.physical_package_name.should == "Precompute Source Mob"
      @info.clip_name.include?(f).should == true
      @info.render_file?.should == true
    end
  end


end