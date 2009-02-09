DIR = boot
BOOT_OBJECTS = boot.o 
BOOT_OBJECTS += video.o

TARGET = build_in.o

$(TARGET): $(BOOT_OBJECTS)
	@$(AR) -r -o $@ $(BOOT_OBJECTS) > /dev/null 2>&1
	@echo "AR $@"

.S.o:
	@$(CC) $(CCAFLAGS) -c $<
	@echo "CC $@"
.c.o:
	@$(CC) $(CFLAGS) -c $<
	@echo "CC $@"

clean:
	@$(RM) -rf $(BOOT_OBJECTS) $(TARGET) 
