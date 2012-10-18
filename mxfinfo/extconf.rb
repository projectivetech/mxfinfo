require 'mkmf'

unless pkg_config('libMXF-1.0')
	raise "libMXF-1.0 could not be found!"
end

$warnflags = ''
$defs.push '--std=gnu99'
$defs.push '-D_GNU_SOURCE'
$defs.push '-Wall'

create_makefile('mxfinfo')
