## Process this file with automake to produce Makefile.in

xmlincdir = $(includedir)/libxml2/libxml

xmlinc_HEADERS = \
		SAX.h \
		entities.h \
		encoding.h \
		parser.h \
		parserInternals.h \
		xmlerror.h \
		HTMLparser.h \
		HTMLtree.h \
		debugXML.h \
		tree.h \
		list.h \
		hash.h \
		xpath.h \
		xpathInternals.h \
		xpointer.h \
		xinclude.h \
		xmlIO.h \
		xmlmemory.h \
		nanohttp.h \
		nanoftp.h \
		uri.h \
		valid.h \
		xlink.h \
		xmlversion.h \
		xmlwin32version.h \
		DOCBparser.h \
		catalog.h \
		threads.h \
		globals.h

install-exec-hook:
	$(mkinstalldirs) $(DESTDIR)$(xmlincdir)

EXTRA_DIST = xmlversion.h.in xmlwin32version.h.in
