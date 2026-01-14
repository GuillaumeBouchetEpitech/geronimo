#!/bin/bash

# dot -Kneato -Tsvg mindmap.dot > mindmap.svg
dot -Kfdp -Tsvg mindmap.dot > mindmap.svg
# dot -Kdot -Tsvg mindmap.dot > mindmap.svg

dot -Kdot -Tsvg dependencies.dot > dependencies.svg

