Linus Dehner & Dario Bruckner

# MyFind

## Usage

```bash
./myfind [-R] [-i] searchpath filename1 [filename2] ...[filenameN]
```

- -R:
  -  should switch myfind in recursive mode (else the files should only be search in the defined folder) 

- -i
  - case in-sensitive search

- searchpath 
  - can be an absolute or a relative path
- filename 
  - only filenames as plain string 
  - no support for paths, subpaths, wildcards required.

## Output

```bash
<pid>: <filename>: <complete-path-to-found-file>
```

- <pid>
  - the process id of the searching child
- <filename> 
  - the filename of the searched file
- <complete-path-to-found-file> 
  - the path tho the desired file 