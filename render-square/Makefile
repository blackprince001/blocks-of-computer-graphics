install:
	meson wrap install glfw glm

build:
	meson setup builddir

clean:
	rm -rf ./builddir .cache

compile:
	cd builddir && ninja

run-bin:
	./builddir/render-square
