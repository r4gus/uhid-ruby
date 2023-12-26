require "rake/extensiontask"
require 'rspec/core/rake_task'

spec = Gem::Specification.new do |s|
  s.name        = "uhid"
  s.version     = "0.1.0"
  s.summary     = "Linux uhid library"
  s.description = "Create virtual USB devices on Linux using the uhid module"
  s.authors     = ["David Pierre Sugar"]
  s.email       = "david@thesugar.de"
  s.extensions  = FileList["ext/**/extconf.rb"]
  s.files       = ["lib/uhid.rb"]
  s.files       += Dir["ext/**/*"]
  s.homepage    = ""
  s.license     = "MIT"
end

Gem::PackageTask.new(spec) do |pkg|
end

#Rake::ExtensionTask.new "uhid" do |ext|
#  ext.lib_dir = "lib/uhid"
#end
Rake::ExtensionTask.new("uhid", spec)

# Run tests with `rake spec`
RSpec::Core::RakeTask.new(:spec)
