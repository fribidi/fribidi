# The fribidi binary is used by the test setup, so if bin=false we still
# need to build it for internal usage, we just won't install it.
gen_man_pages = 'fribidi.1'
fribidi = executable('fribidi',
  'fribidi-main.c', 'getopt.c', 'getopt1.c', fribidi_unicode_version_h,
  c_args: ['-DHAVE_CONFIG_H'] + fribidi_static_cargs,
  include_directories: incs,
  link_with: libfribidi,
  install: get_option('bin'))

if not get_option('bin')
  subdir_done()
  gen_man_pages = []
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

python3 = import('python').find_installation()
result = run_command(python3, '-c', '''import os.path; import sys
sys.exit(0 if os.path.isfile('@0@') else 1)'''.format(join_paths(meson.current_source_dir(), gen_man_pages)),
  check: false)
have_man_pages = result.returncode() == 0
message('Have pre-generated man pages: @0@'.format(have_man_pages))

if have_man_pages
  install_man(gen_man_pages)
elif get_option('docs')
  help2man = find_program('help2man', required: false)
  if not help2man.found()
    error('help2man is required to build documentation from git.')
  endif

  target = fribidi.name() + '.1'
  custom_target(target,
    input: fribidi,
    output: target,
    command: ['sh', '-c', help2man.full_path() + ' --no-info --version-string=- -n "$(@INPUT@ --help | head -n 3 | tail -n 1)" --output=$0 @INPUT@', '@OUTPUT@'],
    depends: [fribidi],
    install_dir: join_paths(get_option('prefix'), get_option('mandir'), 'man1'),
    install: true)
endif
