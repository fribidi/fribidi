#include <stdio.h>
#include <string.h>
#include <fribidi.h>

char buf[BUFSIZ];
FriBidiChar ubuf[BUFSIZ];
/* fribidi_unicode_to_charset can expand each character up to 4x for UTF-8 */
char outbuf[4 * BUFSIZ + 1];

int
main (
  void
)
{
  FriBidiCharSet caprtl = fribidi_parse_charset ("CapRTL");
  FriBidiCharSet utf8 = fribidi_parse_charset ("UTF-8");

  while (fgets (buf, sizeof buf, stdin))
    {
      char eol[5];

      FriBidiStrIndex len = strlen (buf);
      while (len && (buf[len - 1] == '\n' || buf[len - 1] == '\r'))
	len--;
      strncpy (eol, buf + len, sizeof eol - 1);
      eol[sizeof eol - 1] = 0;
      buf[len] = 0;

      len = fribidi_charset_to_unicode (caprtl, buf, len, ubuf);
      fprintf (stderr, "%d\n", len);
      len = fribidi_unicode_to_charset (utf8, ubuf, len, outbuf);

      outbuf[len] = 0;
      printf ("%s%s", outbuf, eol);
    }

  return 0;
}
