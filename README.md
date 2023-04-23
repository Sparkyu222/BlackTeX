# BlackTeX

### About this project
BlackTeX is a PGN to LaTeX converter, you just need to input a PGN file and it will convert the file into a beautiful TEX file.

This is a college project made by [Evios17](https://github.com/Evios17), [Sparkyu222](https://github.com/Sparkyu222) and [ch2792](https://github.com/ch2792).

### Usage
Basic usage :
```bash
$ blacktex <input_file>
```
> This command will convert the PGN input file to a TEX file with the same name as the input

You can also use options :
```bash
$ blacktex -i <input_file> -o <output_file> -n <counts>
```
> `-i`: specify the input file
> 
> `-o` : specify the output file
> 
> `-n` : specify the number of counts before showing the chessboard in the output

### Building
To build the converter, you just need to execute the `build.sh` script :
```bash
$ chmod u+x build.sh
$ ./build.sh
```
