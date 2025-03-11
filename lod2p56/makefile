lod2p56: lod2p56.c
	gcc $< -s -o $@ -O2

backup:
	find . -maxdepth 1 -type f -print0 | tar cfz `date +old/v%Y%m%d%H%M.tar.gz` --null -T -
