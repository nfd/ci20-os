warning:
	@echo
	@echo "Run 'make download' to retrieve third-party tools (requires Internet"
	@echo "connectivity)."
	@echo
	@echo No changes made.

download: pyelftools

pyelftools:
	git clone https://github.com/eliben/pyelftools.git
	cd pyelftools; git checkout b975892c2ef9167b890d1bc0684e0ba8b3e125fd; cd ..

