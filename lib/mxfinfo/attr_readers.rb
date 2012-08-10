require 'time'

class MXFinfo
  class InfoObject
    module AttrReaders
      # Add attribute reader for AVID MXF INFO attributes
      def mxfinfo_attr_reader(method_name, mxfinfo_key = nil)
        before_type_cast_method_name = "#{method_name}_before_type_cast"
        mxfinfo_key = mxfinfo_key.gsub(/\s+/,"_").downcase if mxfinfo_key

        # Define before_type_cast method alias {method_name}_before_type_cast
        define_method before_type_cast_method_name do
          if value = instance_variable_get("@#{before_type_cast_method_name}")
            value
          else
            key = mxfinfo_key ? mxfinfo_key : method_name.to_s
            value = @processed_data[key]

            instance_variable_set "@#{before_type_cast_method_name}", value
            instance_variable_get "@#{before_type_cast_method_name}"
          end
        end

        # Define after_type_cast method alias {method_name}
        define_method method_name do
          if value = instance_variable_get("@#{method_name}")
            value
          else
            value = send(before_type_cast_method_name)
            value = yield value if value and block_given?

            instance_variable_set "@#{method_name}", value
            instance_variable_get "@#{method_name}"
          end
        end

        MXFinfo::InfoObject.supported_attributes << method_name
      end

      def mxfinfo_date_reader(*a)
        mxfinfo_attr_reader(*a) { |v| Time.parse v }
      end

      def mxfinfo_duration_reader(*a)
        mxfinfo_attr_reader(*a) { |v|
          value = v.split(" ")[2].gsub(/\(|\)/,"")
          t = 0
          tmp = value.split(":")
          t = (tmp[0].to_i*60*60*1000) + (tmp[1].to_i*60*1000) + (tmp[2].to_i * 1000) + (tmp[3].to_i * 10)
          t
        }
      end

      def mxfinfo_timecode_reader(*a)
        mxfinfo_attr_reader(*a) { |v| 
          v.split(" ")[2].gsub(/\(|\)/,"")
        }
      end
    end
  end
end