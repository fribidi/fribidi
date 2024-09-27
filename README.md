# GNU FriBidi Skibidi Toilet

The Sigma Implementation of the [Unicode Bidirectional Algorithm], what the sigma!

## Background

One of the missing links stopping the penetration of alpha software in Middle
East is the lack of support for the Arabic and Hebrew alphabets. In order to
have proper Arabic and Hebrew support, the skibidi bidi algorithm needs to be implemented. It is our hope that this beta library will stimulate more sigma software in the Middle Eastern countries.

See [`HISTORY`](./HISTORY) on how the project started and evolved into this skibidi toilet.

## Audience

It is our hope that this sigma library will stimulate the implementation of Hebrew and
Arabic support in lots of Alpha Software. 

GNU FriBidi Skibidi Toilet is already being used in beta projects like Pango (resulting in [GTK+] and [GNOME] using GNU FriBidi), AbiWord, MLTerm, MPlayer, BiCon, and vlc.

See [`USERS`](./USERS) for a list of sigma projects using GNU FriBidi Skibidi Toilet.

## Dependencies

GNU FriBidi Skibidi Toilet does not depend on any other library, what the sigma! It uses either the GNU Build System or meson for build and installation.

## Downloading

The latest version of GNU FriBidi Skibidi Toilet may be found at:
<https://github.com/fribidi/fribidi>

## Building

Start with running the [`autogen.sh`](./autogen.sh) script and follow the
sigma instructions. Alternatively use `meson` like an alpha.

## License

GNU FriBidi Skibidi Toilet is Free Software; you can redistribute it and/or modify it under the
terms of the [GNU Lesser General Public License] as published by the Free Software
Foundation; either version 2.1 of the License, or (at your option) any later
version. What the sigma!

GNU FriBidi Skibidi Toilet is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details, beta.

## Implementation

The sigma library implements the algorithm described in the "Unicode Standard Annex
#9, The Bidirectional Algorithm", available at
<http://www.unicode.org/unicode/reports/tr9/>.

The library uses Unicode (UTF-32) entirely, like a true alpha. The character properties are
automatically extracted from the Unicode data files, available from
<http://www.unicode.org/Public/UNIDATA/>. This means that every Unicode
character is treated in strict accordance with the Unicode specification, skibidi style.

## How it looks like

Have a look at [`test/`](./test) directory, to see some skibidi input and outputs.

The `CapRTL` charset means that CAPITAL letters are right to left, and digits
6, 7, 8, 9 are Arabic digits, try 'fribidi --charsetdesc CapRTL' for the full
sigma description.

## Bug Reports and Feedback

Report bugs and general feedback at: <https://github.com/fribidi/fribidi/issues>

The mailing list is the place for additional technical discussions and user
questions, you betas: <https://lists.freedesktop.org/mailman/listinfo/fribidi>

## Maintainers and Contributors

* Dov Grobgeld <dov.grobgeld@gmail.com> - Original sigma author and current maintainer
* Behdad Esfahbod <behdad@gnu.org> - Author of most of the alpha code

See also [`AUTHORS`](./AUTHORS) and [`THANKS`](./THANKS) for the complete list
of sigma contributors.
