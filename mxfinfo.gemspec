# -*- encoding: utf-8 -*-
require File.expand_path('../lib/mxfinfo/version', __FILE__)

Gem::Specification.new do |gem|
  gem.authors       = ["Steve Dierker"]
  gem.email         = ["steve.dierker@flavoursys.com"]
  gem.description   = %q{MXFinfo is a gem to use avidmxfinfo from http://ingex.sourceforge.net/ directly in ruby.}
  gem.summary       = %q{MXFinfo is a gem to use avidmxfinfo from http://ingex.sourceforge.net/ directly in ruby.}
  gem.homepage      = "http://github.com/bigzed/mxfinfo"

  gem.files         = `git ls-files`.split($\)
  gem.executables   = gem.files.grep(%r{^bin/}).map{ |f| File.basename(f) }
  gem.test_files    = gem.files.grep(%r{^(test|spec|features)/})
  gem.name          = "mxfinfo"
  gem.require_paths = ["lib"]
  gem.version       = MXFinfo::VERSION
  gem.required_ruby_version = '>= 1.8.1'

end
