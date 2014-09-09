# Global MKPKG definitions for the TABLES packages.

#$set XFLAGS	= "$(XFLAGS) -p starbase"
#$set XVFLAGS	= "$(XVFLAGS) -p starbase"
#$set LFLAGS	= "$(LFLAGS) -p starbase"

$set XFLAGS     = "$(XFLAGS) -c -F -g -q -p starbase"
$set XVFLAGS    = "$(XVFLAGS) -p starbase"
$set LFLAGS     = "$(LFLAGS) -d -g -q -p starbase"

$ifeq (MACH, sparc) then
$include "starbase$lib/mkpkg.sf.SUN4"
$else $ifeq (MACH, i386) then
$include "starbase$lib/mkpkg.sf.I386"
$else
$include "starbase$lib/mkpkg.sf.SUN3"
$end
#$include "starbase$lib/mkpkg.sf.VMS"
