# Linux UHID class.
#
# This class allows the creation of virtual USB HID
# devices on Linux using the `uhid` kernel module.
#
# @author David P. Sugar (r4gus)
class Uhid
  # FIDO2/U2F USB report descriptor
  RD_FIDO = "\x06\xd0\xf1\x09\x01\xa1\x01\x09"\
            "\x20\x15\x00\x26\xff\x00\x75\x08"\
            "\x95\x40\x81\x02\x09\x21\x15\x00"\
            "\x26\xff\x00\x75\x08\x95\x40\x91"\
            "\x02\xc0".force_encoding("BINARY")
  
  # Create a new virtual HID device.
  #
  # @param name [String] the name of the device.
  # @param rd [String] `BINARY` encoded report descriptor.
  # @param vendor [Integer] vendor ID.
  # @param product [Integer] product ID.
  # @param path [String] the file path (usually `/dev/uhid`).
  def initialize(
    name = "ruby-fido", 
    rd = RD_FIDO, 
    vendor = 0x15d9, 
    product = 0x0a37, 
    path = "/dev/uhid"
  )
    @file = File.open(path, "r+", File::NONBLOCK)
    ObjectSpace.define_finalizer(self, method(:finalize))
    self.create(name, vendor, product, rd)
  end
  
  private

  def finalize(_id)
    self.destroy()
    @file.close
  end
end

require_relative "uhid/uhid"



