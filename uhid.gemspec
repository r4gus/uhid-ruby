Gem::Specification.new do |s|
  s.name        = "uhid"
  s.version     = "0.1.1"
  s.summary     = "Linux uhid library"
  s.description = "Create virtual USB devices on Linux using the uhid module."
  s.authors     = ["David P. Sugar"]
  s.email       = "david@thesugar.de"
  s.extensions  = %w[ext/uhid/extconf.rb]
  s.files       = ["lib/uhid.rb"]
  s.files       += Dir["ext/**/*"]
  s.homepage    = "https://github.com/r4gus/uhid-ruby"
  s.license     = "MIT"
end
