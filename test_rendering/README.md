# test_rendering

This is a test ROM testing consistency between RDP
configuration methods in libdragon.

The methods compared are:

- with/without `rdpq_mode_begin`/`end` batching
- with/without block recording

This test rom does not display anything, it only outputs to
a terminal (including drawing framebuffers to the terminal).

A test is failed if the results (i.e. framebuffer contents)
of a configuration method doesn't match the initial
configuration run results (without batching, without block
recording).

The last line printed shows the amount of failed tests:
`n_failures = 0`.

To add a test, see the `trf_list` array.

## TODO

- Add more tests
- Test frozen blocks
- Permute trfs for a given test
