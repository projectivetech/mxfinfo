require 'mkmf'

pkg_config('libMXF-1.0')

$warnflags = ''
$defs.push '--std=c99'
$defs.push '-Wall'

create_makefile('avidmxfinfo')
