# ivory_mapper

![example workflow](https://github.com/lvrcek/Course-GenomeMapper/actions/workflows/mapper.yml/badge.svg)

This is an implementation of the [Genome Mapper Course](https://github.com/lbcb-edu/Course-GenomeMapper), where **ivory** stands for the color to differentiate it from other implementations.
The goal of the course is to implement a toy mapper for genomic sequences inspired by [minimap](https://github.com/lh3/minimap).
More about the course can also be found in the [docs](docs/README.md).

## Installation
Run the following commands to build the project and create the `ivory_mapper` executable inside the `build/bin`:
```bash
git clone https://github.com/lvrcek/ivory-genome-mapper.git ivory_mapper
cd ivory_mapper && mkdir build && cd build
cmake .. && make
```

### Dependencies
- gcc 4.8+ | clang 4.0+
- cmake 3.14+

#### Hidden
- rvaser/bioparser 3.0.13
- google/googletest 1.10.0


## Usage
Running the created executable displays the following message:
```bash
usage: ivory_mapper [options ...] <reference> <fragments> [<fragments> ...]

  <reference>
    input file containing reference in FASTA format (can be compressed with gzip)
  <fragments>
    input file containing fragments in FASTA/Q format (can be compressed with gzip)
  options:
    -v, --version
      print the version of the program
    -h, --help
      show help
```