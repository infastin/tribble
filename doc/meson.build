config_data = configuration_data()
config_data.set('VERSION', libversion)

libtribble_toml = configure_file(
  input: 'libtribble.toml.in',
  output: 'libtribble.toml',
  configuration: config_data,
)

gidocgen = find_program('gi-docgen')

docsdir = datadir / 'doc'

custom_target('tribble-doc',
  input: [libtribble_toml, libtribble_gir[0]],
  output: 'libtribble-@0@'.format(apiversion),
  command: [
    gidocgen,
    'generate',
    '--quiet',
    '--config=@INPUT0@',
    '--output-dir=@OUTPUT@',
    '--no-namespace-dir',
    '@INPUT1@',
  ],
  build_by_default: true,
  install: true,
  install_dir: docsdir,
)
