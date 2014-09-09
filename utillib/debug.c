

dad(d, n, m)
			double	*d;
			int	 n, m;
{
  int	j;
  
  for ( j = 0; j < n * m; j++ ) {
    printf("%+12.10e ", d[j]);
    if ( ((j + 1) % m) == 0 ) printf("\n");
  }
  printf("\n");
}



dap(i, n, m)
			int	*i;
			int	 n, m;
{
  int	j;
  
  for ( j = 0; j < n * m; j++ ) {
    printf("0x%08x ", i[j]);
    if ( ((j + 1) % m) == 0 ) printf("\n");
  }
  printf("\n");
}





dapd(d, n, m)
			double	**d;
			int	  n, m;
{
  int	i, j;

  for ( i = 0; i < n; i++ ) {
    for ( j = 0; j < m; j++ )
      printf("%+12.10e ", d[i][j]);
    printf("\n");
  }
  printf("\n");
}


