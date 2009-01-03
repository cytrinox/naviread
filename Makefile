# Makefile for Naviread by Alexander Steffen (c) Markus Hoffmann V.1.00

# Directories
prefix=/usr/local
exec_prefix=${prefix}

BINDIR=${exec_prefix}/bin
DATADIR=${prefix}/share
LIBDIR=${exec_prefix}/lib
MANDIR=${prefix}/man



all: naviread


OBJS= naviread.o



CSRC=$(OBJS:.o=.c)
HSRC= naviread.h

DIST= README INSTALL COPYING AUTHORS RELEASE_NOTES Makefile $(CSRC) $(HSRC) \

BINDIST= README RELEASE_NOTES naviread 

DIR=naviread-0.2.1
DIR2=$(DIR)-bin

naviread: $(OBJS) Makefile
	$(CC) -o $@ $(OBJS)
	strip $@
clean:
	rm -f *.o *.a *.so *.bak *~ naviread *-bin.zip
dist :	$(DIR).tar.gz

$(DIR).tar.gz : $(DIST)
	rm -rf /tmp/$(DIR)
	mkdir /tmp/$(DIR)
	(tar cf - $(DIST))|(cd /tmp/$(DIR); tar xpf -)
	(cd /tmp; tar cf - $(DIR)|gzip -9 > $(DIR).tar.gz)
	mv /tmp/$(DIR).tar.gz .

bindist : $(DIR2).zip

$(DIR2).zip : $(BINDIST)
	rm -rf /tmp/$(DIR2)
	mkdir /tmp/$(DIR2)
	mkdir /tmp/$(DIR2)/bin
	rsync -a README /tmp/$(DIR2)/
	rsync -a RELEASE_NOTES /tmp/$(DIR2)/
	rsync -a naviread /tmp/$(DIR2)/bin/
	(cd /tmp ; zip -r $(DIR2).zip $(DIR2))
	mv /tmp/$(DIR2).zip .	
depend : $(DEPSRC) 
	cp Makefile Makefile.bak
	chmod +w Makefile
	makedepend $(INC) $(DEF) $(DEPSRC) 
	sed -n '/^# DO NOT DELETE/,$$p' < Makefile > Makefile.tmp
	sed -n '1,/^# DO NOT DELETE/p' < Makefile > Makefile.new
	tail +2 Makefile.tmp|\
	sed 's/^\([A-Za-z0-9_]*\)\.o:/\1.o \1.ln:/g'>>Makefile.new
	rm -f Makefile.tmp
	mv Makefile.new Makefile

# DO NOT DELETE
