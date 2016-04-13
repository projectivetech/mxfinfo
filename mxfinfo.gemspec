require File.expand_path('../lib/mxfinfo/version', __FILE__)

Gem::Specification.new do |gem|
  gem.authors       = ['Steve Dierker', 'Malte Rohde']
  gem.email         = ['technology@flavoursys.com']
  gem.description   = %q{MXFinfo is a gem that wraps avidmxfinfo from libmxf in a native ruby extension.}
  gem.summary       = %q{MXFinfo is a gem that wraps avidmxfinfo from libmxf in a native ruby extension.}
  gem.homepage      = 'http://github.com/flavoursys/mxfinfo'

  gem.files         = `git ls-files`.split($\)
  gem.executables   = gem.files.grep(%r{^bin/}).map{ |f| File.basename(f) }
  gem.test_files    = gem.files.grep(%r{^(test|spec|features)/})
  gem.extensions    = ['ext/mxfinfo/extconf.rb']
  gem.name          = 'mxfinfo'
  gem.require_paths = ['lib']
  gem.version       = MXFInfo::VERSION
  gem.license       = 'MIT'

  gem.required_ruby_version = '>= 1.9.3'

  gem.add_development_dependency 'rspec', '~> 3'
  gem.add_development_dependency 'rspec-its'
  gem.add_development_dependency 'rake-compiler', '~> 0.9'
end

