install-glfw:
	meson wrap install glfw

install-glm:
	meson wrap install glm

build:
	meson setup builddir

clean:
	rm -rf ./builddir .cache

compile:
	cd builddir && ninja

run-bin:
	./builddir/render-square
