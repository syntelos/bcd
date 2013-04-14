
  This binary coded decimal software package is oriented to
  applications requiring a data structure for each base ten digit in a
  number.

  In BCD.h, the digit is a data structure named "BCDDigit".
  Applications may extend this data structure for their own purposes.

  The number is a data structure containing an array of digits.

  Currently, BCD numbers are defined from floating point (32 bit)
  values.

  Status

    Initial definition and testing complete.  See "make run" and
    "test.in" for more info.

  Targets

    Endian agnostic with 32 bit math (recommend 16 bit or better).
    See "BCDSetFloat" in BCD.c for evaluation.

  Test vectors

    The file "test.in" contains a set of test vectors that are
    evaluated by the ancillary program "test.c" using "make run".

  Author

    John Pritchard

      https://github.com/syntelos/bcd

