Gem::Specification.new do |s|
  s.name        = "uhid"
  s.version     = "0.1.0"
  s.summary     = "Linux uhid library"
  s.description = "Create virtual USB devices on Linux using the uhid module"
  s.authors     = ["David Pierre Sugar"]
  s.email       = "david@thesugar.de"
  s.extensions  = %w[ext/uhid/extconf.rb]
  s.files       = ["lib/uhid.rb"]
  s.files       += Dir["ext/**/*"]
  s.homepage    = ""
  s.license     = "MIT"
end
