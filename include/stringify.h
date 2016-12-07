/* represent as a string (literally, without expansion) */
#define STRING(x) #x

/* represent as a string (after expansion), i.e.
 * #define BLAH 4
 * something(XSTRING(BLAH));
 *
 * becomes something("4");
 * */
#define XSTRING(x) STRING(x)

