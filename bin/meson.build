# The fribidi binary is used by the test setup, so if bin=false we still
# need to build it for internal usage, we just won't install it.
fribidi = executable('fribidi',
  'fribidi-main.c', 'getopt.c', 'getopt1.c', fribidi_unicode_version_h,
  c_args: ['-DHAVE_CONFIG_H'] + fribidi_static_cargs,
  include_directories: incs,
  link_with: libfribidi,
  install: get_option('bin'))

if not get_option('bin')
  subdir_done()
endif

executable('fribidi-benchmark',
  'fribidi-benchmark.c', 'getopt.c', 'getopt1.c', fribidi_unicode_version_h,
  c_args: ['-DHAVE_CONFIG_H'] + fribidi_static_cargs,
  include_directories: incs,
  link_with: libfribidi,
  install: false)

executable('fribidi-bidi-types',
  'fribidi-bidi-types.c', fribidi_unicode_version_h,
  c_args: ['-DHAVE_CONFIG_H'] + fribidi_static_cargs,
  include_directories: incs,
  link_with: libfribidi,
  install: false)

executable('fribidi-caprtl2utf8',
  'fribidi-caprtl2utf8.c', fribidi_unicode_version_h,
  c_args: ['-DHAVE_CONFIG_H'] + fribidi_static_cargs,
  include_directories: incs,
  link_with: libfribidi,
  install: false)

if get_option('fuzzer_ldflags') != ''
  fribidi_fuzzer = executable('fribidi-fuzzer',
    'fribidi-fuzzer.c', fribidi_unicode_version_h,
    c_args: ['-DHAVE_CONFIG_H'] + fribidi_static_cargs,
    include_directories: incs,
    link_with: libfribidi,
    link_args: get_option('fuzzer_ldflags').split(),
    install: false)
endif
