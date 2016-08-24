all: a2b b2d m2s

a2b:
	cd a2fcbmp && $(MAKE)

b2d:
	cd src && $(MAKE)

m2s:
	echo "Missing sources!"
