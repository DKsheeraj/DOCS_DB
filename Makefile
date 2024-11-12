all: build

build: install

rebuild: clean install

install:
	cmake -S . -B ./build
	cmake --build ./build

unistall:
	find ./ -name "build" -type d -exec rm -rf {} +

hash_table.a:
	cmake -S ./table/ -B ./table/build
	cmake --build ./table/build

clean: unistall
	rm -rf ./lib/*

tests:
	cmake -S ./test/ -B ./test/build
	cmake --build ./test/build
	./test/build/TEST

cppcheck: install
	@cd build/; make cppcheck

clang-format: install
	@cd build/; make clang-format

lcov: test
	cd ./test/build; make coverage;
	open ./test/build/coverage-report/index.html 

.PHONY: all build rebuild unistall clean cppcheck clang-format tests lcov
