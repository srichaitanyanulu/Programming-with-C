Note: 

1. Use sudo if you get permission denied error.

2. Execute writer first and then parallelly execute reader. Writer will write the data and wait for the reader to read that data. Till this data is read, the writer waits for the reader.

3. In the output, Reader program was stopped manually using "Cntrl + C".