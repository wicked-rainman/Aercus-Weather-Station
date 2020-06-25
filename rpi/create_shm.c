void* create_shm(size_t size) {
  int protection;
  int visibility;

	visibility  = MAP_SHARED | MAP_ANONYMOUS;
	protection = PROT_READ | PROT_WRITE;
  	return mmap(NULL, size, protection, visibility, -1, 0);
}
