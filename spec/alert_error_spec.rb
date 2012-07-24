require 'spec_helper'

describe "ALERT01.ERROR.info" do

  it "should return an invalid MXF file" do
    infoFile = File.open(name_to_fixture "ALERT01.ERROR.info")
    @info = MXFinfo.import(infoFile.read)
    @info.valid?.should == false
  end
end