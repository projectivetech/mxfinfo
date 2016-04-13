require 'bundler/gem_tasks'
require 'rspec/core/rake_task'
require 'rake/extensiontask'

RSpec::Core::RakeTask.new('spec')
task spec: [:compile]
Rake::ExtensionTask.new('mxfinfo')
