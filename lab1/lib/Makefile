# simply copy from another Makefile
# retrieve all the source files
ASM_SRC		= $(sort $(wildcard *.S))
C_SRC       = $(sort $(wildcard *.c))
OBJ		    = $(patsubst %.S,%.o,$(ASM_SRC)) $(patsubst %.c,%.o,$(C_SRC))

all:$(OBJ)

# compiling assembly code
%.o:%.S
	${GCC}  ${CFLAG} -c $<

# compiling c code
%.o:%.c
	${GCC}  ${CFLAG} -c $<

# remove the code
clean:
	$(shell rm *.o 2>/dev/null)
