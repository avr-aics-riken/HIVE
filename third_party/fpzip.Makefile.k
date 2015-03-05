# compile-time options --------------------------------------------------------

# optimize for speed; may impact correctness and portability
FPZIP_FP = FPZIP_FP_FAST

# use volatile accumulator
#FPZIP_FP = FPZIP_FP_SAFE

# emulate floating-point arithmetic
#FPZIP_FP = FPZIP_FP_EMUL

# reinterpret floating-point numbers as integers; most portable
#FPZIP_FP = FPZIP_FP_INT

# output buffer size (ideally the disk block size)
FPZIP_BLOCK_SIZE = 0x1000

# bitwise type conversion mechanisms (defaults to memcpy)
#FPZIP_CONV = -DWITH_REINTERPRET_CAST
FPZIP_CONV = -DWITH_UNION


# no changes should be necessary below ----------------------------------------

CXX = mpiFCCpx
#CXXFLAGS += -ansi -Wall -g
#CXXFLAGS += -O2
CXXFLAGS += -Kfast -g
DEFS += -DFPZIP_BLOCK_SIZE=$(FPZIP_BLOCK_SIZE) -DFPZIP_FP=$(FPZIP_FP) -DFPZIP_VERBOSE=$(FPZIP_VERBOSE) $(FPZIP_CONV)

CXX_FILES =	rcdecoder.cpp\
		rcencoder.cpp\
		rcqsmodel.cpp\
		read.cpp\
		write.cpp
O_FILES = $(CXX_FILES:.cpp=.o)

../lib/libfpzip.a: $(O_FILES)
	@rm -f $@
	ar rc $@ $^

.cpp.o:
	$(CXX) $(CXXFLAGS) $(DEFS) -I../inc -c $<

clean:
	@rm -f $(O_FILES) 
