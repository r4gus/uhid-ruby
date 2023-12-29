# Uhid Ruby

## Build

### From source

To build from source you need Ruby, and rake.

```bash
gem install rake-compiler
```

Then run one of the following commands:
* `rake compile` - this compiles the C extension and places the `uhid.so` into `lib/uhid/`.
* `gem build` - build a gem
* `rake native gem` - build a native gem using rake

### Build gem

```
rake compile
gem build uhid.gemspec
gem push uhid-x.y.z.gem
```

## Docs

To build the documentation you need Yard.

```bash
gem install yard
```

Then run `yard doc`.

## Testing

Run tests with `./bin/rspec --format doc`.
