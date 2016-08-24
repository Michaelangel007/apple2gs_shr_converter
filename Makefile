all: a2b b2d m2s xpack

clean:
	rm a2b b2d m2s xpack

a2b:
	cd src_a2b && $(MAKE)

b2d:
	cd src_b2d && $(MAKE)

m2s:
	cd src_m2s && $(MAKE)

xpack:
	cd src_xpack && $(MAKE)

