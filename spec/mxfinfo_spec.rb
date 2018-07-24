require 'spec_helper'

describe 'MXFInfo' do
  describe 'scan' do
    context 'given an invalid MXF file' do
      it 'should raise an error' do
        expect {
          MXFInfo.scan File.expand_path('../fixtures/InvalidFileTest.mxf', __FILE__)
        }.to raise_error(/Failed to read header partition/)
      end
    end

    context 'given an invalid MXF file' do
      it 'should raise not an error' do
        expect {
          MXFInfo.scan File.expand_path('../fixtures/IMG_0395.MOV.A14DC7130D.mxf', __FILE__)
        }.to_not raise_error
      end

      it 'should return an InfoObject instance' do
        expect(MXFInfo.scan File.expand_path('../fixtures/IMG_0395.MOV.A14DC7130D.mxf', __FILE__)).to be_a(MXFInfo::InfoObject)
      end

      it 'should return an UTF-8 clip_name' do
        expect(MXFInfo.scan(File.expand_path('../fixtures/IMG_0395.MOV.A14DC7130D.mxf', __FILE__)).clip_name.encoding).to eq(Encoding::UTF_8)
      end
    end
  end
end

