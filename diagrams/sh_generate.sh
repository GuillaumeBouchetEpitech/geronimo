#!/bin/bash

# dot -Kdot -Tpng mmo-fps-test.dot > mmo-fps-test.png

# dot -Kneato -Tsvg mindmap.dot > mindmap.svg
dot -Kfdp -Tsvg mindmap.dot > mindmap.svg
# dot -Kdot -Tsvg mindmap.dot > mindmap.svg

# dot -Kdot -Tpng test.dot > test.png
# # dot -Kneato -Tsvg wasm-libcryptopp.dot > wasm-libcryptopp.svg
# # dot -Kneato -Tpng wasm-libcryptopp.dot > wasm-libcryptopp.png
# dot -Kdot -Tsvg wasm-libcryptopp.dot > wasm-libcryptopp.svg
# dot -Kdot -Tpng wasm-libcryptopp.dot > wasm-libcryptopp.png
