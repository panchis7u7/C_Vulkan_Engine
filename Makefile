.PHONY: all
	cmake -S . -B out/build/Linux
build: out/build/Linux
	make -C out/build/Linux
run: build
	out/build/Linux/VulkanEngine