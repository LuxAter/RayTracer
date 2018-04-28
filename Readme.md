To run on BLT run the command:

```
SGE_Batch -r "name" -c "LD_LIBRARY_PATH:./build/libpng/lib:$LD_LIBRARY_PATH ray" -P cores
```
