The GPU driver code is taken from the Linux kernel. As we only have kernel
space, we don't implement KMS.

It seems that there are some files where `int` is used instead of strict integer
types. I think that using e.g. `u32` everywhere is better.
