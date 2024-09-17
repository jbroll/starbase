The statements or expression is preprocessed by the program before passed to
awk for evaluation.  Column and header value names are substituted for thier
values in the table.  Column names may be subscripted with an array index and
the column data will be split on the ARRDEL environment variable.  Header
values may be also subscripted, but assignments made to header values will not
appear in the output table.

