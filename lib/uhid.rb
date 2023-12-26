class Uhid
  VERSION = "0.1.0"

  RD_FIDO = "\x06\xd0\xf1\x09\x01\xa1\x01\x09\x20\x15\x00\x26\xff\x00\x75\x08\x95\x40\x81\x02\x09\x21\x15\x00\x26\xff\x00\x75\x08\x95\x40\x91\x02\xc0".force_encoding("BINARY")

  def initialize(name = "ruby-fido", rd = RD_FIDO, vendor = 0x15d9, product = 0x0a37, path = "/dev/uhid")
    @file = File.open(path, "r+", File::NONBLOCK)
    ObjectSpace.define_finalizer(self, method(:finalize))
    self.create(name, vendor, product, rd)
  end
  
  private

  def finalize(_id)
    @file.close
  end
end

require_relative "uhid/uhid"



